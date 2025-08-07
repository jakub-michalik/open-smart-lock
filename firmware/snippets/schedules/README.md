# schedules snippet

Optional Door Lock schedules surface. Selects the `LOCK_SCHEDULES`
Kconfig option and includes `app/access/schedules.cpp`
in the build.

```bash
west build -b nanoBoard -p auto firmware --snippet schedules
```

A separate ZAP definition is required for the schedules cluster
surface; consumers enabling this snippet are expected to provide
their own `lock.zap` with the schedules cluster declared.
