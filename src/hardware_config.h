#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

#include "pico/stdlib.h"

// --- Multiplexer (CD74HC4067) ---
// Address Pins
#define MUX_ADDR_0_PIN 0
#define MUX_ADDR_1_PIN 1
#define MUX_ADDR_2_PIN 2
#define MUX_ADDR_3_PIN 3

// Signal Pin (Analog Input)
#define MUX_SIG_PIN 26 // ADC0
#define MUX_ADC_CH  0

// Enable Pin (Active Low) - Hardwired to GND in this project, but defining just in case
// #define MUX_ENABLE_PIN ... 

// --- Buttons ---
// --- Buttons ---
#define BTN_OCTAVE_UP_PIN   16 // U17 - Increase (Corrected)
#define BTN_OCTAVE_DOWN_PIN 15 // U16 - Decrease (Corrected)

// --- LEDs (Octave Indicators) ---
// Sequence: -2, -1, 0, +1, +2
// Based on PCB image: Leftmost LED connected to Pin 14 (GP10)
#define LED_OCT_MINUS_2_PIN 10
#define LED_OCT_MINUS_1_PIN 11
#define LED_OCT_0_PIN       12
#define LED_OCT_PLUS_1_PIN  13
#define LED_OCT_PLUS_2_PIN  14

// Helper array for LEDs
static const uint LED_PINS[] = {
    LED_OCT_PLUS_2_PIN,
    LED_OCT_PLUS_1_PIN,
    LED_OCT_0_PIN,
    LED_OCT_MINUS_1_PIN,
    LED_OCT_MINUS_2_PIN
};
#define LED_COUNT 5

// --- Constants ---
#define NUM_KEYS 13

#endif // HARDWARE_CONFIG_H
