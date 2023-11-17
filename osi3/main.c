#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "d_string.h"
#include "sync_constants.h"
#include <signal.h>

char* ERROR_PTR;

void accept_err(int signum){
    write(STDOUT_FILENO, ERROR_PTR, sizeof(char)*strlen(ERROR_PTR));
}

int loop(char* msg_ptr, pid_t child_pid) {
    errno = 0;
    char* st = NULL;
    int st_len = 0;
    int scan_res;
    do {
        scan_res = scan_string(&st, &st_len);
        st[st_len] = (scan_res == EOF)? EOF : '\n';
        for (int i=0; i <= st_len; ++i) {
        msg_ptr[i] = st[i];
        }
        kill(child_pid, CHILD_SIGNAL_CHECK);
    } while (scan_res != EOF);
    return errno;
}


int main() {
    errno = 0;
    char* filename = NULL;
    int filename_len = 0;
    int file_answers, file_messages, file_errors;
    scan_string(&filename, &filename_len);
    file_answers = creat(filename, S_IRWXU);
    file_messages = open(MESSAGES_FILENAME, O_RDWR|O_CREAT, S_IRWXU);
    file_errors = open(ERRORS_FILENAME, O_RDWR|O_CREAT, S_IRWXU);
    

    ftruncate(file_messages, MESSAGES_FILESIZE);
    ftruncate(file_errors, ERRORS_FILESIZE);
        

    int sync_st = getpid();

    write(file_messages, &sync_st, sizeof(sync_st));

    pid_t id = fork();
    if (id < 0)
        return errno;
    else if (id == 0) {
        dup2(file_answers, STDOUT_FILENO);
        execl("./child", "./child", (char *) NULL);
    }


    signal(PARENT_SIGNAL_CHECK, accept_err);

    char *msg_ptr = mmap(NULL, MESSAGES_FILESIZE*sizeof(char),PROT_WRITE, MAP_SHARED, file_messages, 0);
    if (errno)
        return errno;
    ERROR_PTR = mmap(NULL, ERRORS_FILESIZE*sizeof(char),PROT_READ, MAP_SHARED, file_errors, 0);
    if (errno)
        return errno;
    else if (loop(msg_ptr, id))
        return errno;
    else if (munmap(msg_ptr, MESSAGES_FILESIZE*sizeof(char)) == -1)
        return errno;
    else if (munmap(ERROR_PTR, ERRORS_FILESIZE*sizeof(char)) == -1)
        return errno;
    int status;
    waitpid(id, &status, 0);
    return status;
}
