#include <stdlib.h>
#include <sys/msg.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <zconf.h>
#include "main.h"

int server;
int client_queue;
int flaga = 1;

void handler(int sig) {
    flaga = 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("trolololo");
        exit(1);
    }
    int n = atoi(argv[1]);

    server = msgget(ftok(argv[1], n), S_IWUSR | S_IRUSR | S_IWGRP | S_IROTH | S_IWOTH);
    client_queue = msgget(IPC_PRIVATE, S_IWUSR | S_IRUSR | S_IWGRP | S_IROTH | S_IWOTH);

    if (client_queue == -1) {
        printf("error");
        exit(2);
    }
    signal(SIGINT, handler);
    struct msg message;
    message.mtype = NEWCLNT;
    message.client_id = client_queue;
    int garek = msgsnd(server, &message, sizeof(message), 0);
    if (garek < 0) {
        printf("error\n");
        perror("");
    }
    int ala = msgrcv(client_queue, &message, sizeof(struct msg), SERACCLIENT, 0);

    if (ala < 0) {
        printf("error\n");
        exit(2);
    }

    int client_id = message.client_id;


    while (flaga) {
        message.mtype = GETNEXT;
        message.client_id = client_id;
        msgsnd(server, &message, sizeof(message), 0);
        msgrcv(client_queue, &message, sizeof(message), GETNEXT, 0);
        printf("%d\n", message.number);
        message.is_prime = is_prime(message.number);
        message.mtype = CLRESP;
        msgsnd(server, &message, sizeof(message), 0);
        sleep(1);
    }
    msgctl(client_queue, IPC_RMID, 0);
    exit(0);
}
