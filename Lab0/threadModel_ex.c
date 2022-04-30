//threadModel_ex.c
//thread example (Refer to the Chp27 in OSTEP)
//by J.choi (choijm@dku.edu)

#include <stdio.h>
#include <stdlib.h>

int a = 10;

void *func() {
	a++;
	printf("pid = %d a = %d\n", getpid(), a);
}

int main() {
	pthread_t p_thread;

	if ((pthread_create(&p_thread, NULL, func, (void*)NULL))<0) {
		// thread를 생성했을 때 리턴값이 음수이면 ERROR이므로 exit()처리 
		exit(0);
	}

	pthread_join(p_thread, (void*)NULL); // wait()와 기능이 유사함
	printf("a = %d by pid = %d\n", a, getpid());

}
