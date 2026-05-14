# Getting started

Build openMatterSmartLock on the reference nanoBoard hardware in five
steps.

## 1. Prerequisites

- nRF Connect SDK v2.6.0 (pinned in `firmware/west.yml`).
- nanoBoard hardware (nRF52840-based module).
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


## Troubleshooting

- **PWM device not ready:** verify the board overlay binds `servo_pwm`
  to a real `pwm0` (or `pwm20` on newer SoCs) and that pinctrl is set.
- **Commissioning fails over BLE:** confirm `CONFIG_BT=y` and that no
  other BLE central is paired with the device.
- **Build fails on missing CHIP module:** run `west update` after
  cloning to fetch the CHIP module per the manifest.
