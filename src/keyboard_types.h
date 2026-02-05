#ifndef KEYBOARD_TYPES_H
#define KEYBOARD_TYPES_H

#include <stdint.h>
#include <stdbool.h>

// Key States for the State Machine (Phase 4), defined here for struct completeness
typedef enum {
    KEY_STATE_IDLE,
    KEY_STATE_PRESS_DETECTED, // Passed T1
    KEY_STATE_WAIT_FOR_T2,    // Between T1 and T2 (Logic might vary)
    KEY_STATE_NOTE_ON,        // Passed T2, Note Sent
    KEY_STATE_RELEASE_DETECTED
} key_state_t;

typedef struct {
    // Calibration Data
    uint16_t min_val; // Resting position (Magnetic flux min/max depends on polarity, we assume raw value)
    uint16_t max_val; // Bottom out position
    
    // Real-time State
    uint16_t current_raw;
    float current_normalized; // 0.0 to 1.0
    
    // Logic State
    key_state_t state;
    uint32_t t1_time;         // Timestamp when T1 was crossed
    uint8_t velocity;          // Calculated velocity
    uint8_t active_midi_note; // The MIDI note number actually sent (for Note Off)
    uint8_t last_pressure;    // Last sent aftertouch value
} Key;

#endif
