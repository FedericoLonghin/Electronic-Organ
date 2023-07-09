#include "common.h"
#include "synth.h"
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

void noteOn(int ch, int key)
{
    if (ch == 1)
    {
    std::cout << "NoteOn  ch: " << ch << " key: " << key << " freq: "<<noteFreq[(key-60)%12]<<"\n";
       semo.lock();
        keyPressed[key - 60] = 1;
        numKeyPressed++;
        std::cout <<"KeyPressed: "<<numKeyPressed<<"\n";
       semo.unlock();
    }
    //  if(ch==2)key+=12;
    // fluid_synth_noteon(synth[0], ch, key-12, 80);
    // fluid_synth_noteon(synth2, ch, key, 80);
}

void noteOff(int ch, int key)
{
    if (ch == 1)
    {
    std::cout << "NoteOff ch: " << ch << " key: " << key << "\n";
        keyPressed[key - 60] = 0;
        numKeyPressed--;
        std::cout <<"KeyPressed: "<<numKeyPressed<<"\n";
    }

    //  if(ch==2)key+=12;
    // fluid_synth_noteoff(synth[0], ch, key-12);
    // fluid_synth_noteoff(synth2, ch, key);
}

void bootSound()
{
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
