/*
 * Copyright (c) 2026 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Transport switch HAL — optional pure interface for runtime
 * selection between Thread and Wi-Fi on platforms that support both
 * radios. Implementations are board-specific and only present where
 * the underlying SoC carries both transports.
 */

#pragma once

#include <cstdint>

namespace omsl::hal {

enum class Transport : uint8_t {
    Thread = 0,
    WiFi = 1,
};

class ITransportSwitch {
public:
    virtual ~ITransportSwitch() = default;

    virtual bool Init() = 0;

    /**
     * Request a transport switch. Implementations may defer the actual
     * switch (e.g., until current operations complete) and report it
     * via GetActiveTransport(). Returns true if the request was accepted.
     */
    virtual bool RequestTransport(Transport transport) = 0;

    virtual Transport GetActiveTransport() const = 0;
};

}  // namespace omsl::hal
