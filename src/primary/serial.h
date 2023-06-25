bool setupSerial();
bool checkForSerial(SerialEvent &evt);

SerialEvent parseMessage(char *raw_msg);
int simpleRead();
