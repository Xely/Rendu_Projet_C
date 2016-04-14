#include <stdlib.h>

/* Returns the greater of two numbers */
int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

/* Returns the lower of two numbers */
int min(int a, int b) {
    if (a > b) {
        return b;
    } else {
        return a;
    }
}

/* Returns a random integer between StartValue and endValue */
int doRand(int startValue, int endValue) {
    return (rand() % (endValue - startValue + 1) + startValue);
}

/* Returns a random integer between  and diceMax */
int dice(int diceMax) {
    int n = doRand(1, diceMax);
    return n;
}
