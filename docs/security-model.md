# Security model

This document captures the device-level security model for
openMatterSmartLock as a Matter Door Lock implementation. It is
reference-quality, intended to support downstream certification work
(EU CRA, RED Cyber Delegated Act, ETSI EN 303 645) without claiming
certification itself.

## 1. Trust boundaries

```text
┌────────────────────────────────────────┐
│ Matter ecosystem (fabric)              │
│  Apple Home / Google Home / HA / etc.  │
└──────────────────┬─────────────────────┘
                   │  TLS-equivalent (CASE/PASE)
                   │  certificate-based trust
                   ▼
┌────────────────────────────────────────┐
│ Matter stack (CHIP) on device          │
│  - Fabric ACLs, session keys           │
│  - Door Lock cluster ACL, PIN ACL      │
└──────────────────┬─────────────────────┘
                   │  in-process call
                   ▼
┌────────────────────────────────────────┐
│ Lock domain (Bolt + Roster + Vault)    │
│  - State machine, PIN validation       │
└──────────────────┬─────────────────────┘
                   │  HAL interface
                   ▼
┌────────────────────────────────────────┐
│ Hardware (servo, GPIO, ADC)            │
│  - Physical actuation                  │
└────────────────────────────────────────┘
```

Each arrow is a trust boundary. Compromise at one layer is bounded
by the layer below not granting authority beyond its contract.

## 2. Commissioning trust

| Aspect | Decision |
|---|---|
| Pairing mode | Standard Matter BLE commissioning (PASE) |
| Setup PIN | 8-digit numeric, factory-provisioned (or test value in dev builds) |
| Discriminator | 12-bit, factory-provisioned |
| BLE advertising window | Open at first boot or after factory reset; closes on first successful commissioning |
| Re-commissioning | Requires factory reset (button hold 10 s — `CONFIG_OMSL_FACTORY_RESET_HOLD_TIME_MS`) |
| Verifier storage | Generated at provisioning, stored in factory partition (`pm_static.factory_data`) |

**Threat:** Attacker within BLE range during the commissioning window
joins the device to an attacker fabric. **Mitigation:** Window opens
only at factory reset (requires physical access). Setup PIN is unique
per device.

## 3. Door Lock cluster access control

| Surface | Policy |
|---|---|
| `LockDoor` / `UnlockDoor` | Requires Matter `Operate` privilege on Door Lock cluster |
| `SetCredential` / `ClearCredential` | Requires `Administer` privilege; tracked in `Roster` |
| `SetUser` / `ClearUser` | Requires `Administer` privilege |
| PIN over-the-air | `RequirePINforRemoteOperation` Kconfig-gated; default ON in release |
| Anti-brute-force | Wrong-PIN counter, exponential backoff after 5 failures |

`Roster::ValidatePin()` is the single point of authority for PIN
acceptance. PINs are compared in constant time. Failed-attempt counters
are persisted via `Vault` so that resets do not clear them.

## 4. Credential storage

- **In-memory model**: `Roster` keeps the live list of users and
  credentials.
- **Persistence**: `Vault` writes snapshots to the settings storage
  partition on explicit mutation events (commissioning, credential
  add / remove, factory reset). Not write-through on every PIN check.
- **At-rest protection**: relies on NCS settings backend, which on
  nrf52840 uses NVS in internal flash. **No PSA-backed protected
  storage in the no-NOR profile.** Production deployments should
  enable `CONFIG_TRUSTED_STORAGE` and PSA Secure Storage where the
  hardware supports it.

## 5. Firmware update / OTA

| Aspect | Decision |
|---|---|
| OTA in default profile | **Disabled** (no MCUboot in the no-NOR build) |
| OTA in v0.8 profile | Matter OTA Requestor over BDX |
| Provider placement | Local: OTBR host or HA add-on (no cloud Provider) |
| Image signing | Project-controlled key; public key in MCUboot region |
| Anti-rollback | Monotonic counter; older versions refused after critical updates |
| Apply strategy | Dual-bank via MCUboot; atomic pointer flip after verification |
| Energy gate | OTA refuses to start below configurable battery threshold |
| Trigger | User-initiated only (no auto-apply) |
| Lock state during apply | Pre-apply mechanical state restored on first boot |

**Active-profile invariance**: an OTA must never change the active
Matter device type. Adding device profiles via OTA is allowed but
switching requires factory reset.

## 6. Secure boot

- **Bootloader**: MCUboot on profiles where OTA is enabled.
- **Signing**: ECDSA-P256 with project key. Bootloader verifies
  signature on each boot; failure halts.
- **Anti-tampering**: AP-PROTECT / approtect locked in production
  hardware to prevent SWD readout of credentials.
- **Provisioning**: factory data (vendor ID, serial, certificates,
  PIN verifier) written once at manufacture, read-only thereafter.

## 7. Factory reset semantics

Hold the action button for `OMSL_FACTORY_RESET_HOLD_TIME_MS` (default
10 s) to trigger.

On factory reset:
1. **Cancel any in-flight motion** and return the bolt to a safe
   resting position (typically `Unlocked`).
2. **Clear `Roster`** — all users, credentials, schedules removed.
3. **Wipe `Vault`** — settings storage partition erased.
4. **Forget all fabrics** — Matter `RemoveFabric` invoked for each.
5. **Reboot** into commissioning advertising mode.

**Does NOT** auto-unlock the door. The bolt does not move during a
factory reset trigger initiated while the door is locked.

## 8. Threat model summary

| Threat | Vector | Mitigation |
|---|---|---|
| Replay of `UnlockDoor` | Captured Matter command | Matter session keys; nonce-based message protection |
| BLE proximity attack during commissioning | Adversary in physical range | Commissioning window opens only on factory reset (physical) |
| PIN brute force | Repeated cluster invocations | Wrong-counter + exponential backoff |
| Flash readout (SWD) | Physical device access | AP-PROTECT in production hardware |
| Compromised OTA | Malicious signed image | Project signing key; anti-rollback |
| Profile-swap via OTA | `LockDoor` → `WindowCovering` | OTA must not change active device type |
| Battery drain attack | Adversary sending many BLE adv responses | Commissioning window short (60 s, then off) |
| Servo lockup mid-motion | Mechanical jam | Stall detection in actuator HAL; timeout returns `Stalled` |
| Power loss mid-motion | Sudden battery loss | Pre-update state restored on first boot; no atomic-write claim during motion |

## 9. Standards mapping (high-level intent)

| Standard | Where it lands |
|---|---|
| **EU CRA** Annex I (essential cybersecurity requirements) | Sections 2, 3, 5, 6, 7 collectively |
| **RED Cyber Delegated Act** | Mostly Section 2 (commissioning), Section 5 (updates) |
| **ETSI EN 303 645** consumer IoT baseline | Mostly Section 2 (unique credentials) and Section 5 (updates) |
| **EU MDR** | Not applicable (not a medical device) |
| **EN 14846 (door locks)** | Out of software scope; mechanical certification only |

This document is **not** a compliance statement — it is the design
intent that a compliance program would test against.

## 10. Open questions tracked

- **Anti-tampering on nanoBoard and custom hardware** without
  AP-PROTECT support: documented as a known gap.
- **Wrong-PIN counter persistence** across factory reset is debated:
  current draft keeps it across to prevent brute-force-via-reset, but
  may need to be cleared if used in a multi-tenant retrofit scenario.
- **Local OTA Provider trust** when running on a non-trusted home LAN:
  needs additional thought before v0.8.
<!-- typo guard -->

