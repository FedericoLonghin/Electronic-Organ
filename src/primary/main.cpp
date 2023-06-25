#include "common.h"
#include "synth.h"
#include "serial.h"
// #include "SerialEvent.h"

int sustained = 0;
int sustained_note[20];

fluid_settings_t *settings;
fluid_audio_driver_t *adriver;
fluid_audio_driver_t *adriver2;
fluid_synth_t *synth;
fluid_synth_t *synth2;

int main(int argc, char **argv)
{
    // SERIAL
    if (setupSerial() == 0)
        return 1;

    // END SERIAL
    int sfont_id;

    settings = new_fluid_settings();
    fluid_settings_setstr(settings, "audio.driver", "pulseaudio");

    synth = new_fluid_synth(settings);
    synth2 = new_fluid_synth(settings);
    adriver = new_fluid_audio_driver(settings, synth);
    adriver2 = new_fluid_audio_driver(settings, synth2);

    sfont_id = fluid_synth_sfload(synth, "../soundfonts/example.sf2", 1);
    sfont_id = fluid_synth_sfload(synth2, "../soundfonts/example.sf2", 1);
    if (sfont_id == FLUID_FAILED)
    {
        puts("Loading the SoundFont failed!");
        // goto err;
    }

    bootSound();
    int a, b, c;
    fluid_synth_program_change(synth, 1, 30);
    fluid_synth_program_change(synth2, 0, 40);

    int t = 0;
    SerialEvent event;
    event.eventType = 55;
    while (1)
    {
        //std::cout << "Press any key to exit\n";
        //std::cout << t++;
        if (checkForSerial(event))
        {
            //std::cout << (int)event.eventType;
            triggerSynthEvent(event);
        }
        // system("stty raw");
        // if (getchar())
        //     return 0;
        //system("stty cooked");
    }

err:
    /* Clean up */
    delete_fluid_audio_driver(adriver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);

    close(serial_port);

    return 0;
}
