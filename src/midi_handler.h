#ifndef MIDI_HANDLER_H
#define MIDI_HANDLER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialize MIDI (if needed, mostly handled by TinyUSB init)
void midi_handler_init(void);

// Send MIDI Note On
// Channel: 0-15 (usually 0)
// Note: 0-127
// Velocity: 0-127
void send_midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity);

// Send MIDI Note Off
void send_midi_note_off(uint8_t channel, uint8_t note);

// Check if USB MIDI is mounted/ready
bool is_midi_ready(void);

#ifdef __cplusplus
}
#endif

#endif
