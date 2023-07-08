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

#define FLUIDSYNTH_ISTANCE_NUMBER 5
extern fluid_settings_t *settings;
extern fluid_audio_driver_t *adriver[FLUIDSYNTH_ISTANCE_NUMBER];
// extern fluid_audio_driver_t *adriver2;
extern fluid_synth_t *synth[FLUIDSYNTH_ISTANCE_NUMBER];
extern int serial_port;
extern int serial_event_in_buffer;

extern const char *sf_path;

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

class ConsoleCommand
{
public:
    ConsoleCommand() {}
    int type;
};
enum ConsoleCommandCode
{
    COMMAND_EXIT,
    COMMAND_LOAD,
    COMMAND_ADD,
    COMMAND_GET_SOUNDFONT_LIST,
    COMMAND_SET_LEGATO

};
