# Tests

Unit tests run on Zephyr's `native_sim` target via ztest. Tests live
under `tests/unit/` and are intentionally hardware-free: drivers are
replaced by mock HAL implementations from `tests/unit/mocks/`.

## Run

```bash
west build -b native_sim -p auto tests/unit
./build/zephyr/zephyr.exe
```

Or via twister:

```bash
west twister -T tests/unit -p native_sim
```

## Status

The test scaffolding (testcase.yaml, mocks, ztest suites) is in place
and the test sources are organized to match the firmware module
layout. **The full test suite has not yet been verified to compile
on `native_sim`** — the device firmware was the primary build target
during initial bring-up, and tests-on-native-sim require their own
Kconfig adjustments (no Matter stack, no Thread).

First-run TODOs:

- Verify `west twister` picks up the suites under `tests/unit/`.
- Confirm `MockActuator` / `MockFeedback` provide the right
  call-tracking semantics for the state-machine tests.
- Wire a GitHub Actions job (`.github/workflows/build.yml` already
  drafts one) to run the suite on every push.

The architecture supports unit-testability — drivers behind HAL
interfaces, no globals leaking out of the lock domain — so adding
coverage as features land is the intended workflow, not a retrofit.
