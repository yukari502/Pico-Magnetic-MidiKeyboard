#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------+
// SIMULATION
//--------------------------------------------------------------------+
#define CFG_TUSB_RHPORT0_MODE     OPT_MODE_DEVICE

//--------------------------------------------------------------------+
// DEVICE CONFIGURATION
//--------------------------------------------------------------------+
#ifndef CFG_TUD_ENDPOINT0_SIZE
#define CFG_TUD_ENDPOINT0_SIZE    64
#endif

//--------------------------------------------------------------------+
// CLASS DRIVER CONFIGURATION
//--------------------------------------------------------------------+
#define CFG_TUD_MIDI              1
#define CFG_TUD_MIDI_RX_BUFSIZE   64
#define CFG_TUD_MIDI_TX_BUFSIZE   64

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */
