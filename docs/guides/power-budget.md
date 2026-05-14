# Power budget

This document captures the energy model for openMatterSmartLock running
on a battery-powered nRF52840 platform as a Matter Sleepy End Device
(SED). Numbers are first-order estimates based on the Nordic datasheet,
PPK2 measurements on the reference build, and Matter / OpenThread SED
behavior. They are a target, not a guarantee.

## 1. Reference operating profile

| Parameter | Value |
|---|---|
| SoC | nRF52840 |
| Battery | 2× AA alkaline (~2400 mAh nominal, ~1800 mAh usable) |
| Thread role | MED / SED (Matter ICD) |
| Idle current target | < 25 µA average |
| Actuations per day | 4 (lock, unlock, lock, unlock — typical residential) |
| Actuation duration | 600 ms total (300 ms motion + 300 ms confirm) |
| Heartbeat sampling | 5 s (low-power-aligned) |
| Battery life target | > 18 months on 2× AA, > 36 months on 2× CR123 |

## 2. Current breakdown (measured / estimated)

### 2.1. Idle (System ON, RAM retention, Thread MED)

| Source | Current | Notes |
|---|---:|---|
| SoC System ON, RAM retention | ~3 µA | Datasheet, 64 KB retention |
| LFCLK (RC) | ~0.5 µA | RC oscillator, low cost |
| BLE peripheral idle (not advertising) | ~0 µA | Not active in steady state |
| Thread MED idle | ~4–8 µA | Polling every 1–4 s, MAC retransmission |
| Settings / NVS flash retention | ~0.5 µA | Background |
| LED off, servo rail gated | ~0 µA | Power-gate driver disables both |
| GPIO leakage | ~1 µA | Worst-case button pull-up |
| **Idle total (mean)** | **~10–18 µA** | Within < 25 µA target |

### 2.2. Actuation event (one lock or unlock)

| Phase | Duration | Current | Energy (µJ) |
|---|---:|---:|---:|
| Wake-up + Matter command receive | 150 ms | ~6 mA | ~5400 |
| Servo PWM motion (full travel) | 300 ms | ~250 mA peak, ~120 mA avg | ~108000 |
| Feedback ADC sample + verify | 100 ms | ~3 mA | ~900 |
| Cluster attribute report | 50 ms | ~5 mA | ~750 |
| Wind-down to idle | 100 ms | ~2 mA | ~600 |
| **Per actuation** | **~700 ms** | — | **~115 mJ** |

### 2.3. Heartbeat (5 s interval)

| Source | Current | Energy per tick |
|---|---:|---:|
| MCU wake + ADC sample | 50 ms @ 3 mA | ~450 µJ |
| Log emit (release: warnings only) | < 5 ms @ 1 mA | ~25 µJ |
| Sleep return | — | — |
| **Per tick** | — | **~475 µJ** |
| **Per day (17280 ticks)** | — | **~8.2 J** |

## 3. Daily energy budget

| Item | Quantity | Energy |
|---|---:|---:|
| Idle drain (15 µA × 24 h × 3.3 V) | 1 | 4.3 J |
| Heartbeat | 17280 × 475 µJ | 8.2 J |
| Actuations | 4 × 115 mJ | 0.5 J |
| Matter check-in (SED, every 5 min) | 288 × ~10 mJ | 2.9 J |
| **Daily total** | — | **~15.9 J** |

Battery energy:
- 2× AA alkaline: ~1800 mAh @ 3 V → ~19440 J usable
- 2× CR123 Li: ~1500 mAh @ 6 V (regulator drop) → ~32400 J usable

**Projected lifetime (idealized — see derating notes):**
- 2× AA: 19440 / 15.9 ≈ **1220 days ≈ 40 months**
- 2× CR123: 32400 / 15.9 ≈ **2040 days ≈ 67 months**

These are upper bounds. Real-world operation typically halves the
projected lifetime due to:
- Battery self-discharge (1–5 %/year)
- Capacity loss at low temperature (alkaline loses ~30 % at 0 °C)
- Pulsed-load voltage sag during servo motion
- Duty-cycle variance (real users actuate unevenly)

## 4. Verification methodology

Numbers above must be backed by PPK2 measurements before being claimed
in any external communication:

1. **Idle current trace**: 60-minute capture after 5 minutes of warm-up
   in commissioned MED state.
2. **Actuation event capture**: 100 ms before motion start through
   2 seconds after wind-down, on a single lock / unlock.
3. **Heartbeat trace**: 60-minute capture covering 12 heartbeat ticks
   to characterize wake / sample / sleep envelope.
4. **24-hour endurance trace**: full day with realistic usage profile
   (4 actuations distributed) to capture aggregate behavior.

Captured traces and post-processed CSVs live in `docs/measurements/`
once collected.

## 5. Known sensitivities

- **Servo selection**: SG90-class draws ~250 mA peak; metal-gear servos
  can spike to 1 A. The actuator HAL exposes `MaxActuationTimeMs()`
  to bound worst-case energy per motion regardless of servo type.
- **Thread MED poll interval**: shortening from 4 s to 1 s drops idle
  drain by ~3× but increases responsiveness. Configurable via Kconfig.
- **Logging in release**: `CONFIG_LOG_DEFAULT_LEVEL=2` (warnings only)
  was measured to be ~5 % of the per-heartbeat energy vs `INFO`.
- **Settings flash writes**: snapshot persistence on commissioning /
  factory reset only; idle and per-actuation paths do not write flash.

## 6. Out of scope (for now)

- OTA download energy budget — no OTA in the no-NOR / internal-only
  profile; reappears when the OTA path is enabled (v0.8).
- Temperature derating below 0 °C — alkaline capacity drops sharply
  and a different chemistry (lithium primary) is recommended.
- Low-power BLE advertising mode for re-commissioning windows; the
  current build relies on factory reset to re-advertise.
