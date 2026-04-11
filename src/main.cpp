#include <RobotWebUI.h>

const char* WIFI_SSID = "YourSSID";
const char* WIFI_PASS = "YourPassword";

RobotWebUI ui;

void onMotor(const MotorCmd& cmd) {
    Serial.printf("Motor: %s @ %d%%\n", cmd.direction.c_str(), cmd.speed);
}

void setup() {
    Serial.begin(115200);
    ui.onMotorCommand(onMotor);
    ui.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    ui.loop();
}
