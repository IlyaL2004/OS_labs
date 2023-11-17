#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include "sync_constants.h"


int SYNC_ST;

char* MESSAGE_PTR;
char* ERROR_PTR;


int LEN_READ_ST = 0;
int MAX_READ_LEN = 1;
char* READ_ST = NULL;



void read_chunk(int signum) {
    if (READ_ST == NULL)
        READ_ST = malloc(MAX_READ_LEN*sizeof(char));
    int i=0;
    for (; MESSAGE_PTR[i] != '\n' && MESSAGE_PTR[i] != EOF; ++i) {
        READ_ST[LEN_READ_ST++] = MESSAGE_PTR[i];
        if (LEN_READ_ST >= MAX_READ_LEN) {
            MAX_READ_LEN *= 2;
            READ_ST = realloc(READ_ST, MAX_READ_LEN*sizeof(char));
        }
    }
    char last = MESSAGE_PTR[i];
    if ((last == '\n' || last == EOF) && i != 0) {
        if ('A' <= READ_ST[0] && READ_ST[0] <= 'Z') {
            write(STDOUT_FILENO, READ_ST, LEN_READ_ST * sizeof(char));
        } else {
            strcpy(ERROR_PTR, "invalid string\n");
            kill(SYNC_ST, PARENT_SIGNAL_CHECK);
        }
        LEN_READ_ST = 0;
    }
    if (last == EOF)
        exit(0);

}

int main() {
    errno = 0;
    int file_messages, file_errors;
    file_messages = open(MESSAGES_FILENAME, O_RDWR);
    file_errors = open(ERRORS_FILENAME, O_RDWR);
    read(file_messages, &SYNC_ST, sizeof(SYNC_ST));
    MESSAGE_PTR = mmap(NULL, MESSAGES_FILESIZE*sizeof(char),PROT_READ, MAP_SHARED, file_messages, 0);
    if (errno)
        return errno;
    ERROR_PTR = mmap(NULL, ERRORS_FILESIZE*sizeof(char), PROT_WRITE, MAP_SHARED, file_errors, 0);
    if (errno)
        return errno;
    signal(CHILD_SIGNAL_CHECK, read_chunk);    
    while (true);
}
