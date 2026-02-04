#include "adc_scanner.h"
#include "hardware/adc.h"
#include "mux.h"
#include "hardware_config.h"

void adc_scanner_init(void) {
    adc_init();
    adc_gpio_init(MUX_SIG_PIN);
    adc_select_input(MUX_ADC_CH); // ADC0 corresponding to GP26

    mux_init();
}

uint16_t adc_scan_channel(uint8_t channel) {
    mux_select(channel);
    
    // Settling time
    // Plan mentions 2us.
    // busy_wait_us(2); 
    // Creating a tiny delay loop might be faster/sufficient than overhead of function call for 2us
    // But busy_wait_us is safe.
    busy_wait_us_32(2); 

    return adc_read();
}

void adc_scan_all(uint16_t *buffer) {
    // Only scan the 13 keys (0-12)
    for(int i=0; i<NUM_KEYS; i++) {
        buffer[i] = adc_scan_channel(i);
    }
}
