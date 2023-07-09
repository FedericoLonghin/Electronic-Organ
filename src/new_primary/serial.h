bool setupSerial();
int checkForSerial(SerialEvent *evt, int num);

int parseMessage(char *raw_msg, SerialEvent *evt, int num);
int simpleRead();
