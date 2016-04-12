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

#endif //ZAD1_MAIN_H
