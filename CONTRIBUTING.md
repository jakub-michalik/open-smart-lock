# Contributing to openMatterSmartLock

Thanks for your interest in contributing. openMatterSmartLock is in active development and contributions are welcome, especially:

- additional board support (overlay + Kconfig + partition manager configs),
- new actuator or feedback driver implementations against the existing HAL,
- documentation improvements,
- bug reports backed by reproducible builds.

## Ground rules

- **Match the layered architecture.** Domain code does not call into drivers directly; new hardware integration belongs in `firmware/drivers/<category>/<name>/` behind the appropriate HAL interface. See [ARCHITECTURE.md](ARCHITECTURE.md).
- **No manufacturer-specific clusters in upstream.** The protocol surface stays pure Matter Door Lock so the device remains interoperable everywhere. Product-specific extensions belong in downstream consumer repositories.
- **License.** All contributions are licensed under Apache-2.0 (see [LICENSE](LICENSE)). If you contribute code that you did not author yourself, ensure it is compatible with Apache-2.0 and that its copyright lines are preserved per the [NOTICE](NOTICE) file conventions.

## Pull request checklist

- The build passes for at least one supported board (`west build -b <board> -p auto firmware`).
- New code includes appropriate file-level copyright / license headers.
- Public APIs in HAL or domain layers carry doc comments.
- For new boards or new drivers: a short note in [CHANGELOG.md](CHANGELOG.md) under "Unreleased".

## Reporting issues

Please include:

- the target board and west / NCS versions,
- the command used to build,
- the full failing output,
- a minimal reproduction path if possible.
