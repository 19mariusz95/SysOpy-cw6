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
    exit(0);
}

void closemsg() {
    if (msgctl(client_queue, IPC_RMID, 0) < 0) {
        perror(NULL);
        exit(4);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("2 arguments expexted\n");
        exit(1);
    }
    int n = atoi(argv[2]);

    server = msgget(ftok(argv[1], n), S_IWUSR | S_IRUSR | S_IWGRP | S_IROTH | S_IWOTH);
    client_queue = msgget(IPC_PRIVATE, S_IWUSR | S_IRUSR | S_IWGRP | S_IROTH | S_IWOTH);

    if (client_queue == -1 || server == -1) {
        perror(NULL);
        exit(2);
    }
    signal(SIGINT, handler);
    atexit(closemsg);
    struct msg message;
    message.mtype = NEWCLNT;
    message.client_id = client_queue;
    int garek = msgsnd(server, &message, sizeof(message), 0);
    if (garek < 0) {
        perror(NULL);
    }
    ssize_t ala = msgrcv(client_queue, &message, sizeof(struct msg), SERACCLIENT, 0);

    if (ala < 0) {
        perror(NULL);
        exit(2);
    }

    int client_id = message.client_id;


    while (flaga) {
        message.mtype = GETNEXT;
        message.client_id = client_id;
        if (msgsnd(server, &message, sizeof(message), 0) < 0) {
            perror(NULL);
            exit(2);
        }
        if (msgrcv(client_queue, &message, sizeof(message), GETNEXT, 0)<0) {
            perror(NULL);
            exit(3);
        }
        message.is_prime = is_prime(message.number);
        message.mtype = CLRESP;
        if (msgsnd(server, &message, sizeof(message), 0) < 0) {
            perror(NULL);
            exit(1);
        }
        sleep(1);
    }
    exit(0);
}
