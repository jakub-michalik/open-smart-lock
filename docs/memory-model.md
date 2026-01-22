# Memory model

This document captures the memory ownership, allocation, and budgeting
rules that openMatterSmartLock follows. It is reference-quality, not
exhaustive: edge cases live in code comments next to the relevant
allocations.

## 1. Principles

1. **Static allocation by default.** Domain-layer modules
   (`Bolt`, `Roster`, drivers) are singletons in
   static storage. Their lifetime is the program lifetime; no
   destruction at shutdown.
2. **No heap from domain code.** Application and lock-domain code must
   not call `new`, `malloc`, `k_heap_alloc`, or `chip::Platform::MemoryAlloc`
   directly. All dynamic memory belongs to the Matter stack or to Zephyr
   net buffers.
3. **Heap budget is explicit.** `CONFIG_HEAP_MEM_POOL_SIZE` is sized
   against the worst-case Matter commissioning + OTA download path. The
   heartbeat task logs heap pressure in debug builds.
4. **Stacks are sized per-thread, not blanket-large.** Each Zephyr /
   Matter thread has its own Kconfig stack size, tuned against
   measured high-water marks rather than guessed.
5. **No exceptions, no RTTI.** `CONFIG_CPP_EXCEPTIONS=n` and
   `CONFIG_CPP_RTTI=n`. Code that wants `throw` should use a
   `chip::Optional` or a result enum instead.

## 2. Where memory comes from

| Region | Owner | Size knob |
|---|---|---|
| `.bss` / static storage | Module singletons, callback storage | Determined at link time |
| Main thread stack | `main()`, then idles | `CONFIG_MAIN_STACK_SIZE` |
| CHIP / Matter task stack | Matter event loop | `CONFIG_CHIP_TASK_STACK_SIZE` |
| System work queue stack | `k_work_delayable` items (heartbeat) | `CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE` |
| BT RX / TX stacks | BLE controller | `CONFIG_BT_RX_STACK_SIZE`, `CONFIG_BT_HCI_TX_STACK_SIZE` |
| Heap pool | `malloc`, CHIP `MemoryAlloc` | `CONFIG_HEAP_MEM_POOL_SIZE` |
| Net buffer pool | Thread / BLE / BDX OTA transfers | `CONFIG_NET_BUF_*`, `CONFIG_NET_PKT_*` |
| Settings storage | `Vault` snapshots | `pm_static_<board>.yml` settings partition |

## 3. Lifetime conventions

- **Module singletons** (`Bolt::Instance()`, `Roster::Instance()`)
  use the Meyers-singleton pattern with function-local statics. Their
  storage is `.bss`; their initialization is on first use.
- **HAL drivers** (`ServoPwmActuator`, `AdcFeedback`, `GpioGatePower`)
  follow the same pattern. `Runtime` holds a non-owning pointer to
  whichever driver is selected by Kconfig.
- **Callbacks** are passed as `std::function`. The application uses
  small-buffer-optimized lambdas (no captures, or only captures by
  pointer to long-lived statics) so no heap allocation happens for
  callback storage in practice. See `Bolt::Lock()` for the
  canonical pattern.
- **Access data** (users, credentials, schedules) live in RAM during
  runtime, owned by `Roster`. Persistence is snapshot-based via
  `Vault`, not write-through, to keep flash wear predictable
  and avoid mid-mutation crashes from yielding partial state.

## 4. What the heap is used for

- Matter / CHIP internal allocations (cluster object tracking,
  network buffer descriptors, exchange manager state).
- BLE Host stack working buffers.
- Thread network message reassembly.

What it is **not** used for:

- Application data structures
- HAL driver state
- Lock state machine
- Access records

If a future feature needs dynamic allocation, it should:

1. Justify it in code review.
2. Use a dedicated `k_heap` instance with a bounded size, not the
   global heap pool.
3. Document the allocation in this file.

## 5. Instrumentation

Debug builds enable:

- `CONFIG_THREAD_ANALYZER` — periodic high-water-mark log per thread.
- `CONFIG_INIT_STACKS` — stack canaries on every thread, MPU-enforced.
- `CONFIG_THREAD_STACK_INFO` — `kernel stacks` shell command available
  for live inspection.
- `OMSL_LOG_HEARTBEAT_STACKS=y` — heartbeat task prints stack usage
  on every tick.

Release builds strip all of the above for code size and runtime cost.

## 6. Verification

The "memory budget verified" claim in CHANGELOG entries requires:

1. Build with `prj.conf` (debug) on the target board.
2. Run the worst-case scenario: BLE commission + lock/unlock cycle +
   OTA staging + 24 h of heartbeat ticks.
3. Capture thread analyzer output, verify peak high-water mark is
   below 70% of configured stack size per thread.
4. Capture heap pressure via `sys_heap_runtime_stats_get()` shell
   helper; peak heap usage must be below 80% of pool size.

Numbers per board live in `docs/measurements/` (added incrementally as
each board is verified).
