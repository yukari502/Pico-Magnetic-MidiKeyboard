#ifndef SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_H

#include <stdint.h>
#include "keyboard_types.h"

// Deadzone percentages (0.0 - 1.0)
#define DEADZONE_BOTTOM 0.10f // Increased to 10% to handle magnetic crosstalk (~80 units)
#define DEADZONE_TOP    0.02f // 2% at the top (full press)

#ifdef __cplusplus
extern "C" {
#endif

// Initialize a key with default safe values
void key_init(Key *k);

// Load hardcoded values based on user measurements
void key_load_factory_calibration(Key *keys, int count);

// Update valid min/max during calibration
void key_update_calibration(Key *k, uint16_t val);

// Normalize raw value based on calibration, applying deadzones
// Returns 0.0 (released) to 1.0 (pressed)
float key_info_normalize(Key *k, uint16_t raw_val);

#ifdef __cplusplus
}
#endif

#endif
