#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
struct uproc {
    int parentpid;
    int procpid;
    int prio;
    enum procstate state;
    char name [16];
};

int main(int argc, char** argv) {

    struct uproc uprocs [NPROC];

    int count;
    count = getprocs(uprocs);

    if (count < 0) {
        printf(1, "Error getting processes.\n");
        return -1;
    }

    printf(1, "PID\tPPID\tPRIO\tSTATE\t\tNAME\n");

    int i;
    for (i = 0; i < count; i++) {
        struct uproc up = uprocs[i];
        printf(1, "%d\t%d\t%d\t", up.procpid, up.parentpid, up.prio);
        switch (up.state) {
            case UNUSED:
                printf(1, "%s\t\t", "UNUSED"); // technically shouldn't happen since getprocs filters out these
                break;
            case EMBRYO:
                printf(1, "%s\t\t", "EMBRYO");
                break;
            case SLEEPING:
                printf(1, "%s\t", "SLEEPING");
                break;
            case RUNNABLE:
                printf(1, "%s\t", "RUNNABLE");
                break;
            case RUNNING:
                printf(1, "%s\t\t", "RUNNING");
                break;
            case ZOMBIE:
                printf(1, "%s\t\t", "ZOMBIE");
                break;
        }
        printf(1, "%s\n", up.name);
    }

    exit();
}
