void SIGINT_handler(int s);
void setupSigaction();
void consoleIn();
void parseCommand(std::string raw);
int getCommand(std::string cmd_txt);
static void inspectsoundfont(int n);
