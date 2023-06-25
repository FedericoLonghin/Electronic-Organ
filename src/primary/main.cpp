#include "common.h"
#include "synth.h"
#include "serial.h"

int sustained = 0;
int sustained_note[20];

fluid_settings_t *settings;
fluid_audio_driver_t *adriver;
fluid_audio_driver_t *adriver2;
fluid_synth_t *synth;
fluid_synth_t *synth2;

using namespace std;

void SIGINT_handler(int s)
{
    cout << "\nClosing Electronic Organ...\n- Empting SerialMessages...";
    while (simpleRead() == 0)
    {
        cout << "*";
    }
    cout << " Done\n- Closing Serial Port...";
    close(serial_port);
    cout << " Done\n- Deleting fluidsynth Instances...";

    delete_fluid_audio_driver(adriver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);

    cout << " Done\n- Sendind exit command...\n";
    exit(0);
}

int main(int argc, char **argv)
{
    cout << "Electronic Organ by Federico Longhin\nStarting sequence:\n";
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = SIGINT_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

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

    sfont_id = fluid_synth_sfload(synth, "../soundfonts/example.sf2", 1);
    // sfont_id = fluid_synth_sfload(synth2, "../soundfonts/example.sf2", 1);
    if (sfont_id == FLUID_FAILED)
    {
        puts("Loading the SoundFont failed!");
    }
    cout << "Done!\n";

    bootSound();
    int a, b, c;
    fluid_synth_program_change(synth, 1, 30);
    fluid_synth_program_change(synth2, 0, 40);

    int t = 0;
    SerialEvent event[EVENT_BUFFER_LENGTH];
    cout<<"All done!\n";
    while (true)
    {

        if (checkForSerial(event[serial_event_in_buffer]))
        {
            // std::cout << (int)event.eventType;
            triggerSynthEvent(event[serial_event_in_buffer]);
        }
        // exit(1);
        // system("stty raw");
        // if (getchar())
        //     return 0;
    }

err:
    /* Clean up */
    delete_fluid_audio_driver(adriver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);

    close(serial_port);

    return 0;
}
