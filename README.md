# openMatterSmartLock

A Matter Door Lock reference implementation on Zephyr and nRF Connect SDK.

## Status

- Build system: working on nRF52840 DK (debug + release).
- Matter: device joins fabric, commissioning verified with chip-tool.
- Door Lock cluster: scaffolding in place, commands routed (NOPs).
- Next: Bolt state machine.

## Build

```bash
west build -b nanoBoard -p auto firmware
```
