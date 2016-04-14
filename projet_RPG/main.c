#include <stdlib.h>
#include <time.h>


void main() {
    /* srand is called here so that the next random
     * number called will actually be random */
    srand(time(0));
    system("cls");
    startMenu();
}
