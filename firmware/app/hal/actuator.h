/*
 * Copyright (c) 2026 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Actuator HAL — pure interface for any physical actuator capable of
 * moving the bolt of a lock between configured positions.
 *
 * Implementations live under firmware/drivers/actuator/<name>/ and are
 * selected at build time via Kconfig. No domain-layer code may include
 * a driver header directly; all access goes through this interface.
 */

#pragma once

#include <cstdint>
#include <functional>

namespace omsl::hal {

enum class ActuatorTarget : uint8_t {
    Locked = 0,
    Unlocked = 1,
};

enum class ActuatorResult : uint8_t {
    Success = 0,
    Stalled = 1,
    TimedOut = 2,
    AbortedByCaller = 3,
    Unsupported = 4,
};

using ActuatorCompletionCallback = std::function<void(ActuatorResult)>;

class IActuator {
public:
    virtual ~IActuator() = default;

    /**
     * Initialize the underlying hardware. Returns true on success.
     * Called once during Runtime boot, before any MoveTo() call.
     */
    virtual bool Init() = 0;

    /**
     * Start asynchronous motion toward target. The completion callback
     * is invoked exactly once for each successful MoveTo() call, on
     * either Success or a failure outcome (Stalled / TimedOut / Aborted).
     *
     * If MoveTo is called while a previous motion is in progress, the
     * implementation should either reject the new call (return false)
     * or stop the previous motion and replace it; behavior is driver-
     * defined but must be consistent. Returns true if motion was
     * accepted.
     */
    virtual bool MoveTo(ActuatorTarget target, ActuatorCompletionCallback cb) = 0;

    /**
     * Abort any motion in progress. The pending completion callback,
     * if any, is invoked with AbortedByCaller. Safe to call when no
     * motion is in progress.
     */
    virtual void Stop() = 0;

    /**
     * True if a MoveTo() is currently in progress.
     */
    virtual bool IsBusy() const = 0;

    /**
     * Configured maximum actuation duration in milliseconds. Implementations
     * that exceed this duration must abort motion and report TimedOut.
     */
    virtual uint32_t MaxActuationTimeMs() const = 0;
};

}  // namespace omsl::hal
