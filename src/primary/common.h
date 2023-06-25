//#include "SerialEvent.h"
#include <fluidsynth.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
 extern   fluid_settings_t *settings ;
 extern   fluid_audio_driver_t *adriver;
 extern   fluid_audio_driver_t *adriver2;
 extern   fluid_midi_driver_t *mdriver;
 extern   fluid_synth_t *synth ;
 extern   fluid_synth_t *synth2 ;
 extern int serial_port;


    #define EVENT_CODE_NOTEON 144
#define EVENT_CODE_NOTEOFF 128

class SerialEvent{
    public:
    int message;
    int channel;
    int eventType;
};

