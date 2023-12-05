/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Output PWM signals on pins 0 and 1

// Import the required standard libraries and hardware PWM library.
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

int main() {
    // Initialize standard library for Pico
    stdio_init_all();

    // Set GPIO 6 and 7 to PWM function
    gpio_set_function(6, GPIO_FUNC_PWM);
    gpio_set_function(7, GPIO_FUNC_PWM);

    // Get the slice number for GPIO 6 (and also GPIO 7 since they are on the same slice)
    uint slice_num = pwm_gpio_to_slice_num(6);

    // Set the PWM frequency
    uint32_t sys_clk_freq = clock_get_hz(clk_sys); // System clock frequency
    float divider = sys_clk_freq / (500.0f * 4096.0f); // Desired frequency is 500 Hz, counter range is 4096
    pwm_set_clkdiv(slice_num, divider);

    // Set the PWM wrap value to maximum, for full resolution
    pwm_set_wrap(slice_num, 4095);

    // Motor Drive

    // Forward direction: PWM on GPIO 6 (Channel A), GPIO 7 (Channel B) low
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 2048); // 50% duty cycle
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 0);    // LOW
    pwm_set_enabled(slice_num, true);                // Enable PWM
    sleep_ms(1000);

    // Coasting: Disable PWM and set both GPIOs to high-impedance
    pwm_set_enabled(slice_num, false);
    gpio_set_function(6, GPIO_FUNC_SIO);
    gpio_set_dir(6, GPIO_IN);
    gpio_set_function(7, GPIO_FUNC_SIO);
    gpio_set_dir(7, GPIO_IN);
    sleep_ms(1000);

    // Reverse direction: PWM on GPIO 7 (Channel B), GPIO 6 (Channel A) low
    gpio_set_function(6, GPIO_FUNC_PWM); // Reassign GPIO 6 to PWM function
    gpio_set_function(7, GPIO_FUNC_PWM); // Reassign GPIO 7 to PWM function
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);    // LOW for direction
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 2048); // 50% duty cycle for reverse
    pwm_set_enabled(slice_num, true);                // Enable PWM
    sleep_ms(1000);

    // Coasting: Disable PWM and set both GPIOs to high-impedance
    pwm_set_enabled(slice_num, false);
    gpio_set_function(6, GPIO_FUNC_SIO);
    gpio_set_dir(6, GPIO_IN);
    gpio_set_function(7, GPIO_FUNC_SIO);
    gpio_set_dir(7, GPIO_IN);
    sleep_ms(1000);

}
