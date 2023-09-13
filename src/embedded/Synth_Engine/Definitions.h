#ifndef Definitions_h
#define Definitions_h

// General:
#define Sample_Rate 40000
#define MAX_AUDIO_OBJECT_PER_CHANNEL 44
#define MAX_AUDIO_OBJECT_CHANNEL 3
#define MAX_AUDIO_ACTIVE_OBJECT_NUMBER 44
#define MAX_SOUNDS_NUMBER MAX_AUDIO_OBJECT_CHANNEL
#define MAGIC_BUFFER_OFFSET 100  //Output buffer size

// Wavetable
#define Wavetable_Length 200
#define MaxWaveTypes 5
#define Wavetable_MaxAmplitude_val 255
#define WAVETYPE_SIN 0
#define WAVETYPE_TRIANG 1
#define WAVETYPE_SAW 2
#define WAVETYPE_SQUARE 3
#define WAVETYPE_SIN_3 4

// Envelopes
#define Envelope_AttackDecay_Table_Length 10000
#define Envelope_Release_Table_Length 5000

#endif