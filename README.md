# openMatterSmartLock

A Matter Door Lock reference implementation on Zephyr and nRF Connect SDK.

## Status

- Matter Door Lock cluster: working (commission + lock/unlock from chip-tool).
- Servo PWM actuator: working on nRF52840 DK and nRF5340 DK.
- Battery sampling: heartbeat every 5 s.
- Next: users, credentials, persistence.

## Supported boards

- nRF52840 DK
- nRF5340 DK

## Build

```bash
west build -b nanoBoard -p auto firmware
west build -b nanoBoard/nrf5340_cpuapp -p auto firmware
```
