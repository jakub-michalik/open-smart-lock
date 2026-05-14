# Build Profiles

openMatterSmartLock ships three reproducible build profiles, each driven by a separate `prj_*.conf` plus the corresponding `sysbuild_*.conf`. They differ in three dimensions: log/console support, MCUboot presence, and OTA capability.

| Profile | Kconfig | sysbuild | MCUboot | OTA | Logs / shell | Typical image |
| --- | --- | --- | :---: | :---: | --- | ---: |
| **debug** | `prj.conf` | `sysbuild.conf` | – | – | yes | ~605 KiB |
| **release** | `prj_release.conf` | `sysbuild.conf` | – | – | stripped | ~461 KiB |
| **release+OTA** | `prj_release_ota.conf` | `sysbuild_release_ota.conf` | yes | yes | stripped | ~491 KiB |

## Building each profile

### debug

```bash
west build -b nanoBoard -p auto firmware --sysbuild
```

Use this for bring-up. UART console up at 115200 8N1, full Zephyr logging, full Matter logs.

### release

```bash
west build -b nanoBoard -p auto firmware --sysbuild \
    -- -DCONF_FILE=prj_release.conf
```

LTO on, `CONFIG_SIZE_OPTIMIZATIONS=y`, no `CONSOLE`, no `SHELL`, no `LOG`. Suitable for production behavior but **not** field-updatable — to upgrade you must reflash the SoC.

### release+OTA

```bash
west build -b nanoBoard -p auto firmware --sysbuild \
    -- -DFILE_SUFFIX=release_ota
```

Adds MCUboot bootloader (28 KiB) and the Matter OTA Requestor. Two image slots are carved out of internal flash; see [Partition Layout](partitions.md) for the map.

## What `FILE_SUFFIX=release_ota` does

`FILE_SUFFIX` is a sysbuild composition mechanism. It tells the build system to look for `<name>_release_ota.conf` variants throughout the project, and falls back to the default name if the suffixed version does not exist. Specifically:

- `prj_release_ota.conf` overrides `prj.conf`
- `sysbuild_release_ota.conf` overrides `sysbuild.conf`
- `pm_static_<board>_release_ota.yml` overrides any default partition layout

This is how the release+OTA profile composes correctly without parallel `samples/` trees.

## Why three profiles, not five

Two combinations are deliberately not exposed:

- **debug+OTA** — debug logging blows the primary slot size budget; not useful in practice.
- **debug+MCUboot, no OTA** — no real use case; either you want field-update (release+OTA) or you want the fastest debug cycle (flat debug).

## See also

- [Partition Layout](partitions.md) — exact flash addresses per profile
- [OTA Updates](../operations/ota.md) — runtime flow for the release+OTA profile
- [Power Budget](../guides/power-budget.md) — quiescent current under each profile
