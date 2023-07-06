#include "common.h"
#include "terminal.h"
#include <signal.h>
#include "serial.h"
#include <boost/regex.hpp>
#include <fluidsynth.h>

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
                    strcpy(path,sf_path);
                    strcat(path,linearr[1].c_str());
                    cout << fluid_synth_sfload(synth, path , 1);
                    cout<<"Done.\n";
                }
                else{
                    cout<< "Incorrect param number; Retry.\n";
                }

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
    else if (cmd_txt.compare("exit") == 0)
        return COMMAND_EXIT;
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
