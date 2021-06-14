#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char** argv) {

	for (int i = 0; i < 2; ++i) {
		pid_t pid = fork();
		
		if (pid > 0) {
			printf("Processo pai criou %d\n", pid);
		} else if (pid == 0) {
			printf("Processo filho %d criado\n", getpid());
			exit(0);
		} else {
			printf("Não rolou :(\n");
		}
	}

	wait(NULL);
	printf("Processo pai finalizado!\n");   
	return 0;
}


