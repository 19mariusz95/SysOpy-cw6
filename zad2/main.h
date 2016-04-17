//
// Created by student on 12/04/16.
//

#ifndef ZAD1_MAIN_H
#define ZAD1_MAIN_H

int is_prime(int n) {
    if (n == 2 || n == 3 || n == 5)
        return 1;
    if (n < 2 || n % 2 == 0 || n % 3 == 0 || n % 5 == 0)
        return 0;
    for (int i = 7; i * i <= n;) {
        if (n % i == 0)
            return 0;
        i += 4;
        if (n % i == 0)
            return 0;
        i += 2;
    }
    return 1;
}

#define MAX_SIZE 100

#define MAXCLIENTS 10

#define NEWCLNT 1
#define SERACCLIENT 2
#define CLRESP 3
#define GETNEXT 4
#define CLSCLNT 5

#endif //ZAD1_MAIN_H
