
// ----------      Exercice Ping Pong          -----------------//

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>


void actionFils();
void actionPere(int);
void signalPere(int);
void signalFils(int);


int main()
{
	int filspid = fork();

	if ( filspid == 0 )
	{
		actionFils();
	}
	else
	{
		actionPere(filspid);
	}

	return 0;
}

void actionPere(int filspid)
{
	struct sigaction sa;
	sigset_t sigmask;

	sa.sa_handler = signalPere;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGUSR1, &sa, NULL);

	while(1)
	{
		
		sigfillset(&sigmask);
		sigdelset(&sigmask,SIGUSR1);
		sigdelset(&sigmask,SIGINT);
		sigsuspend(&sigmask);
        
		sleep(1);
		kill(filspid, SIGUSR1);
	}
}

void actionFils()
{
	struct sigaction sa;
	sigset_t sigmask;

	sa.sa_handler = signalFils;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGUSR1, &sa, NULL);

	while(1)
	{
		
		sleep(1);
		kill(getppid(), SIGUSR1);

		
		sigfillset(&sigmask);
		sigdelset(&sigmask,SIGUSR1);
		sigdelset(&sigmask,SIGINT);
		sigsuspend(&sigmask);
	}
}

void signalPere(int signo)
{
	fprintf(stderr, "Ping - \n");
}

void signalFils(int signo)
{
	fprintf(stderr, " -- Pong\n");
}