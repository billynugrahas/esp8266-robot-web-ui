# Roadmap: RobotWebUI

## Overview

Build a reusable C++ library that serves an animated robot dashboard from an ESP8266. The journey starts with the async transport foundation and WebSocket communication, delivers the full robot dashboard with motor control and sensor displays, then adds the WiFi manager subsystem as a separate concern. Each phase produces a coherent, testable capability on real hardware.

## Phases

**Phase Numbering:**
- Integer phases (1, 2, 3): Planned milestone work
- Decimal phases (2.1, 2.2): Urgent insertions (marked with INSERTED)

Decimal phases appear between their surrounding integers in numeric order.

- [x] **Phase 1: Transport Foundation** - Async HTTP/WebSocket server, ITransport abstraction, C++ class API, PROGMEM page serving, JSON protocol, connection status
- [ ] **Phase 2: Robot Dashboard** - Motor control with safety timeout, sensor displays, dark theme animated UI, mobile layout, alert system
- [x] **Phase 3: WiFi Manager** - Network scanning, SSID selection, password entry, connection management from the dashboard (completed 2026-04-12)
- [ ] **Phase 4: Polish & Integration** - Example sketches, API documentation, PROGMEM budget validation, heap stability verification

## Phase Details

### Phase 1: Transport Foundation
**Goal**: A developer can instantiate the RobotWebUI class, call begin(), and see a live WebSocket dashboard page served from the ESP8266 with system info updating in real time
**Depends on**: Nothing (first phase)
**Requirements**: CORE-01, CORE-02, CORE-03, CORE-04, COMM-01, COMM-02, COMM-03, WIFI-01, WIFI-02
**Success Criteria** (what must be TRUE):
  1. Developer integrates the library in under 10 lines of setup code and the ESP8266 serves a web page at its IP address
  2. A browser connecting to the ESP8266 sees a WebSocket-connected status indicator that turns red on disconnect and green on reconnect
  3. System info (IP address, uptime counter, free heap in KB, WiFi RSSI) updates live on the dashboard via WebSocket push
  4. The HTML page is served from PROGMEM with zero heap allocation at serve time (verified via heap logging)
  5. The pre-allocated JsonDocument handles all serialization without per-message heap allocation over a 1-hour stress test
**Plans**: 2 plans

Plans:
- [x] 01-01-PLAN.md -- Core transport library: ITransport interface, RobotWebUI class, ArduinoTransport implementation, WSProtocol JSON serialization
- [x] 01-02-PLAN.md -- Dashboard page + integration: PROGMEM HTML/CSS/JS, platformio.ini deps, example sketch, compile verification

### Phase 2: Robot Dashboard
**Goal**: Users can control robot motors and see live sensor data from a mobile-friendly dark-themed animated dashboard
**Depends on**: Phase 1
**Requirements**: MOTO-01, MOTO-02, MOTO-03, MOTO-04, SENS-01, SENS-02, SENS-03, SENS-04, UISC-01, UISC-02, UISC-03, UISC-04
**Success Criteria** (what must be TRUE):
  1. User can control 2 motors (forward/back/left/right/stop) with speed slider (0-100%) from a mobile phone browser and motors respond in real time
  2. All motors stop within 500ms when the WebSocket disconnects (phone sleeps, browser backgrounds, WiFi drops)
  3. Ultrasonic distance displays with color-coded thresholds (green >30cm, yellow 15-30cm, red <15cm), IR proximity shows detected/clear state, odometry shows x/y/heading updating live
  4. Limit switch states display as color-coded on/off badges with animated state transitions
  5. Dashboard shows an emergency stop button prominently, and touch targets are at least 44px on mobile with a single-column responsive layout
**Plans**: 3 plans

Plans:
- [x] 02-01-PLAN.md -- Motor safety timeout: dual-safety auto-stop (millis timeout + disconnect handler) in RobotWebUI C++ backend
- [x] 02-02-PLAN.md -- Dashboard UI expansion: motor controls (D-pad, E-stop, speed slider), sensor displays (distance, IR, odometry, limit switches), alert enhancements in pages.h
- [x] 02-03-PLAN.md -- Example sketch update: sensor simulation demo, motor command callback, PROGMEM budget verification, build integration test
**UI hint**: yes

### Phase 3: WiFi Manager
**Goal**: Users can scan for WiFi networks, select one, enter a password, and connect from the dashboard without touching the ESP8266 code
**Depends on**: Phase 1
**Requirements**: WIFI-03, WIFI-04
**Success Criteria** (what must be TRUE):
  1. User clicks a "Scan Networks" button on the dashboard and sees a list of available WiFi networks with signal strength within 5 seconds
  2. User selects an SSID from the scan results, enters a password, and the ESP8266 connects to that network with IP address updating on the dashboard
**Plans**: 2 plans

Plans:
- [x] 03-01-PLAN.md -- WiFi backend: scan, connect, EEPROM persistence in WSProtocol and RobotWebUI C++ classes
- [x] 03-02-PLAN.md -- WiFi UI: scan button, network list with signal bars, password entry, connect flow in pages.h
**UI hint**: yes

### Phase 4: Polish & Integration
**Goal**: Any robotics project can integrate RobotWebUI by reading clear documentation and running verified example sketches
**Depends on**: Phase 2, Phase 3
**Requirements**: (No new requirements -- this phase validates and documents the integration of all prior requirements)
**Success Criteria** (what must be TRUE):
  1. An example sketch compiles, uploads, and serves the full dashboard with motor control and sensor data on first try
  2. PROGMEM budget is validated -- total HTML/CSS/JS stays within the 45KB flash budget with headroom
  3. API documentation covers all public methods with usage examples
**Plans**: 3 plans

Plans:
- [x] 04-01-PLAN.md -- Demo sketch polish: heap logging, Serial prefixes, minimal example env, PROGMEM size comments
- [x] 04-02-PLAN.md -- API documentation: README.md with quickstart, full API reference, JSON protocol, PROGMEM budget, troubleshooting
- [x] 04-03-PLAN.md -- Build verification: PROGMEM threshold post-build script in platformio.ini

## Progress

**Execution Order:**
Phases execute in numeric order: 1 -> 2 -> 3 -> 4

| Phase | Plans Complete | Status | Completed |
|-------|----------------|--------|-----------|
| 1. Transport Foundation | 2/2 | Complete | 2026-04-11 |
| 2. Robot Dashboard | 0/3 | Planned | - |
| 3. WiFi Manager | 2/2 | Complete    | 2026-04-12 |
| 4. Polish & Integration | 2/3 | In Progress | - |
