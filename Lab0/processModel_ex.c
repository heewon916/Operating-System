// processModel_ex.c
// fork example (Refer to the Chp05 in OSTEP)
// by J.Choi (choijm@dku.edu)

#include <stdio.h>
#include <stdlib.h>

int a = 10;

void *func() {
	a++;
	printf("pid = %d a = %d\n", getpid(), a);
}

int main() {
	int pid;
	if ((pid = fork()) == 0) { // need exception handle
		func();
		exit(0);
	}
	wait();
	printf("a = %d by pid = %d\n", a, getpid());
}
