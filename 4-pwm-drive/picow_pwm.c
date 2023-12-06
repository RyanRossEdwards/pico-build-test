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


// Function prototypes
void car_setup_pwm();
void car_drive_forward();
void car_drive_backward();
void car_drive_coast();
void wheel_setup_pwm(int gpio_1, int gpio_2);
void wheel_drive_forward(int gpio_1, int gpio_2);
void wheel_drive_backward(int gpio_1, int gpio_2);
void wheel_drive_coast(int gpio_1, int gpio_2);


int main() {
    // Initialise standard library for Pico
    stdio_init_all();

    // Initalise the PWM
    car_setup_pwm();

    // Motor Drive
    for(;;) {
        car_drive_forward();
        sleep_ms(1000);

        car_drive_coast();
        sleep_ms(1000);

        car_drive_backward();
        sleep_ms(1000);

        car_drive_coast();
        sleep_ms(1000);
    }

    return 0;
}

// car functions

void car_setup_pwm() {
    // Initalise the PWM
    wheel_setup_pwm(6, 7);    // Right Back
    wheel_setup_pwm(8, 9);    // Right Front
    wheel_setup_pwm(18, 19);  // Left Front
    wheel_setup_pwm(20, 21);  // Left Back
}

void car_drive_forward() {
    // All wheels forward
    wheel_drive_forward(6, 7);    // Right Back
    wheel_drive_forward(8, 9);    // Right Front
    wheel_drive_backward(18, 19); // Left Front
    wheel_drive_forward(20, 21);  // Left Back
}

void car_drive_backward() {
    // All wheels backward
    wheel_drive_backward(6, 7);    // Right Back
    wheel_drive_backward(8, 9);    // Right Front
    wheel_drive_forward(18, 19);   // Left Front
    wheel_drive_backward(20, 21);  // Left Back
}

void car_drive_coast() {
    // All wheels coast
    wheel_drive_coast(6, 7);    // Right Back
    wheel_drive_coast(8, 9);    // Right Front
    wheel_drive_coast(18, 19);  // Left Front
    wheel_drive_coast(20, 21);  // Left Back
}



// wheel functions

void wheel_setup_pwm(int gpio_1, int gpio_2) {
    // Set GPIO pins to PWM function
    gpio_set_function(gpio_1, GPIO_FUNC_PWM);
    gpio_set_function(gpio_2, GPIO_FUNC_PWM);

    // Get the slice number (gpio_1 and gpio_2 must be on the same slice)
    uint slice_num = pwm_gpio_to_slice_num(gpio_1);

    // Set the PWM frequency
    uint32_t sys_clk_freq = clock_get_hz(clk_sys); // System clock frequency
    float divider = sys_clk_freq / (500.0f * 4096.0f); // Desired frequency is 500 Hz, counter range is 4096
    pwm_set_clkdiv(slice_num, divider);

    // Set the PWM wrap value to maximum, for full resolution
    pwm_set_wrap(slice_num, 4095);
}


void wheel_drive_forward(int gpio_1, int gpio_2) {
    // Reassign GPIO to PWM function
    gpio_set_function(gpio_1, GPIO_FUNC_PWM); 
    gpio_set_function(gpio_2, GPIO_FUNC_PWM);

    // Get the slice number (gpio_1 and gpio_2 must be on the same slice)
    uint slice_num = pwm_gpio_to_slice_num(gpio_1);

     // Forward direction: PWM on GPIO 6 (Channel A), GPIO 7 (Channel B) low
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);     // LOW
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 2048);  // 50% duty cycle
    pwm_set_enabled(slice_num, true);                 // Enable PWM
}


void wheel_drive_backward(int gpio_1, int gpio_2) {
    // Reassign GPIO to PWM function
    gpio_set_function(gpio_1, GPIO_FUNC_PWM); 
    gpio_set_function(gpio_2, GPIO_FUNC_PWM);

    // Get the slice number (gpio_1 and gpio_2 must be on the same slice)
    uint slice_num = pwm_gpio_to_slice_num(gpio_1);

    // Backward direction: PWM on GPIO 6 (Channel A), GPIO 7 (Channel B) low
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 2048);  // 50% duty cycle
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 0);     // LOW
    pwm_set_enabled(slice_num, true);                 // Enable PWM
}


void wheel_drive_coast(int gpio_1, int gpio_2) {
    // Get the slice number (gpio_1 and gpio_2 must be on the same slice)
    uint slice_num = pwm_gpio_to_slice_num(gpio_1);

    // Coasting: Disable PWM and set both GPIOs to high-impedance
    pwm_set_enabled(slice_num, false);

    gpio_set_function(gpio_1, GPIO_FUNC_SIO);
    gpio_set_dir(gpio_1, GPIO_IN);

    gpio_set_function(gpio_2, GPIO_FUNC_SIO);
    gpio_set_dir(gpio_2, GPIO_IN);
}


