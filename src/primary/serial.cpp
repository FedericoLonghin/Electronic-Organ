#include "common.h"
#include "serial.h"
#include <regex>

char read_buf[256];
int num_bytes;
int serial_port;
bool isRead_buffEmpty = true;
int serial_event_in_buffer = 0;
using namespace std;

bool setupSerial()
{
    serial_event_in_buffer = 0;
    serial_port = open("/dev/ttyACM0", O_RDWR);
    struct termios tty;
    if (tcgetattr(serial_port, &tty) != 0)
    {
        return 0;
    }
    tty.c_cc[VTIME] = 10;
    tty.c_cc[VMIN] = 0;

    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        cout << "Error";
        return false;
    }
    return true;
}

bool checkForSerial(SerialEvent &evt)
{

    //memset(&read_buf, '\0', sizeof(read_buf));

    num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    if (num_bytes > 3)
    {
        // char *ptr = read_buf;
        // char **ptrPtr = &ptr;
        cout << "Parsing: " << read_buf;
        evt = parseMessage(read_buf);
        // evt = newParseMessage(ptrPtr);
        // cout << (int)evt.eventType;
        return 1;
    }
    return 0;
}

// class SerialEvent
// {
// public:
//     char channel;
//     char eventType;
//     char message;
// };

// SerialEvent newParseMessage(char *raw_msg)
// {
//     cout << raw_msg;
//     SerialEvent msg;
//     char *token;
//     // char *str[4];
//     int i = 0;
//     const std::regex rgx("(.*) (\\d*) (\\d*) (\\d*)");
//     std::smatch match;

//     std::string s(raw_msg);

//     if (std::regex_search(s.begin(), s.end(), match, rgx))
//         if (strcmp(match[0], "N-On") == 0)
//             {
//                 msg.eventType = EVENT_CODE_NOTEON;
//             }
//             else if (strcmp(match[0], "N-Off") == 0)
//             {
//                 msg.eventType = EVENT_CODE_NOTEOFF;
//             }
//             msg.channel = atoi(match[1]);
//             msg.message = atoi(match[2]);

//     return msg;
// }

SerialEvent parseMessage(char *raw_msg)
{
    // cout << raw_msg;
    SerialEvent msg;
    char *token;
    // char *str[4];
    int i = 0;
    isRead_buffEmpty = true;
    token = strtok(raw_msg, " ");
    while (token != NULL)
    {
        // str[i] = token;
        if (i == 0)
        {
            if (strcmp(token, "N-On") == 0)
            {
                msg.eventType = EVENT_CODE_NOTEON;
            }
            else if (strcmp(token, "N-Off") == 0)
            {
                msg.eventType = EVENT_CODE_NOTEOFF;
            }
        }
        if (i == 1)
        {
            msg.channel = atoi(token);
        }
        if (i == 2)
        {
            msg.message = atoi(token);
        }
        if(i>=3){
            char* trash=token;
        }

        i++;
        token = strtok(NULL, " ");
    }

    return msg;
}

int simpleRead()
{
    // cout << "Reading from Serial/n";

    //memset(&read_buf, '\0', sizeof(read_buf));

    num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
    //cout << "read num:" << num_bytes;
    //puts(read_buf);
    if (num_bytes!=0)return 0;
    return -1;
}