# ZAP cluster configuration

Door Lock cluster definition and generated source for openMatterSmartLock.

The `.zap` file describes endpoint composition (Root + Door Lock) and
required clusters. Generated C++ glue lives under `generated/` and is
produced by the ZAP tool against the `.zap` file. Regenerate after any
cluster topology change:

```bash
zap-cli generate -i door_lock.zap -o generated/
```
