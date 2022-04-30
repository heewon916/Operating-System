// OSTEP chap05: Figure 5.1: Calling fork (p1.c)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	printf("hello world (pid:%d)\n", (int)getpid());
	int rc = fork();
	if (rc < 0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	}
	else if(rc == 0) {
		printf("hello, I am child (pid:%d)\n", getpid());
	}
	else{
		printf("hello, I am parent (pid:%d)\n", getpid());
	}
	return 0;
}
