#include "common.h"
#include "terminal.h"
#include <signal.h>
#include "serial.h"
using namespace std;

void SIGINT_handler(int s)
{
    cout << "\nClosing Electronic Organ...\n- Empting SerialMessages...";
    while (simpleRead() > 0)
    {
        cout << "*\n";
    }
    cout << " Done\n- Closing Serial Port...";
    close(serial_port);
    cout << " Done\n- Deleting fluidsynth Instances...";

    delete_fluid_audio_driver(adriver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);

    cout << " Done\n- Sending exit command...\n";
    exit(0);
}

void setupSigaction()
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = SIGINT_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
}