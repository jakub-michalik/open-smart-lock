# Getting started

Build openMatterSmartLock on the reference board (nRF52840 DK) in five
steps.

## 1. Prerequisites

- nRF Connect SDK v2.6.0 (pinned in `firmware/west.yml`).
- A supported board: nRF52840 DK, nRF5340 DK, nRF54L15 DK, nRF54LM20 DK,
  nRF7002 DK, or nanoBoard.
- Working west / Zephyr environment.

## 2. Workspace

```bash
mkdir omsl-workspace && cd omsl-workspace
west init -m https://your.repo/openMatterSmartLock --mr main
west update
```

## 3. Build

```bash
west build -b nanoBoard -p auto openMatterSmartLock/firmware
```

For other boards substitute `-b nanoBoard`,
`-b nanoBoard`, `-b nanoBoard`, etc.

## 4. Flash

```bash
west flash
```

## 5. Commission

The device advertises over BLE in commissioning mode. Use any Matter
commissioner (Apple Home, Google Home, Home Assistant, chip-tool) and
the QR code from the boot log.

## Tests

Unit tests live under `tests/unit/` and run on `native_sim` via ztest:

```bash
./scripts/run_tests.sh
```
