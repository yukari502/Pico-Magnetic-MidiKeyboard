#ifndef MUX_H
#define MUX_H

#include "pico/stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize Mux GPIOs
void mux_init(void);

// Select a channel (0-15)
// This function sets the S0-S3 pins
void mux_select(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif
