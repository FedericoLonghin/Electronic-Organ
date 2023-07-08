#include "common.h"
#include "terminal.h"
#include <fluidsynth.h>
#include <signal.h>
#include "serial.h"
#include <boost/regex.hpp>

using namespace std;
fluid_sfont_t *sfont = NULL;
// fluid_preset_t preset = NULL;

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
    for (int i = 0; i < FLUIDSYNTH_ISTANCE_NUMBER; i++)
    {
        delete_fluid_audio_driver(adriver[i]);
        delete_fluid_synth(synth[i]);
    }
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

void consoleIn()
{
    string linearr[10];
    string line, word;
    int wordCount;
    while (1)
    {

        wordCount = 0;
        getline(cin, line);
        stringstream lineStream(line);
        while (lineStream >> linearr[wordCount])
            ++wordCount;

        if (line.length() > 0)
        {
            // cout << "Typed: " << line << "| " << wordCount << " words\n";
            // parseCommand(txt);
            switch (getCommand(linearr[0]))
            {
            case COMMAND_EXIT:
                SIGINT_handler(0);
                break;
            case COMMAND_LOAD:
                if (wordCount == 2)
                {
                    char path[100];
                    strcpy(path, sf_path);
                    strcat(path, linearr[1].c_str());
                    cout << fluid_synth_sfload(synth[0], path, 1);
                    cout << "Done.\n";
                }
                else
                {
                    cout << "Incorrect param number; Retry.\n";
                }

                break;
            case COMMAND_ADD:
                if (wordCount == 2)
                {
                    char path[100];
                    strcpy(path, sf_path);
                    strcat(path, linearr[1].c_str());
                    // cout << fluid_synth_add_sfont(synth[0], path, 1);
                    cout << "Done.\n";
                }
                else
                {
                    cout << "Incorrect param number; Retry.\n";
                }

                break;
            case COMMAND_GET_SOUNDFONT_LIST:
                cout << "Soundfonts currently loaded:\n";
                cout << fluid_synth_sfcount(synth[0]);

                sfont = fluid_synth_get_sfont_by_id(synth[0], 1);

                inspectsoundfont(stoi(linearr[1]));
                if (sfont)
                {
                    //    fluid_preset_t preset= fluid_sfont_get_preset(sfont,0,0);
                    // fluid_preset_t preset;
                    //     sfont->iteration_start(sfont);
                    //     while (sfont->iteration_next(sfont, &preset))
                    //     {
                    //         int bank = preset.get_banknum(&preset);
                    //         int prog = preset.get_num(&preset);
                    //         char *name = preset.get_name(&preset);
                    //         printf("bank: %d prog: %d name: %s\n", bank, prog, name);
                    //     }
                }

                // cout << fluid_sfont_get_name();
                break;
            case COMMAND_SET_LEGATO:
            int *leg;
                cout << "Setting Legato:\n";
                cout << fluid_synth_get_legato_mode(synth[0], 1,leg);
                cout<<leg;
                cout << fluid_synth_set_legato_mode(synth[0], 2, 1);
                cout << fluid_synth_get_legato_mode(synth[0], 1,leg);
                cout<<leg;
                break;
            default:
                cout << "Unknown command, Please retry.\n";
                break;
            }
            // cout << "command: " << getCommand(linearr[0]) << "\n";
        }
    }
}

int getCommand(string cmd_txt)
{
    if (cmd_txt.compare("load") == 0 || cmd_txt.compare("Load") == 0)
        return COMMAND_LOAD;
    else if (cmd_txt.compare("add") == 0)
        return COMMAND_ADD;
    else if (cmd_txt.compare("exit") == 0)
        return COMMAND_EXIT;
    else if (cmd_txt.compare("sf_list") == 0)
        return COMMAND_GET_SOUNDFONT_LIST;
    else if (cmd_txt.compare("set_legato") == 0)
        return COMMAND_SET_LEGATO;
    return -1;
}

void parseCommand(string raw)
{
    // ConsoleCommand *cmd = new ConsoleCommand;

    // cout << "Parsing: " << raw << "\n";

    // // boost::regex re(",|:|-|\\s+");                                  // Create the reg exp
    // boost::regex re("(\\w+)\\s+");                                  // Create the reg exp
    // boost::sregex_token_iterator p(raw.begin(), raw.end(), re, -1); // Create an iterator using a sequence and that reg exp
    // boost::sregex_token_iterator end;                               // Create an end-of-reg-exp

    // while (p != end)
    //     std::cout << *p++ << '\n';
}

static void inspectsoundfont(int n)
{
    fluid_sfont_t *sfont = fluid_synth_get_sfont_by_id(synth[0], n);
    for (int bank = 0; bank < 16384; bank++)
    {
        for (int num = 0; num < 128; num++)
        {
            fluid_preset_t *preset = fluid_sfont_get_preset(sfont, bank, num);
            if (preset == nullptr)
                continue;
            const char *name = fluid_preset_get_name(preset);
            std::cout << "bank: " << bank << " index: " << num << " " << name << std::endl;
        }
    }
}