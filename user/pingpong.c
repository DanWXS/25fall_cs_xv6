#include "kernel/types.h"
#include "user.h"

int main(int argc, char* argv[]) {
    int c2p[2], p2c[2];
    int parent_pid, child_pid;
    pipe(c2p);
    pipe(p2c);

    if (fork() == 0) {
        int c_pid = getpid();
        close(p2c[1]); 
        close(c2p[0]); 

        read(p2c[0], &parent_pid, sizeof(parent_pid));
        write(c2p[1], &c_pid, sizeof(c_pid));
        printf("%d: received ping from pid %d\n", getpid(), parent_pid);

        close(p2c[0]);
        close(c2p[1]);
        exit(0);
    } 
    else {
        int p_pid = getpid();
        close(p2c[0]); 
        close(c2p[1]); 

        write(p2c[1], &p_pid, sizeof(p_pid));
        read(c2p[0], &child_pid, sizeof(child_pid));
        wait(0);
        printf("%d: received pong from pid %d\n", getpid(), child_pid);

        close(p2c[1]);
        close(c2p[0]);
        
    }
    exit(0);
}
