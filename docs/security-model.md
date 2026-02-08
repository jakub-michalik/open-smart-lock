# Security model

> **Status:** placeholder. Will document the device-level security model for openMatterSmartLock as the implementation matures and as EU CRA / RED Cyber Delegated Act preparation work begins.

## Planned scope

- Commissioning trust model and BLE pre-commissioning window semantics.
- Matter Door Lock cluster ACL and user-credential storage policy.
- Firmware signing key management and OTA signing pipeline.
- Secure boot (MCUboot) configuration and root-of-trust provenance.
- Anti-rollback strategy and version pinning.
- Factory reset semantics and protected boot-time invariants.
- Threat model summary and mitigations.
- Mapping to relevant standards: EU CRA, RED Cyber Delegated Act, ETSI EN 303 645.

This file will be load-bearing for any downstream consumer pursuing certification or compliance.
