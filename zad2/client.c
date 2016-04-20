#include <mqueue.h>
#include <printf.h>
#include <stdlib.h>
#include <signal.h>
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <zconf.h>

int server;
int client_queue;
char queue_name[30];

void handler(int sig) {
    exit(0);
}

void clean() {
    char message[MAX_SIZE];
    message[0] = CLSCLNT;
    mq_send(server, message, MAX_SIZE, 0);
    mq_close(server);
    mq_close(client_queue);
    mq_unlink(queue_name);
}

int main(int argc, char *argv[]) {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 1;
    sprintf(queue_name, "/client%d", getpid());
    server = mq_open("/server", O_WRONLY, 0, &attr);
    client_queue = mq_open(queue_name, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR, &attr);

    if (client_queue == -1 || server == -1) {
        perror("client or server queue open");
        exit(2);
    }
    signal(SIGINT, handler);
    char message[MAX_SIZE];
    message[0] = NEWCLNT;
    strcpy(message + 1, queue_name);
    int garek = mq_send(server, message, MAX_SIZE, 0);
    if (garek < 0) {
        perror(NULL);
    }
    ssize_t ala = mq_receive(client_queue, message, MAX_SIZE, 0);
    if (ala < 0) {
        perror(NULL);
        exit(2);
    }
    atexit(clean);

    int client_id;
    sscanf(message + 1, "%d", &client_id);
    printf("client id : %d\n", client_id);
    if (client_id == -1) {
        printf("not registered");
        exit(1);
    }
    printf("registered\n");

    while (1) {
        message[0] = GETNEXT;
        sprintf(message + 1, "%d", client_id);
        mq_send(server, message, MAX_SIZE, 0);
        mq_receive(client_queue, message, MAX_SIZE, 0);
        int tmp;
        sscanf(message + 1, "%d", &tmp);
        printf("%d\n", tmp);
        int resp = is_prime(tmp);
        message[0] = CLRESP;
        sprintf(message + 1, "%d %d %d", client_id, tmp, resp);
        if (mq_send(server, message, MAX_SIZE, 0) < 0) {
            perror(NULL);
        }
        sleep(1);
    }
}
