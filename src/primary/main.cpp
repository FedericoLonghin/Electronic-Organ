#include "common.h"
#include "config.h"

#include "synth.h"
#include "serial.h"
#include "terminal.h"
#include <thread>

int sustained = 0;
int sustained_note[20];

fluid_settings_t *settings;
fluid_audio_driver_t *adriver;
fluid_audio_driver_t *adriver2;
fluid_synth_t *synth;
fluid_synth_t *synth2;

using namespace std;

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

    cout << "- Setupping Fluidsynth... ";
    int sfont_id;

    settings = new_fluid_settings();
    fluid_settings_setstr(settings, "audio.driver", "pulseaudio");

    synth = new_fluid_synth(settings);
    // synth2 = new_fluid_synth(settings);
    adriver = new_fluid_audio_driver(settings, synth);
    // adriver2 = new_fluid_audio_driver(settings, synth2);

    sfont_id = fluid_synth_sfload(synth, "../../../soundfonts/example2.sf2", 1);
    // sfont_id = fluid_synth_sfload(synth, "../soundfonts/example.sf2", 1);
    //  sfont_id = fluid_synth_sfload(synth, "../soundfonts/minimoog_leads.sf2", 1);
    if (sfont_id == FLUID_FAILED)
    {
        puts("Loading the SoundFont failed!");
    }
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
    cout << "All done!\n";

    while (true)
    {
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
