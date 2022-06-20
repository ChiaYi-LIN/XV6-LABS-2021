#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int
main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs command...\n");
        exit(1);
    }
    
    int i;
    char *xargv[MAXARG];
    for (i = 1; i < argc; ++i) {
        xargv[i - 1] = argv[i];
    }
    
    char buf[512], *arg_start = buf, c = '0';
    int p = 0, xargc = argc - 1;
    while (read(0, &c, 1) == 1) {
        if (c == ' ' || c == '\n') {
            buf[p++] = 0;
            xargv[xargc++] = arg_start;
            arg_start = &buf[p];
            if (c == '\n') {
                xargv[xargc] = 0;
                if (fork() == 0) {
                    exec(xargv[0], xargv);
                }
                else {
                    wait(0);
                }
                xargc = argc - 1;
            }
        } else {
            buf[p++] = c;
        }
    }

    exit(0);
}