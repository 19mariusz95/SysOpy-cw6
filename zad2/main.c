#include <mqueue.h>
#include <stdlib.h>
#include <signal.h>
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>

int zakonnica;
static int clients[MAXCLIENTS];


void init_clients();

int get_id();

void handler(int sig) {
    exit(0);
}

void clean() {
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (clients[i] != -1) {
            mq_close(clients[i]);
        }
    }
    mq_close(zakonnica);
    mq_unlink("/server");
}

int main(int argc, char *argv[]) {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 1;
    char message[MAX_SIZE + 1];
    zakonnica = mq_open("/server", O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR, &attr);
    if (zakonnica == -1) {
        perror("open queue");
        exit(1);
    }

    signal(SIGINT, handler);
    srand(time(NULL));
    atexit(clean);
    init_clients();

    while (1) {
        if (mq_receive(zakonnica, message, MAX_SIZE, 0) >= 0) {
            long type = message[0];
            switch (type) {
                case NEWCLNT: {
                    int new_id = get_id();
                    if (new_id != -1) {
                        int cl_q = mq_open(message + 1, O_WRONLY, 0, &attr);
                        clients[new_id] = cl_q;
                        message[0] = SERACCLIENT;
                        sprintf(message + 1, "%d", new_id);
                        if (mq_send(cl_q, message, MAX_SIZE, 0)) {
                            perror("send");
                            exit(2);
                        }
                    }
                    break;
                }
                case CLRESP: {
                    int clientid;
                    int number;
                    int isprime;
                    sscanf(message + 1, "%d %d %d", &clientid, &number, &isprime);
                    if (isprime) {
                        printf("Liczba pierwsza: %d klient %d\n", number, clientid);
                    }
                    break;
                }
                case GETNEXT: {
                    int tmp = rand() % 100;
                    int cl_q;
                    sscanf(message + 1, "%d", &cl_q);
                    sprintf(message + 1, "%d", tmp);
                    if (mq_send(clients[cl_q], message, MAX_SIZE, 0) < 0) {
                        perror(NULL);
                    }
                    break;
                }
                case CLSCLNT: {
                    int client_id;
                    sscanf(message + 1, "%d", &client_id);
                    mq_close(clients[client_id]);
                    clients[client_id] = -1;
                    break;
                }
                default:
                    break;
            }
        }
    }
}

int get_id() {
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (clients[i] == -1)
            return i;
    }
    return -1;
}

void init_clients() {
    for (int i = 0; i < MAXCLIENTS; i++) {
        clients[i] = -1;
    }
}