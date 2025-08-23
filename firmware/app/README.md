# firmware/app — Application layer

Layered Matter Door Lock application.

| Subdir | Responsibility |
|---|---|
| `matter/` | ZCL Door Lock cluster glue |
| `lock/` | Lock state machine |
| `access/` | Users, credentials, schedules + persistence |
| `ui/` | LED + button input |
| `zap/` | ZAP cluster definition and generated code |
