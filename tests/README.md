# Tests

Unit tests run on Zephyr's `native_sim` target via ztest. Tests live
under `tests/unit/` and are intentionally hardware-free: drivers are
replaced by mock HAL implementations from `tests/unit/mocks/`.

Run:

```bash
west build -b native_sim -p auto tests/unit
./build/zephyr/zephyr.exe
```

Or via twister:

```bash
west twister -T tests/unit -p native_sim
```
