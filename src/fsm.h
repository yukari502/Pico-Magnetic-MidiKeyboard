#ifndef FSM_H
#define FSM_H

#include "keyboard_types.h"
#include "pico/stdlib.h"

// Logic Thresholds (Normalized 0.0 - 1.0)
// T1: Start timing point (Hair trigger start)
#define THRESHOLD_T1 0.05f 

// T2: End timing point (Note On trigger - shallow actuation)
#define THRESHOLD_T2 0.40f 

// Reset: Note Off point (Schmitt trigger hysteresis)
// Must be lower than T1 to ensure clean release
#define THRESHOLD_RESET 0.04f 

// Velocity Curve Settings
#define VELOCITY_CURVE_FACTOR 2.0f // 1.0 = Linear, 2.0 = Quadratic (Heavy)
#define MIN_NOTE_TIME_US 1000      // 1ms (Fastest possible press)
#define MAX_NOTE_TIME_US 150000    // 150ms (Slowest considered press) 

extern int8_t global_transpose; // -24 to +24 semitones

#ifdef __cplusplus
extern "C" {
#endif

void process_key_state(Key *key, uint32_t timestamp_us, uint8_t key_index);

#ifdef __cplusplus
}
#endif

#endif // FSM_H
