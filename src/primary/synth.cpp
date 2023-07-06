#include "common.h"
#include "synth.h"
#include <fluidsynth.h>

void noteOn(int ch, int key)
{
     if(ch==2)key+=12;
    fluid_synth_noteon(synth, ch, key-12, 80);
    // fluid_synth_noteon(synth2, ch, key, 80);
}

void noteOff(int ch, int key)
{
     if(ch==2)key+=12;
    fluid_synth_noteoff(synth, ch, key-12);
    // fluid_synth_noteoff(synth2, ch, key);
}

void bootSound()
{
    fluid_synth_noteon(synth, 0, 84, 80);
    usleep(50000);
    fluid_synth_noteon(synth, 0, 88, 80);
    usleep(50000);
    fluid_synth_noteon(synth, 0, 91, 80);
    usleep(200000);
    fluid_synth_noteoff(synth, 0, 84);
    fluid_synth_noteoff(synth, 0, 88);
    fluid_synth_noteoff(synth, 0, 91);
}

void triggerSynthEvent(SerialEvent event)
{
    switch (event.eventType)
    {
    case EVENT_CODE_NOTEON:
        // std::cout << "Triggering NoteOn \t" << (int)event.channel << "\t" << (int)event.message << "\n";
        noteOn(event.channel, event.message);
        break;
    case EVENT_CODE_NOTEOFF:
        // std::cout << "Triggering NoteOff \t" << (int)event.channel << "\t" << (int)event.message << "\n";
        noteOff(event.channel, event.message);
        break;
    default:
        // std::cout << "Err - EventType not recognized. String:["<<event.eventType<<" "<<event.channel<<" "<<event.message<<"]\n";
    break;
    }
}
