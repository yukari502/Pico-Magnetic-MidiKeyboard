#include "mux.h"
#include "hardware_config.h"

void mux_init(void) {
    gpio_init(MUX_ADDR_0_PIN);
    gpio_set_dir(MUX_ADDR_0_PIN, GPIO_OUT);
    
    gpio_init(MUX_ADDR_1_PIN);
    gpio_set_dir(MUX_ADDR_1_PIN, GPIO_OUT);
    
    gpio_init(MUX_ADDR_2_PIN);
    gpio_set_dir(MUX_ADDR_2_PIN, GPIO_OUT);
    
    gpio_init(MUX_ADDR_3_PIN);
    gpio_set_dir(MUX_ADDR_3_PIN, GPIO_OUT);

    // If we had an Enable pin, we would init it here.
    // gpio_init(MUX_ENABLE_PIN);
    // gpio_set_dir(MUX_ENABLE_PIN, GPIO_OUT);
    // gpio_put(MUX_ENABLE_PIN, 0); // Enable active low
}

void mux_select(uint8_t channel) {
    // Basic implementation - can be optimized with gpio_put_masked if pins are contiguous
    // Our pins are 0,1,2,3 which ARE contiguous (mask 0x0F)
    
    // Safety mask
    channel &= 0x0F;

    // Fast contiguous write since pins are GP0-GP3
    // mask = 0x0F << MUX_ADDR_0_PIN (assuming MUX_ADDR_0_PIN is 0, shift is 0)
    // We update all 4 bits at once.
    
    // Verify pin continuity for optimization:
    // MUX_ADDR_0_PIN = 0
    // MUX_ADDR_1_PIN = 1
    // ...
    // So we can use gpio_put_masked
    
    gpio_put_masked(0x0F, channel);
    
    /* 
    // Fallback manual method:
    gpio_put(MUX_ADDR_0_PIN, (channel >> 0) & 1);
    gpio_put(MUX_ADDR_1_PIN, (channel >> 1) & 1);
    gpio_put(MUX_ADDR_2_PIN, (channel >> 2) & 1);
    gpio_put(MUX_ADDR_3_PIN, (channel >> 3) & 1);
    */
}
