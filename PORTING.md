# Porting Guide

This guide explains how to extend openMatterSmartLock to new hardware: adding board support, integrating a new actuator, or implementing a new feedback sensor. All extensions follow the same principle — domain code never depends directly on hardware, so adding new hardware is local to the board layer and driver layer only.

## 1. Adding a new board

A "board" in openMatterSmartLock terms is any combination of SoC + reference development kit (or custom PCB) that has a corresponding **Zephyr board definition**.

### 1.1. Prerequisites

- The board must have a Zephyr board definition (either upstream in Zephyr / nRF Connect SDK, or shipped under `zephyr/boards/` in the consumer of this firmware).
- The SoC must support 802.15.4 (Thread) or Wi-Fi if you intend to commission over Matter.
- A PWM peripheral and an ADC peripheral are recommended if you want to use the default servo + ADC drivers.

### 1.2. Steps

1. **Create a board overlay** at `firmware/boards/<board_name>.overlay`. Bind the following devicetree nodes:
  - `servo_pwm` → a PWM channel and pin assignment for the actuator.
   - `feedback_adc` → an ADC channel for position / battery feedback (optional, but recommended).
   - `power_gate` → a GPIO that controls auxiliary power rails (optional).
   - Standard Zephyr aliases (`led0`, `sw0`) for board UX.

2. **Add a board Kconfig fragment** at `firmware/boards/<board_name>.conf` selecting:
   - the actuator driver (`CONFIG_OMSL_ACTUATOR_SERVO_PWM=y` or another),
   - the feedback driver (`CONFIG_OMSL_FEEDBACK_ADC_POTENTIOMETER=y` or none),
   - the power driver (`CONFIG_OMSL_POWER_GPIO_GATE=y` or none),
   - any board-specific Zephyr options (BLE controller selection, Thread role, network core image selection on multi-core SoCs).

3. **Add a partition manager configuration** at `firmware/pm_static_<board_name>.yml` if your board requires fixed partitioning (most production targets do). Mirror the partitioning style of existing boards.

4. **Verify the build**:

   ```bash
   west build -b <board_name> -p auto firmware
   ```

5. **Register the board in CI**: add the board to the build matrix in `firmware/sample.yaml`.

## 2. Adding a new actuator driver

The actuator HAL (`firmware/app/hal/actuator.h`) defines the contract every actuator must implement. The default driver is `servo_pwm`; you can replace or add to it.

### 2.1. Driver contract

A driver implementing `actuator.h` must:

- initialize the underlying hardware (via Zephyr device APIs and devicetree references),
- expose a `MoveTo(target_state)` entry point that starts asynchronous motion,
- expose a `Stop()` entry point,
- invoke a registered completion callback with either `Success` or `Stall/Timeout` outcome,
- respect a configurable maximum actuation time and stop on timeout.

### 2.2. Steps

1. Create a new folder under `firmware/drivers/actuator/<driver_name>/`.
2. Add a `CMakeLists.txt`, a `Kconfig`, and the driver source files.
3. Define a Kconfig option `OMSL_ACTUATOR_<NAME>`, mutually exclusive with other actuator drivers.
4. Define any custom devicetree binding under `firmware/dts/bindings/<binding_name>.yaml` if your driver needs a non-standard node type.
5. Implement the driver against `actuator.h`.
6. Wire the driver selection in `firmware/drivers/actuator/CMakeLists.txt` so the right source files are linked when the corresponding Kconfig option is enabled.

### 2.3. Driver examples

- **Servo PWM** (`servo_pwm/`): the reference driver. PWM channel + optional `enable-gpios` for servo power gating.
- **DC motor + H-bridge** (planned): two GPIO direction pins + one PWM pin for speed, plus an end-of-travel detection strategy (current sensing or stall detection via feedback HAL).
- **Latching solenoid** (planned): two short pulses for lock / unlock; no continuous power required.
- **Stepper motor** (planned): step + direction + enable.

## 3. Adding a new feedback driver

Same pattern as actuator drivers. Drivers under `firmware/drivers/feedback/<name>/`, Kconfig option `OMSL_FEEDBACK_<NAME>`, implementing `firmware/app/hal/feedback.h`.

Typical implementations:

- **ADC potentiometer** (reference): potentiometer on the actuator output shaft, sampled via ADC.
- **Hall encoder** (planned): Hall sensor + magnet for absolute position.
- **Magnetic encoder** (planned): I²C / SPI magnetic encoder for precise position feedback.

## 4. Building downstream products on top of openMatterSmartLock

openMatterSmartLock is intentionally a clean foundation. Downstream projects that want to add product-specific configuration (manufacturer-specific clusters, custom calibration profiles, OTA signing, factory provisioning) should:

- consume openMatterSmartLock as a **west dependency**, not by copying files,
- add their own layers on top in a separate repository,
- keep upstream contributions cleanly separated from downstream IP.

This keeps the open foundation healthy and makes upstream merges trivial.
