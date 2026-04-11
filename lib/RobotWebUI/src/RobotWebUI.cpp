#include "RobotWebUI.h"
#include "ArduinoTransport.h"
#include "pages.h"
#include <ESP8266WiFi.h>

RobotWebUI::RobotWebUI()
    : _transport(nullptr), _motorCount(2) {
}

void RobotWebUI::begin(const char* ssid, const char* password, int motorCount) {
    _motorCount = motorCount;

    // Create Arduino transport implementation
    _transport = new ArduinoTransport();
    if (!_transport) {
        Serial.println("[RobotWebUI] FATAL: Failed to allocate transport");
        return;
    }

    // Register internal WebSocket callbacks
    _transport->onWSMessage([this](const char* data, size_t len) {
        handleWSMessage(data, len);
    });
    _transport->onWSConnect([this](bool connected) {
        handleWSConnect(connected);
    });
    _transport->onWSDisconnect([this](bool connected) {
        handleWSDisconnect(connected);
    });

    // Connect WiFi and start server
    if (!_transport->begin(ssid, password)) {
        Serial.println("[RobotWebUI] WiFi connection failed");
        return;
    }

    // Serve dashboard page from PROGMEM
    _transport->servePage((const uint8_t*)INDEX_HTML, sizeof(INDEX_HTML));

    Serial.printf("[RobotWebUI] Started with %d motor(s)\n", _motorCount);
}

void RobotWebUI::loop() {
    if (!_transport) return;
    _transport->loop();

    // Motor safety timeout -- auto-stop after 500ms silence (MOTO-04 / D-03)
    if (_motorsActive && (millis() - _lastMotorCommand > MOTOR_SAFETY_TIMEOUT)) {
        emergencyStop();
    }

    // Periodic system info push (WIFI-02)
    if (millis() - _lastSystemPush >= SYSTEM_PUSH_INTERVAL) {
        pushSystemInfo();
    }
}

// D-02: typed push methods per sensor

void RobotWebUI::pushDistance(float cm) {
    if (!_transport || !_transport->wsConnected()) return;
    char buf[128];
    _protocol.buildDistance(cm, buf, sizeof(buf));
    broadcast(buf, strlen(buf));
}

void RobotWebUI::pushIR(bool detected) {
    if (!_transport || !_transport->wsConnected()) return;
    char buf[64];
    _protocol.buildIR(detected, buf, sizeof(buf));
    broadcast(buf, strlen(buf));
}

void RobotWebUI::pushOdometry(float x, float y, float heading) {
    if (!_transport || !_transport->wsConnected()) return;
    char buf[128];
    _protocol.buildOdometry(x, y, heading, buf, sizeof(buf));
    broadcast(buf, strlen(buf));
}

void RobotWebUI::pushBoolean(int index, bool state) {
    if (!_transport || !_transport->wsConnected()) return;
    char buf[64];
    _protocol.buildBoolean(index, state, buf, sizeof(buf));
    broadcast(buf, strlen(buf));
}

// D-03: per-type callbacks with typed structs

void RobotWebUI::onMotorCommand(void (*cb)(const MotorCmd&)) {
    _motorCallback = cb;
}

void RobotWebUI::onWiFiCommand(void (*cb)(const WiFiCmd&)) {
    _wifiCallback = cb;
}

// Internal handlers

void RobotWebUI::handleWSMessage(const char* data, size_t len) {
    const char* type = _protocol.parseMessageType(data, len);
    if (!type) return;  // Silently ignore invalid JSON (T-01-01)

    if (strcmp(type, MsgType::MOTOR) == 0) {
        if (_motorCallback) {
            MotorCmd cmd;
            JsonDocument& doc = _protocol.document();
            cmd.direction = doc["d"]["dir"] | "stop";
            cmd.speed = doc["d"]["spd"] | 0;
            // Validate direction -- silently ignore invalid commands
            if (cmd.direction != "forward" && cmd.direction != "back" &&
                cmd.direction != "left" && cmd.direction != "right" &&
                cmd.direction != "stop") {
                return;
            }
            _motorCallback(cmd);
            _lastMotorCommand = millis();
            _motorsActive = (cmd.direction != "stop");
        }
    } else if (strcmp(type, MsgType::WIFI_CMD) == 0) {
        if (_wifiCallback) {
            WiFiCmd cmd;
            JsonDocument& doc = _protocol.document();
            cmd.action = doc["d"]["act"] | "";
            cmd.ssid = doc["d"]["ssid"] | "";
            cmd.password = doc["d"]["pw"] | "";
            _wifiCallback(cmd);
        }
    }
    // Unknown types ignored silently
}

void RobotWebUI::handleWSConnect(bool connected) {
    Serial.println("[RobotWebUI] WebSocket client connected");
    _motorsActive = false;
}

void RobotWebUI::handleWSDisconnect(bool connected) {
    Serial.println("[RobotWebUI] WebSocket client disconnected");
    if (_motorsActive) {
        emergencyStop();
    }
}

void RobotWebUI::pushSystemInfo() {
    if (!_transport) return;

    String ip = _transport->getLocalIP();
    uint32_t uptime = millis() / 1000;
    uint32_t heapKB = ESP.getFreeHeap() / 1024;
    int32_t rssi = _transport->getRSSI();

    char buf[192];
    _protocol.buildSystemInfo(ip.c_str(), uptime, heapKB, rssi, buf, sizeof(buf));
    broadcast(buf, strlen(buf));

    _lastSystemPush = millis();
}

void RobotWebUI::broadcast(char* buf, size_t len) {
    if (_transport && _transport->wsConnected()) {
        _transport->wsBroadcast(buf, len);
    }
}

void RobotWebUI::emergencyStop() {
    if (_motorCallback) {
        MotorCmd stopCmd;
        stopCmd.direction = "stop";
        stopCmd.speed = 0;
        _motorCallback(stopCmd);
    }
    _motorsActive = false;
    Serial.println("[RobotWebUI] Motor safety timeout -- all motors stopped");
    // Notify browser of motor timeout alert (D-12)
    if (_transport && _transport->wsConnected()) {
        char buf[64];
        _protocol.buildAck("motor_timeout", buf, sizeof(buf));
        _transport->wsBroadcast(buf, strlen(buf));
    }
}
