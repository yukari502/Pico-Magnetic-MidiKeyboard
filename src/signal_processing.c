#include "signal_processing.h"

#define DEFAULT_MIN 1800 // Fallback safe values
#define DEFAULT_MAX 3000

// Measured values from User (2026-02-04)
// Key 10 is weaker (Max ~3650 vs ~3980)
static const uint16_t FACTORY_MIN[] = { 2395, 2390, 2425, 2387, 2441, 2419, 2361, 2402, 2377, 2364, 2332, 2354, 2394 };
static const uint16_t FACTORY_MAX[] = { 3950, 3984, 3983, 3985, 3985, 3983, 3988, 3981, 3984, 3983, 3656, 3983, 3985 };

void key_init(Key *k) {
    // Determine index based on pointer math (hacky but works if array is contiguous)
    // defined in main as keys[NUM_KEYS]
    // Better to pass ID, but for now we assume caller initializes in order or we default.
    // Let's just reset to safe defaults, main loop will overwrite if we implement a load function.
    // ACTUALLY: Let's assume we change key_init to take an ID.
    // For now, I will modify this function to NOT set values, and add a helper "key_load_factory(Key* keys)"
    
    k->min_val = 2200; // Generic fallback
    k->max_val = 4000;
    
    k->current_raw = 0;
    k->current_normalized = 0.0f;
    k->state = KEY_STATE_IDLE;
    k->t1_time = 0;
    k->velocity = 0;
}

void key_load_factory_calibration(Key *keys, int count) {
    for(int i=0; i<count && i<13; i++) {
        keys[i].min_val = FACTORY_MIN[i];
        keys[i].max_val = FACTORY_MAX[i];
    }
}

void key_update_calibration(Key *k, uint16_t val) {
    if (val < k->min_val) k->min_val = val;
    if (val > k->max_val) k->max_val = val;
}

float key_info_normalize(Key *k, uint16_t raw_val) {
    // Basic protection against uncalibrated state
    if (k->max_val <= k->min_val + 10) return 0.0f; // Too close or inverted

    // Clamp input
    if (raw_val < k->min_val) raw_val = k->min_val;
    if (raw_val > k->max_val) raw_val = k->max_val;

    float range = (float)(k->max_val - k->min_val);
    float position = (float)(raw_val - k->min_val) / range;

    // Apply deadzone
    // Map [DEADZONE_BOTTOM, 1.0 - DEADZONE_TOP] to [0.0, 1.0]
    
    if (position < DEADZONE_BOTTOM) return 0.0f;
    if (position > (1.0f - DEADZONE_TOP)) return 1.0f;

    // Rescale the middle section
    float effective_range = 1.0f - DEADZONE_BOTTOM - DEADZONE_TOP;
    float rescaled = (position - DEADZONE_BOTTOM) / effective_range;
    
    return rescaled;
}
