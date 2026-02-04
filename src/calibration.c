#include "calibration.h"
#include "signal_processing.h"

// This strictly updates the calibration limits based on current inputs.
// The main loop decides WHEN to call this.
bool calibration_mode(Key *keys, uint16_t *current_adc_values, int num_keys) {
    bool all_valid = true;
    for(int i=0; i<num_keys; i++) {
        key_update_calibration(&keys[i], current_adc_values[i]);
        
        // Check if this key has decent range (> 100 units maybe?)
        if ((keys[i].max_val - keys[i].min_val) < 100) {
            all_valid = false;
        }
    }
    return all_valid;
}
