#include <RobotWebUI.h>

const char* WIFI_SSID = "YourSSID";
const char* WIFI_PASS = "YourPassword";

RobotWebUI ui;

// Phase 2: Simulated sensor state for demonstration
unsigned long lastSensorPush = 0;
const unsigned long SENSOR_PUSH_INTERVAL = 1000; // 1 second
float simDist = 50.0;
float simX = 0.0, simY = 0.0, simHeading = 0.0;
bool simIR = false;
bool simSwitches[4] = {true, false, true, false};
int simSwitchToggle = 0;

void onMotor(const MotorCmd& cmd) {
    Serial.printf("[Motor] dir=%s spd=%d%%\n", cmd.direction.c_str(), cmd.speed);
}

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("\n[RobotWebUI] Phase 2 Demo Starting...");

    ui.onMotorCommand(onMotor);
    ui.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    ui.loop();

    // Simulate sensor data push every second
    if (millis() - lastSensorPush >= SENSOR_PUSH_INTERVAL) {
        lastSensorPush = millis();

        // Distance: oscillate between 5cm and 60cm
        simDist = 32.5 + 27.5 * sin(millis() / 3000.0);
        ui.pushDistance(simDist);

        // IR: toggle every 5 seconds
        if ((millis() / 5000) % 2 == 0) {
            if (!simIR) { simIR = true; ui.pushIR(simIR); }
        } else {
            if (simIR) { simIR = false; ui.pushIR(simIR); }
        }

        // Odometry: slow drift
        simX += 0.1;
        simY += 0.05;
        simHeading += 1.5;
        if (simHeading >= 360.0) simHeading -= 360.0;
        ui.pushOdometry(simX, simY, simHeading);

        // Limit switches: toggle one switch every 3 seconds
        simSwitchToggle = (simSwitchToggle + 1) % 4;
        simSwitches[simSwitchToggle] = !simSwitches[simSwitchToggle];
        ui.pushBoolean(simSwitchToggle, simSwitches[simSwitchToggle]);
    }
}
