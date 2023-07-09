#include "common.h"
#include "config.h"

#include "synth.h"
#include "serial.h"
#include "terminal.h"
#include <thread>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <mutex>

using namespace std;
int sustained = 0;
int sustained_note[20];
bool keyPressed[100];
int numKeyPressed = 0;
mutex semo;

void audio_callback(void *user_data, Uint8 *raw_buffer, int bytes)
{
    Sint16 *buffer = (Sint16 *)raw_buffer;
    int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
    int &sample_nr(*(int *)user_data);

    for (int i = 0; i < length; i++, sample_nr++)
    {
        double time = (double)sample_nr / (double)SAMPLE_RATE;
        float sinSum = 0;
        int a = numKeyPressed;
        if (a > 0)
        {

            for (int i = 0; i < 100; i++)
            {
                sinSum += keyPressed[i] * sin(2.0f * M_PI * 16.0f * noteFreq[i % 12] * time);
            }
            int amp = AMPLITUDE / a;
            buffer[i] = (Sint16)(amp * (sinSum));
        }
    }
}

int main(int argc, char **argv)
{
    cout << "Electronic Organ by Federico Longhin\nStarting sequence:\n";

    setupSigaction();

    cout << "- Setupping Serial connection... ";
    if (!setupSerial())
    {
        cout << "Error while setupping Serial.";
        return 1;
    }
    else
        cout << "Done!\n";

    cout << "- Starting ConsoleInThread...";
    thread ConsoleInThread(consoleIn);
    cout << "Done!\n";

    SerialEvent *arr = new SerialEvent[EVENT_BUFFER_LENGTH];

    cout << "- Reading Config FIle...";

    if (fetchConfig() != 0)
    {
        cout << " Error while loading config file. ";
    }
    cout << "DONE\n";

    bootSound();

    if (SDL_Init(SDL_INIT_AUDIO) != 0)
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());

    int sample_nr = 0;

    SDL_AudioSpec want;
    want.freq = SAMPLE_RATE;        // number of samples per second
    want.format = AUDIO_S16SYS;     // sample type (here: signed short i.e. 16 bit)
    want.channels = 1;              // only one channel
    want.samples = 2048;            // buffer-size
    want.callback = audio_callback; // function SDL calls periodically to refill the buffer
    want.userdata = &sample_nr;     // counter, keeping track of current sample number

    SDL_AudioSpec have;
    if (SDL_OpenAudio(&want, &have) != 0)
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to open audio: %s", SDL_GetError());
    if (want.format != have.format)
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to get the desired AudioSpec");
    // SDL_PauseAudio(0); // stop playing sound

    cout << "All done!\n";

    while (true)
    {
        SDL_PauseAudio(numKeyPressed == 0);
        int n = checkForSerial(arr, serial_event_in_buffer);
        if (n >= 0)
        {
            serial_event_in_buffer = n;
        }
        for (int i = 0; i < serial_event_in_buffer; i++)
        {
            // cout << " for i= " << i << ", end at: " << serial_event_in_buffer << "\n";
            triggerSynthEvent(arr[i]);
        }

        // if (checkForSerial(arr, serial_event_in_buffer) > 0)
        // {
        //     triggerSynthEvent(arr[0]);
        // }
        // char* in;
        // cin>>in;
        // cout<<in;
    }
    return 0;
}
