#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
printPrime(int *input, int count) {
    if (count <= 0) return;
    printf("prime %d\n", input[0]);
    int p[2];
    pipe(p);
    if (fork() == 0) {
        close(p[0]);
        int *t = input + 1;
        for (int i = 1; i < count; ++i) {
            if (*t % *input != 0) {
                write(p[1], t, sizeof(int));
            }
            ++t;
        }
        close(p[1]);
        exit(0);
    }
    else {
        close(p[1]);
        int *t = input, cnt = 0;
        while (read(p[0], t, sizeof(int)) > 0) {
            ++t;
            ++cnt;
        }
        close(p[0]);
        printPrime(input, cnt);
        exit(0);
    }
}

int
main(int argc, char *argv[])
{
    int input[34], i;
    for (i = 2; i <= 35; ++i) {
        input[i - 2] = i;
    }
    printPrime(input, 34);
    exit(0);
}