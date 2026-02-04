#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <stdint.h>
#include <stdbool.h>
#include "keyboard_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Run the calibration sequence
// This mode is blocking in a sense that it captures min/max
// Returns true if calibration seems valid (range sufficient)
bool calibration_mode(Key *keys, uint16_t *current_adc_values, int num_keys);

#ifdef __cplusplus
}
#endif

#endif
