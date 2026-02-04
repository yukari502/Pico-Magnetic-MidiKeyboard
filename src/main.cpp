#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware_config.h"

// TinyUSB is a C library, ensure C++ linkage work
#ifdef __cplusplus
extern "C" {
#endif
#include "tusb.h"
#ifdef __cplusplus
}
#endif

#include "adc_scanner.h"
#include "keyboard_types.h"
#include "signal_processing.h"
#include "calibration.h"
#include "fsm.h"

// Globals
Key keys[NUM_KEYS];
uint16_t adc_values[NUM_KEYS];

// Octave Control Logic
void update_octave_controls() {
    static uint32_t last_button_time = 0;
    const uint32_t debounce_delay = 200000; // 200ms
    
    // Read Buttons (Active Low typically for internal pullups)
    bool btn_up = !gpio_get(BTN_OCTAVE_UP_PIN);
    bool btn_down = !gpio_get(BTN_OCTAVE_DOWN_PIN);
    
    uint32_t now = time_us_32();
    
    if (now - last_button_time > debounce_delay) {
        if (btn_up) {
            if (global_transpose < 24) global_transpose += 12; // Up should ADD
            last_button_time = now;
        } else if (btn_down) {
            if (global_transpose > -24) global_transpose -= 12; // Down should SUBTRACT
            last_button_time = now;
        }
    }
    
    // Update LEDs
    // Index: -2 -> 0, -1 -> 1, 0 -> 2, +1 -> 3, +2 -> 4
    int led_index = (global_transpose / 12) + 2; 
    
    for (int i = 0; i < LED_COUNT; i++) {
        gpio_put(LED_PINS[i], (i == led_index) ? 1 : 0);
    }
}

int main() {
    // stdio_init_all(); // Disabled for MIDI usage unless using UART
    // board_init(); // Not needed (TinyUSB example helper), SDK setup is enough
    tusb_init();  // TinyUSB stack init
    adc_scanner_init();

    // Init LEDs
    for (int i = 0; i < LED_COUNT; i++) {
        gpio_init(LED_PINS[i]);
        gpio_set_dir(LED_PINS[i], GPIO_OUT);
    }
    
    // Init Buttons
    gpio_init(BTN_OCTAVE_UP_PIN);
    gpio_set_dir(BTN_OCTAVE_UP_PIN, GPIO_IN);
    gpio_pull_up(BTN_OCTAVE_UP_PIN);
    
    gpio_init(BTN_OCTAVE_DOWN_PIN);
    gpio_set_dir(BTN_OCTAVE_DOWN_PIN, GPIO_IN);
    gpio_pull_up(BTN_OCTAVE_DOWN_PIN);
    
    // Init Keys & Load Factory Data
    for(int i=0; i<NUM_KEYS; i++) key_init(&keys[i]);
    key_load_factory_calibration(keys, NUM_KEYS);

    // Optional: Flash LEDs once
    for(int i=0; i<LED_COUNT; i++) gpio_put(LED_PINS[i], 1);
    sleep_ms(500);
    for(int i=0; i<LED_COUNT; i++) gpio_put(LED_PINS[i], 0);

    while (true) {
        tud_task(); // TinyUSB Device Task
        
        // Poll Buttons & LEDs
        update_octave_controls();
        
        adc_scan_all(adc_values);
        uint32_t timestamp = time_us_32();

        for(int i=0; i<NUM_KEYS; i++) {
            // 1. Normalize
            float norm = key_info_normalize(&keys[i], adc_values[i]);
            keys[i].current_normalized = norm;
            
            // 2. Process Logic
            process_key_state(&keys[i], timestamp, i);
        }
    }
    return 0;
}
