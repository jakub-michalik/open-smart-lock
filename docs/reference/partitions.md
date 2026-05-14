# Partition Layout

openMatterSmartLock ships three build profiles. Two of them (`debug`, `release`) are **flat** images that occupy the SoC's internal flash directly. The third (`release+OTA`) introduces MCUboot and splits internal flash into a bootloader, two image slots, and persistent storage — all inside the on-SoC 1 MiB flash, with no external NOR.

## nanoBoard / nRF52840 — flat profiles

For the `debug` and `release` profiles there is no MCUboot. The image is loaded directly at the SoC reset vector. Internal flash is partitioned by Zephyr's default Partition Manager defaults.

```text
0x00000  ┌──────────────────────────────────┐
         │ application image                 │   debug   ≈ 605 KiB
         │                                   │   release ≈ 461 KiB
         │                                   │
0xF8000  ├──────────────────────────────────┤
         │ settings_storage (32 KiB)         │
0x100000 └──────────────────────────────────┘
```

The release profile fits comfortably in the 1 MiB SoC. The debug profile is also under 1 MiB but leaves less headroom — debug logging and shell add ~144 KiB of overhead.

## nanoBoard / nRF52840 — release+OTA profile

Adding MCUboot forces two image slots (primary + secondary), which is why the release+OTA profile uses an aggressively size-optimized build (LTO, no log, no shell, no console). Both slots live in internal flash.

```text
0x00000  ┌──────────────────────────────────┐
         │ mcuboot bootloader      (28 KiB)  │
0x07000  ├──────────────────────────────────┤
         │ mcuboot_pad             (512 B)   │  ← image header
0x07200  ├──────────────────────────────────┤
         │ application image       (483 KiB) │  ← release+OTA build
         │ (mcuboot_primary span)            │
0x80000  ├──────────────────────────────────┤
         │ mcuboot_secondary       (484 KiB) │  ← OTA staging
         │                                   │
0xF9000  ├──────────────────────────────────┤
         │ settings_storage         (24 KiB) │
0xFF000  ├──────────────────────────────────┤
         │ factory_data              (4 KiB) │
0x100000 └──────────────────────────────────┘
```

The current release+OTA build fills ≈ 99 % of the primary slot (491 KiB of 484 KiB available app payload — pad and header included). This is by design: the trade-off documented in the [architecture overview](../architecture/overview.md) is that staying inside on-SoC flash means no external NOR is required, at the cost of any future feature growth needing a corresponding size reduction.

The full layout for this profile is fixed by [`firmware/pm_static_nanoBoard_release_ota.yml`](https://github.com/jakub-michalik/open-smart-lock/blob/main/firmware/pm_static_nanoBoard_release_ota.yml).

## Settings and factory data

| Partition | Purpose |
| --- | --- |
| `settings_storage` | Matter fabric data, ACLs, persistent attributes |
| `factory_data` | Device-unique commissioning data (DAC, PAI, discriminator, passcode hash, vendor/product IDs) |

The factory_data partition is written once at manufacturing time and is read-only at runtime. Settings_storage is read/write throughout the device's lifetime.

## Quick size math

| Profile | App size | Slot size | Fill | OTA |
| --- | ---: | ---: | ---: | :---: |
| debug | ~605 KiB | full flash | 59 % | no |
| release | ~461 KiB | full flash | 45 % | no |
| release+OTA | ~491 KiB | 484 KiB | 99 % | yes |

See [Build Profiles](build-profiles.md) for the exact Kconfig fragments behind each row.
