#include "common.h"
#include "serial.h"
#include <regex>

char read_buf[256];
int num_bytes;
int serial_port;
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

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;

    //----------------
    tty.c_cc[VTIME] = 0.1;
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

    // memset(&read_buf, '\0', sizeof(read_buf));
    if (serial_event_in_buffer == 0)
    {
        num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

        if (num_bytes > 0)
        {
            // char *ptr = read_buf;
            // char **ptrPtr = &ptr;
            cout << "Parsing: " << read_buf;
            evt = parseMessage(read_buf);
            // evt = newParseMessage(ptrPtr);
            // cout << (int)evt.eventType;
            return 1;
        }
    }

    return 0;
}

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
        if (i >= 3)
        {
            char *trash = token;
        }

        i++;
        token = strtok(NULL, " ");
    }

    return msg;
}

int simpleRead()
{

    // memset(&read_buf, '\0', sizeof(read_buf));

    num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
    if (num_bytes > 0)
    {
        puts(read_buf);
        return num_bytes;
    }
    return 0;
}

// class SerialEventQuee
// {
//     SerialEvent quee[];
// public:
//     SerialEventQuee(int len)
//     {
//     }
//     int len;
// };
