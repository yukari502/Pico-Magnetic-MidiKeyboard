#include "fsm.h"
#include "midi_handler.h"
#include <stdio.h>
#include <math.h>

int8_t global_transpose = 0;

// Helper to constrain input
static inline uint8_t constrain_uint8(int x, int a, int b) {
    if (x < a) return a;
    if (x > b) return b;
    return x;
}

// Map a value from one range to another
static inline long map_long(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Helper to calculate velocity with an exponential curve
static uint8_t calculate_velocity_curve(uint32_t delta_t) {
    if (delta_t < MIN_NOTE_TIME_US) delta_t = MIN_NOTE_TIME_US;
    if (delta_t > MAX_NOTE_TIME_US) delta_t = MAX_NOTE_TIME_US;

    // 1. Normalize time to 0.0 - 1.0 (inverted: 1.0 is fast/short time)
    // ratio = 1.0 - (current - min) / (max - min)
    float ratio = 1.0f - (float)(delta_t - MIN_NOTE_TIME_US) / (float)(MAX_NOTE_TIME_US - MIN_NOTE_TIME_US);

    // 2. Apply Curve (Gamma Correction)
    // curve > 1.0 pushes mid-tones down (requiring faster press for higher velocity)
    float curved = powf(ratio, VELOCITY_CURVE_FACTOR);

    // 3. Scale to MIDI 1-127
    int velocity = (int)(curved * 126.0f) + 1;
    return constrain_uint8(velocity, 1, 127);
}

void process_key_state(Key *key, uint32_t timestamp_us, uint8_t key_index) {
    float pos = key->current_normalized;
    
    switch (key->state) {
        case KEY_STATE_IDLE:
            // Waiting for press to cross T1
            if (pos > THRESHOLD_T1) {
                key->state = KEY_STATE_PRESS_DETECTED;
                key->t1_time = timestamp_us;
            }
            break;

        case KEY_STATE_PRESS_DETECTED:
            // Check if we crossed T2 (Note On)
            if (pos > THRESHOLD_T2) {
                // Calculate time delta
                uint32_t delta_t = timestamp_us - key->t1_time;
                if (delta_t == 0) delta_t = 1;
                
                // Calculate Velocity with Curve
                key->velocity = calculate_velocity_curve(delta_t);
                
                // Map index to MIDI note (C4 start = 60)
                int note_val = 60 + key_index + global_transpose;
                uint8_t midi_note = constrain_uint8(note_val, 0, 127); 
                
                // Store the note we are about to play so we can release the SAME note later
                key->active_midi_note = midi_note;

                send_midi_note_on(0, midi_note, key->velocity);
                key->state = KEY_STATE_NOTE_ON; // <--- CRITICAL FIX: prevents spam
                key->last_pressure = 0; // Reset aftertouch pressure
                
                // Temporary Print for Debugging (Simulate MIDI Send)
                // printf("ON: Vel=%d (dt=%dus)\n", key->velocity, delta_t);
            }
            // Check if we released back below T1 before hitting T2 (bounce)
            else if (pos < THRESHOLD_RESET) {
                 key->state = KEY_STATE_IDLE;
            }
            break;

        case KEY_STATE_NOTE_ON:
            // Waiting for release below Reset Threshold
            // Note: We might want a "Release Velocity" later (T2->T1 time)
            if (pos < THRESHOLD_RESET) {
                key->state = KEY_STATE_IDLE;
                
                // Use the stored note to Ensure we turn off the correct pitch
                // even if the user changed octaves while holding the key.
                send_midi_note_off(0, key->active_midi_note);
                
                // Temporary Print
                // printf("OFF\n");
            } else {
                // --- Polyphonic Aftertouch Logic ---
                // Map pressure from "Aftertouch Start" (e.g. 0.45) to 1.0
                // This allows the user to press harder to trigger effects
                const float AT_START = 0.45f;
                
                uint8_t pressure = 0;
                if (pos > AT_START) {
                    float ratio = (pos - AT_START) / (1.0f - AT_START);
                    if (ratio > 1.0f) ratio = 1.0f;
                    pressure = (uint8_t)(ratio * 127.0f);
                }

                // Send only if changed significantly (Anti-Flood)
                // Using a delta > 1 to filter noise
                int delta = (int)pressure - (int)key->last_pressure;
                if (delta < 0) delta = -delta;

                if (delta > 2) { 
                    send_midi_poly_aftertouch(0, key->active_midi_note, pressure);
                    key->last_pressure = pressure;
                }
            }
            break;
            
        default:
            key->state = KEY_STATE_IDLE;
            break;
    }
}
