//OSTEP chap05: Figure 5.4: All of Above with Redirection (p4.c)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	int rc = fork();
	if (rc < 0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) { // child: redirect standard output to a file 
		close(STDOUT_FILENO); // 표준 출력을 닫고 
		open("p4_output.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
		// now exec "wc"...
		char *myargs[3];
		myargs[0] = strdup("wc");
		myargs[1] = strdup("p4.c");
		myargs[2] = NULL;
		execvp(myargs[0], myargs); 
	} else {
		int wc = wait(NULL);
	}
	return 0;
}
