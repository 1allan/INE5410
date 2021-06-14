#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char** argv) {

    for (int i = 0; i < 2; ++i) {
        pid_t process = fork();
        printf("Processo %d, filho de %d\n", getpid(), getppid());


        if (process == 0) {
            for (int j = 0; j < 3; ++j) {
                pid_t grandchild = fork();

                if (grandchild == 0) {
                    printf("Processo %d, filho de %d\n", getpid(), getppid());
                    sleep(5);
                    printf("Processo %d finalizado\n", getpid());
                    exit(0);
                }
            }
            wait(NULL);
            printf("Processo %d finalizado\n", getpid());
            exit(0);
        }
    }    
    wait(NULL);
    printf("Processo principal %d finalizado\n", getpid());    
    return 0;
}
