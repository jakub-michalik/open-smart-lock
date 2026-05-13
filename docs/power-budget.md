# Power budget (placeholder)

Power consumption analysis and battery-life model for openMatterSmartLock.

## Planned content

- Idle (sleepy end device) average current target
- Per-actuation energy cost (servo wake + move + sleep)
- Heartbeat overhead at 5 s interval
- Battery chemistry options and de-rating
- Multi-year operation arithmetic with realistic duty cycles

The implementation work to support these numbers (servo power gating,
heartbeat-aligned radio wake-up, sleepy-end-device Thread role) is
present in the codebase; this document captures the analysis and
measurement methodology.
