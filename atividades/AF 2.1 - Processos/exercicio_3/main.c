#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


void grep() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("grep PID %d iniciado\n", getpid());
        fflush(stdout);
        execlp("grep", "grep", "adamantium", "text", NULL);
        exit(0);
    }
}

void sed() {
    pid_t pid = fork();
    
    if (pid == 0) {
        printf("sed PID %d iniciado\n", getpid());
        fflush(stdout);
        execlp("sed", "sed", "-i", "-e", "s/silver/axamantium/g;s/adamantium/silver/g;s/axamantium/adamantium/g", "text", NULL);
        exit(0);
    }
}

int main(int argc, char** argv) {
    printf("Processo pai iniciado\n");
 
    sed();
    wait(NULL);

    grep();
    int grep_exit;
    wait(&grep_exit);
    int found = WEXITSTATUS(grep_exit);

    printf("grep retornou com código %d,%s encontrou adamantium\n", found, 
    (found ? "" : " não"));

    return 0;
}
