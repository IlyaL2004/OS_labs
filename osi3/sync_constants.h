
#include <signal.h>
const char MESSAGES_FILENAME[] = "messages";
const char ERRORS_FILENAME[] = "errors";
const int CHILD_SIGNAL_CHECK = SIGUSR1;
const int PARENT_SIGNAL_CHECK = SIGUSR1;
const int MESSAGES_FILESIZE = 1024;
const int ERRORS_FILESIZE = 128;

