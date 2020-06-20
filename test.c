#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    while (1) {
        printf("Hello, world!");
        fflush(stdout);
        sleep(1);
    }
}
