#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

volatile sig_atomic_t terminado = 0;

void handle_signal(int signo, siginfo_t *sinfo, void *context) {
	terminado = 1;
}

int main(int argc, char* argv[]) {

    struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handle_signal;
	sigaction(SIGUSR1, &act, NULL);

    pid_t p = fork();
    if (p>0){
        for(int i = 1; i < 4; i++) {
            printf("%d seconds passed\n", i);
			sleep(1);
		} 
        printf("Tarefe A terminada!\n");
		kill(p, SIGUSR1);
		wait(NULL);
    } else if (p == 0){
		int r = (rand() % 5) + 1;
		sleep(r); 
        printf("Tarefe B terminada!\n");

		while(terminado == 0) {
			pause();
        }

        printf("Tarefe C terminada!\n");
		exit(0);
    } else {
        exit(1);
    }

    return 0;
}