/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "console.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

namespace omsl::ui {

static const struct gpio_dt_spec sLed =
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(led_status), gpios, {0});

static const struct gpio_dt_spec sButton =
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw_action), gpios, {0});

static ButtonEventCallback sButtonCallback;
static struct gpio_callback sButtonCbData;

static void ButtonHandler(const struct device * port, struct gpio_callback * cb, uint32_t pins)
{
    if (sButtonCallback) {
        sButtonCallback();
    }
}

Console & Console::Instance()
{
    static Console inst;
    return inst;
}

bool Console::Init()
{
    if (sLed.port) {
        gpio_pin_configure_dt(&sLed, GPIO_OUTPUT_INACTIVE);
    }
    if (sButton.port) {
        gpio_pin_configure_dt(&sButton, GPIO_INPUT);
        gpio_pin_interrupt_configure_dt(&sButton, GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&sButtonCbData, ButtonHandler, BIT(sButton.pin));
        gpio_add_callback(sButton.port, &sButtonCbData);
    }
    LOG_INF("ui::Console: init");
    return true;
}

void Console::SetActionButtonCallback(ButtonEventCallback cb)
{
    sButtonCallback = std::move(cb);
}

void Console::SetStatusLed(bool on)
{
    if (sLed.port) gpio_pin_set_dt(&sLed, on ? 1 : 0);
}

void Console::BlinkStatusLed(uint32_t period_ms)
{
    // Simple blink: caller provides period; actual blink is driven by a timer (TODO)
    SetStatusLed(true);
}

}  // namespace omsl::ui
