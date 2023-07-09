#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <errno.h>   // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <signal.h>
#include <stdlib.h>
#include <mutex>

extern bool keyPressed[100];
extern int numKeyPressed;
const float noteFreq[12]={16.35,17.32,18.35,19.45,20.6,21.83,23.12,24.5,25.96,27.5,29.14,30.87};

extern int serial_port;
extern int serial_event_in_buffer;

extern const char *sf_path;
extern std::mutex semo;

#define EVENT_CODE_NOTEON 144
#define EVENT_CODE_NOTEOFF 128
#define EVENT_BUFFER_LENGTH 256

#define AMPLITUDE  1000
#define SAMPLE_RATE  44100

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
