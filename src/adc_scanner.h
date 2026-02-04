#ifndef ADC_SCANNER_H
#define ADC_SCANNER_H

#include "pico/stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize ADC and Mux
void adc_scanner_init(void);

// Read a single channel from Mux (Selects channel -> waits -> reads)
// blocking call
uint16_t adc_scan_channel(uint8_t channel);

// Scan all connected channels (0-12) and store in buffer
// buffer must be size 13
void adc_scan_all(uint16_t *buffer);

#ifdef __cplusplus
}
#endif

#endif
