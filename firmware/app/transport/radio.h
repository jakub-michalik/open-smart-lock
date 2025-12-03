/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Optional runtime transport switching between Thread and Wi-Fi for
 * platforms that carry both radios (nrf5340 + nrf7002).
 */

#pragma once

#include "../hal/transport_switch.h"

namespace omsl::transport {

class Radio : public hal::ITransportSwitch {
public:
    static Radio & Instance();

    bool Init() override;
    bool RequestTransport(hal::Transport transport) override;
    hal::Transport GetActiveTransport() const override { return mActive; }

private:
    Radio() = default;

    hal::Transport mActive { hal::Transport::Thread };
};

}  // namespace omsl::transport
