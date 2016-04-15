#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <time.h>
#include "main.h"

int zakonnica;
int flaga = 1;
static int clients[MAXCLIENTS];
static int new_id = 0;


void handler(int sig) {
    flaga = 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("trolololo");
        exit(1);
    }
    key_t habit = ftok(argv[1], atoi(argv[2]));
    zakonnica = msgget(habit, IPC_CREAT | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
                              | S_IWOTH);
    if (zakonnica == -1) {
        printf("error");
        exit(1);
    }

    signal(SIGINT, handler);
    srand(time(NULL));

    while (flaga) {
        struct msg message;
        if (msgrcv(zakonnica, &message, sizeof(message), 0, IPC_NOWAIT) >= 0) {
            long type = message.mtype;
            switch (type) {
                case NEWCLNT: {
                    if (new_id < MAXCLIENTS) {
                        int cl = message.client_id;
                        clients[new_id] = message.client_id;
                        message.mtype = SERACCLIENT;
                        message.client_id = new_id;
                        if (msgsnd(cl, &message, sizeof(message), 0)) {
                            perror(NULL);
                            exit(2);
                        }
                        new_id++;
                    }
                    break;
                }
                case CLRESP: {
                    if (message.is_prime) {
                        printf("Liczba pierwsza: %d klient %d\n", message.number, message.client_id);
                    }
                    break;
                }
                case GETNEXT: {
                    message.number = rand() % 10000;
                    msgsnd(clients[message.client_id], &message, sizeof(message), 0);
                    break;
                }
                default:
                    break;
            }
        }
    }
    int kisiel = msgctl(zakonnica, IPC_RMID, NULL);
    if (kisiel == -1) {
        printf("error");
        exit(1);
    }
}