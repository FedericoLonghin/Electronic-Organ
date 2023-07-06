#include <fluidsynth.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <errno.h>   // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <signal.h>
#include <stdlib.h>

extern fluid_settings_t *settings;
extern fluid_audio_driver_t *adriver;
extern fluid_audio_driver_t *adriver2;
extern fluid_midi_driver_t *mdriver;
extern fluid_synth_t *synth;
extern fluid_synth_t *synth2;
extern int serial_port;
extern int serial_event_in_buffer;

#define EVENT_CODE_NOTEON 144
#define EVENT_CODE_NOTEOFF 128
#define EVENT_BUFFER_LENGTH 256

class SerialEvent
{
public:
    SerialEvent() {}
    int message;
    int channel;
    int eventType;
};

class ConsoleCommand{
public :
ConsoleCommand(){}
int type;


};
enum ConsoleCommandCode{
COMMAND_EXIT,
COMMAND_LOAD,

};