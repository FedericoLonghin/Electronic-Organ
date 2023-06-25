//#include"common.h"
int setupSerial();
bool checkForSerial(SerialEvent &evt);

SerialEvent parseMessage(char *raw_msg);
