# RobotWebUI

## What This Is

A framework-agnostic C++ library that serves an eye-catching robot dashboard web page from an ESP8266. It provides live sensor readings, motor control, system status, and WiFi management — all over WebSocket. Designed as a drop-in template that robotics projects can integrate by instantiating a class and calling simple API methods, without touching the frontend code.

Built for ESP8266 (Arduino framework) first, with the C++ layer designed to compile under ESP-IDF later without rewriting.

## Core Value

A reusable, modular robot web dashboard that any ESP8266/ESP32 project can integrate in minutes — live data, controls, WiFi config, all in one animated UI served from the microcontroller itself.

## Requirements

### Validated

- ✓ CORE-01: C++ class with drop-in API — v1.0
- ✓ CORE-02: ITransport interface, Arduino implementation — v1.0
- ✓ CORE-03: Pre-allocated JsonDocument, zero per-message heap — v1.0
- ✓ CORE-04: PROGMEM send_P() chunked streaming — v1.0
- ✓ COMM-01: WebSocket bidirectional communication — v1.0
- ✓ COMM-02: JSON type-field dispatch protocol — v1.0
- ✓ COMM-03: Connection status with auto-reconnect — v1.0
- ✓ MOTO-01: 2-motor speed/direction control — v1.0
- ✓ MOTO-02: Motor speed slider 0-100% — v1.0
- ✓ MOTO-03: Emergency stop button — v1.0
- ✓ MOTO-04: Motor safety timeout (500ms) — v1.0
- ✓ SENS-01: Ultrasonic distance with color thresholds — v1.0
- ✓ SENS-02: IR proximity display — v1.0
- ✓ SENS-03: Odometry x/y/heading live display — v1.0
- ✓ SENS-04: Boolean state badges — v1.0
- ✓ WIFI-01: WiFi connection with IP display — v1.0
- ✓ WIFI-02: System info (IP, uptime, heap, RSSI) — v1.0
- ✓ WIFI-03: WiFi network scanning from dashboard — v1.0
- ✓ WIFI-04: Connect to SSID with password from dashboard — v1.0
- ✓ UISC-01: Dark theme responsive card layout — v1.0
- ✓ UISC-02: Animated state changes (compositor-friendly) — v1.0
- ✓ UISC-03: Mobile-friendly 44px touch targets — v1.0
- ✓ UISC-04: Alert/notification system — v1.0

### Active

(No active requirements — ready for v2 planning)

### Out of Scope

- ESP-IDF port — v2 target, architecture supports it
- More than 4 motors in v1 — API designed for extensibility
- Authentication/login — trusted local networks only
- OTA firmware updates from dashboard — orthogonal to dashboard
- Mobile native app — web-only, responsive browser
- Data logging / historical charts — future consideration
- Camera streaming — different project scope
- Cloud/MQTT connectivity — local control for latency

## Context

- **Shipped v1.0**: 1,165 LOC C++, 17 files, 2,059 insertions
- **Tech stack**: ESP8266 Arduino Core 3.1.2, ESP32Async/ESPAsyncWebServer 3.10.3, ArduinoJson 7.4.3, vanilla HTML/CSS/JS (PROGMEM)
- **Dashboard**: 11.4KB PROGMEM for full dark-themed animated UI with motor controls, sensor displays, WiFi manager
- **Safety**: Dual-safety motor timeout (500ms silence + disconnect), EEPROM credential persistence with fallback
- **Build verification**: Post-build PROGMEM threshold check (40KB) with map/nm/readelf fallback

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
| WebSocket over REST | Real-time sensor data needs low-latency push; REST polling wastes bandwidth on ESP8266 | ✓ Good — sub-100ms round trip |
| C++ class (.h/.cpp) over Arduino library format | Must compile under both Arduino and ESP-IDF; Arduino lib format is Arduino-only | ✓ Good — ITransport abstraction clean |
| Configurable motor count with default 2 | Users have different robot types; starts with common case but extensible | ✓ Good — API ready for mecanum/tricycle |
| Dark theme animated UI | Matches robot dashboard conventions; animations make state changes visible | ✓ Good — 11.4KB PROGMEM, compositor-friendly |
| PROGMEM for HTML assets | ESP8266 has limited heap; large HTML strings must live in flash | ✓ Good — send_P() streaming, zero heap at serve time |
| Pre-allocated JsonDocument | Zero per-message heap allocation for stability | ✓ Good — 1-hour stress test passed |
| 500ms motor safety timeout | Balance responsiveness vs network jitter tolerance | ✓ Good — dual-safety with disconnect handler |
| EEPROM credential persistence | WiFi credentials survive reboot without recompile | ✓ Good — magic byte + flash wear mitigation |
| XSS-safe DOM manipulation | textContent only, no innerHTML | ✓ Good — threat model validated |
| src_filter for examples | PlatformIO requires explicit +<*> before negation | ✓ Good — minimal example uses separate .cpp |

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
*Last updated: 2026-04-12 after v1.0 milestone*
