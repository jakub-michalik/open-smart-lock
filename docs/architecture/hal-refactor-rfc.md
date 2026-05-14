# RFC: HAL boundary for actuator and feedback

## Problem

`Bolt` currently includes `servo_controller.h` and calls
`ServoController` methods directly. This makes:

- headless / native_sim builds impossible (every build pulls in PWM),
- swapping the actuator (DC motor, latching solenoid) require domain-
  layer changes,
- unit testing the state machine require linking real hardware code.

## Proposal

Introduce a HAL boundary at `firmware/app/hal/` with pure C++
interface headers. Domain code depends only on these interfaces.
Drivers move to `firmware/drivers/<category>/<name>/` and implement
the interfaces. Driver selection is controlled by Kconfig.

## Migration plan

1. Add `hal/actuator.h`, `hal/feedback.h`, `hal/power.h` headers.
2. Move `servo_controller.*` to `drivers/actuator/servo_pwm/`, make it
   implement `IActuator`.
3. Move `adc_manager.*` to `drivers/feedback/adc_potentiometer/`,
   implement `IFeedback`.
4. Extract servo power gating into its own `drivers/power/gpio_gate/`.
5. Refactor `Bolt`, `Runtime` to use only HAL pointers.
6. Add Kconfig options to select drivers per board.
