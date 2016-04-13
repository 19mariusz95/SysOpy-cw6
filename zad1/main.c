#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {


    key_t habit = 1;
    int zakonnica = msgget(habit, IPC_CREAT);
    if (zakonnica == -1) {
        printf("error");
        exit(1);
    }

    int kisiel = msgctl(zakonnica, IPC_RMID, NULL);
    if (kisiel == -1) {
        printf("error");
        exit(1);
    }
    return 0;
}