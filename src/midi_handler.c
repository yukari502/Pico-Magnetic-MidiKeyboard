#include "midi_handler.h"
#include "tusb.h"

void midi_handler_init(void) {
    // Nothing specific needed here if tusb_init is called
}

bool is_midi_ready(void) {
    return tud_midi_mounted();
}

void send_midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity) {
    // Only send if USB is ready
    if (!tud_midi_mounted()) return;

    // Cable 0, Channel, Note, Velocity
    uint8_t cable_num = 0;
    
    // Note On message
    uint8_t note_on[3] = { 0x90 | (channel & 0x0F), note, velocity };
    
    tud_midi_stream_write(cable_num, note_on, 3);
}

void send_midi_note_off(uint8_t channel, uint8_t note) {
    if (!tud_midi_mounted()) return;

    uint8_t cable_num = 0;
    
    // Note Off message
    uint8_t note_off[3] = { 0x80 | (channel & 0x0F), note, 0 };
    
    tud_midi_stream_write(cable_num, note_off, 3);
}
