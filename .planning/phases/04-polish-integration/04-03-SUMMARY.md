---
phase: 04-polish-integration
plan: 03
subsystem: infra
tags: [platformio, build-verification, progmem, esp8266, python]

# Dependency graph
requires:
  - phase: 04-polish-integration
    provides: "pages.h with INDEX_HTML PROGMEM (~16KB), platformio.ini build config"
provides:
  - "Post-build PROGMEM budget check script (scripts/check_progspace.py)"
  - "Automatic build failure if INDEX_HTML exceeds 40KB threshold"
  - "Linker map file generation for reliable size extraction"
affects: [build-system, ci, future-html-changes]

# Tech tracking
tech-stack:
  added: []
  patterns: [platformio-extra-script, post-build-validation, multi-strategy-size-detection]

key-files:
  created:
    - scripts/check_progspace.py
  modified:
    - platformio.ini

key-decisions:
  - "Three-strategy size detection: map file primary, nm fallback, readelf fallback"
  - "Warn but do not fail if size cannot be determined (avoids false-negative build breakage)"
  - "40KB threshold chosen per D-10, leaving 5KB buffer below 45KB flash budget target"

patterns-established:
  - "Post-build validation via PlatformIO extra_scripts with post: prefix"
  - "Multi-strategy build artifact inspection with graceful degradation"

requirements-completed: []

# Metrics
duration: 3min
completed: 2026-04-12
---

# Phase 04 Plan 03: PROGMEM Build Verification Summary

**PlatformIO post-build script validates INDEX_HTML PROGMEM size against 40KB threshold with map file / nm / readelf fallback detection**

## Performance

- **Duration:** 3 min
- **Started:** 2026-04-12T04:53:29Z
- **Completed:** 2026-04-12T04:56:39Z
- **Tasks:** 1
- **Files modified:** 2

## Accomplishments
- Created `scripts/check_progspace.py` with three-strategy size detection (map file, nm, readelf)
- Wired script into `platformio.ini` via `extra_scripts = post:scripts/check_progspace.py`
- Added linker map file generation via `build_flags = -Wl,-Map,$BUILD_DIR/${PROGNAME}.map`
- Verified build passes: INDEX_HTML at 15,998 bytes (15.6 KB) with 61% headroom remaining
- Script gracefully degrades -- warns but does not fail if size cannot be determined

## Task Commits

Each task was committed atomically:

1. **Task 1: Create PROGMEM build verification script and wire to platformio.ini** - `cac21ba` (feat)

## Files Created/Modified
- `scripts/check_progspace.py` - PlatformIO post-build script: checks INDEX_HTML PROGMEM size against 40KB budget using map file, nm, or readelf fallback strategies
- `platformio.ini` - Added `extra_scripts` reference to check_progspace.py and `build_flags` for linker map file generation in [env:esp12e]

## Decisions Made
- Three-strategy size detection for reliability across different build environments (map file > nm > readelf)
- Graceful failure: if no strategy can determine size, warn but do not fail the build (avoids false negatives from missing tools)
- 40KB threshold per D-10, with 5KB buffer below the 45KB flash budget target documented in pages.h

## Deviations from Plan

None - plan executed exactly as written.

## Issues Encountered

None.

## User Setup Required

None - no external service configuration required.

## Next Phase Readiness
- All Phase 04 plans complete (3/3)
- Build system now has automatic PROGMEM regression protection
- Combined with boot-time size logging (Plan 01) and pages.h budget comments (Plan 01), PROGMEM budget is fully monitored at both build time and runtime

## Self-Check: PASSED

- scripts/check_progspace.py: FOUND
- Commit cac21ba: FOUND
- PROGMEM_BUDGET_THRESHOLD = 40960: FOUND
- env.AddPostAction: FOUND
- Import("env"): FOUND
- extra_scripts in platformio.ini: FOUND
- build_flags with map file in platformio.ini: FOUND

---
*Phase: 04-polish-integration*
*Completed: 2026-04-12*
