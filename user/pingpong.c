#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p2c[2], c2p[2];
    char *buf = "m";
    int len = sizeof(buf);
    pipe(p2c);
    pipe(c2p);
    
    if (fork() == 0) {
        close(p2c[1]);
        close(c2p[0]);

        if (read(p2c[0], buf, len) != len) {
            fprintf(2, "child read error\n");
            exit(1);
        }
        fprintf(1, "%d: received ping\n", getpid());

        if (write(c2p[1], buf, len) != len) {
            fprintf(2, "child write error\n");
            exit(1);
        }
        close(c2p[1]);

    } else {
        close(p2c[0]);
        close(c2p[1]);

        if (write(p2c[1], buf, len) != len) {
            fprintf(2, "parent write error\n");
            exit(1);
        }
        close(p2c[1]);

        if (read(c2p[0], buf, len) != len) {
            fprintf(2, "parent read error\n");
            exit(1);
        }

        fprintf(1, "%d: received pong\n", getpid());
    }
    
    exit(0);
}