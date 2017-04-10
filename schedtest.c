#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {

    int np = 12;
    int err;
    int p;
    for (p = 0; p < np; p++) {
        int pid;
        pid = fork();
        if (pid < 0) return -1; // failed to fork
        if (pid == 0) { // child proc
            int realpid = getpid();
            // if we are using priority, set that
            int prio = 0;
            #ifdef PRIO
            prio = (p % 3) * 5;
            err = setprio(prio);
            if (err < 0) exit();
            #endif

            // simulate various work - i/o bound, cpu-intensive, cpu-bursts
            sleep(3);
            int i, j, k;
            for (i = 0; i < 1000; i++) {
                for (j = 0; j < (100 * ((p % 2) + 1)); j++) {
                    for (k = 0; k < 1000; k++) {
                        i = i + j - k - j + k;
                    }
                }
                if (i % 50 == 0) sleep((p % 2) + 2);
            }

            // ok, enough of that.
            int sleept, wait, run;

            err = getpstat(&sleept, &wait, &run);
            if (err != 0) {
                printf(1, "Error getting times.\n");
                return -1;
            }
            printf(1, "test pid %d (prio %d) stat | sleep: %d | wait: %d | run: %d\n",
                    realpid, prio, sleept, wait, run);
            exit();
        }
    }

    for (p = 0; p < np; p++) {
        wait(); // wait for test children
    }

    printf(1, "all test processes finished\n");

    exit();
}
