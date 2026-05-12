# Changelog

All notable changes to openMatterSmartLock are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html) once a stable release exists.

## [Unreleased]

### Added

- Project skeleton: README, ARCHITECTURE, PORTING, LICENSE, NOTICE.
- Layered architecture with HAL interface boundaries (`actuator.h`, `feedback.h`, `power.h`, `transport_switch.h`).
- Repository structure for firmware, documentation, and example builds.


## [0.1.0] — 2026-05-13

### Added

- Layered architecture with HAL boundary (`actuator.h`, `feedback.h`, `power.h`, `transport_switch.h`).
- Matter Door Lock cluster fully wired to lock state machine.
- Servo PWM, ADC potentiometer, GPIO gate reference drivers.
- Six supported boards: nanoBoard, nanoBoard, nanoBoard, nanoBoard, nanoBoard, nanoBoard.
- Unit tests under `tests/unit/` using ztest + mock HAL.
- Documentation: README, ARCHITECTURE, PORTING, CONTRIBUTING, getting-started, security-model, power-budget.

### Build verified (nanoBoard, internal flash only)

- Sysbuild path `-DSB_CONFIG_MATTER_OTA=n` builds a flashable hex
  (~590 KB release, ~605 KB debug) without external NOR.
- Source docs (power budget, security model) filled to reference quality.
