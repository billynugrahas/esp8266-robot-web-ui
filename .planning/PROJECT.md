# RobotWebUI

## What This Is

A framework-agnostic C++ library that serves an eye-catching robot dashboard web page from an ESP8266. It provides live sensor readings, motor control, system status, and WiFi management — all over WebSocket. Designed as a drop-in template that robotics projects can integrate by instantiating a class and calling simple API methods, without touching the frontend code.

Built for ESP8266 (Arduino framework) first, with the C++ layer designed to compile under ESP-IDF later without rewriting.

## Core Value

A reusable, modular robot web dashboard that any ESP8266/ESP32 project can integrate in minutes — live data, controls, WiFi config, all in one animated UI served from the microcontroller itself.

## Requirements

### Validated

(None yet — ship to validate)

### Active

- [ ] WebSocket-based live communication between browser and ESP8266
- [ ] Live sensor readings display: ultrasonic distance, IR proximity, odometry (x, y, angle)
- [ ] Boolean state display: limit switches with on/off visual state
- [ ] Motor speed and direction control from the web UI
- [ ] Configurable motor count — default 2 (differential drive), extensible to 4 (mecanum) or 1+steering (tricycle)
- [ ] Light/auxiliary control toggles
- [ ] WiFi manager: scan networks, connect to SSID, configure credentials from the page
- [ ] WiFi manager: enable/disable web server, config which functions are active
- [ ] System info display: IP address, uptime, free heap, WiFi signal strength
- [ ] Eye-catching animated UI: color-coded buttons, size hierarchy, state-change animations
- [ ] Framework-agnostic C++ class (.h/.cpp) — works under Arduino and ESP-IDF
- [ ] Simple integration API: instantiate class, call begin(), push sensor data, receive motor commands via callbacks

### Out of Scope

- ESP-IDF port (v2 — architecture will support it, but Arduino first)
- More than 4 motors in v1 (API designed for extensibility)
- Authentication/login on the web interface
- OTA firmware updates from the dashboard
- Mobile native app (web-only, responsive mobile browser)
- Data logging / historical charts (future consideration)
- Camera streaming (different project scope)

## Context

- **Existing code**: Project has a PlatformIO ESP8266 setup (`esp12e` board, `espressif8266` platform). A WiFi scanner sketch exists in `src/main.cpp`. An example ESP32 web server UI exists in `esp32_webserver_ui.ino` — this serves as visual reference for the dashboard style (dark theme, card layout, colored buttons, status badges).
- **Target hardware**: ESP8266 (ESP-12E) — limited heap (~50KB available), PROGMEM needed for large HTML assets.
- **Future hardware**: ESP32 under ESP-IDF — the C++ class layer must not depend on Arduino-specific APIs directly. Abstract the HTTP server and WebSocket behind a thin interface.
- **Communication pattern**: WebSocket for all data — sensor readings pushed from ESP to browser, control commands sent from browser to ESP. No REST polling.
- **UI reference**: The `.ino` file shows the desired visual direction — dark theme (`#0f172a` bg), card-based layout, colored variant buttons, status badges with dots, sectioned info rows, alert notifications with fade animations.
- **Robot types**: Initially differential drive (2 motors). Library API designed to support mecanum (4 motors), tricycle (1 motor + steering angle), and arbitrary configurations in the future.

## Constraints

- **Memory**: ESP8266 has ~80KB total heap; HTML/CSS/JS must use PROGMEM and be streamed, not held in RAM
- **Framework**: Arduino framework for v1, but C++ class must not tightly couple to Arduino APIs (abstract server/WebSocket behind interface)
- **Board**: ESP-12E (ESP8266) — 4MB flash, 80MHz CPU
- **No external frontend build**: All HTML/CSS/JS is embedded in C++ source as PROGMEM strings — no npm, no bundler, no build step
- **Single-page**: One HTML page served at root, WebSocket connection for all data
- **Browser compatibility**: Must work on Chrome/Firefox/Safari on mobile and desktop

## Key Decisions

| Decision | Rationale | Outcome |
|----------|-----------|---------|
| WebSocket over REST | Real-time sensor data needs low-latency push; REST polling wastes bandwidth on ESP8266 | — Pending |
| C++ class (.h/.cpp) over Arduino library format | Must compile under both Arduino and ESP-IDF; Arduino lib format is Arduino-only | — Pending |
| Configurable motor count with default 2 | Users have different robot types; starts with common case but extensible | — Pending |
| Dark theme animated UI | Matches existing `.ino` reference style; dark theme is common for dashboards; animations make state changes visible | — Pending |
| PROGMEM for HTML assets | ESP8266 has limited heap; large HTML strings must live in flash | — Pending |

## Evolution

This document evolves at phase transitions and milestone boundaries.

**After each phase transition** (via `/gsd-transition`):
1. Requirements invalidated? → Move to Out of Scope with reason
2. Requirements validated? → Move to Validated with phase reference
3. New requirements emerged? → Add to Active
4. Decisions to log? → Add to Key Decisions
5. "What This Is" still accurate? → Update if drifted

**After each milestone** (via `/gsd-complete-milestone`):
1. Full review of all sections
2. Core Value check — still the right priority?
3. Audit Out of Scope — reasons still valid?
4. Update Context with current state

---
*Last updated: 2026-04-11 after initialization*
