#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int count=0;
int main(void)
{
	pid_t pid,pid1,pid2,pid3;
	int i,tmp;
	for ( i=0; i<4; i++) {
		pid = fork();
	}
	if (pid==0){
		printf("child:%d\n",getpid());
		count++;
		//		printf("%d:child\n",count);
		exit(0);

	}
	if (pid != 0) {
		printf("parent:%d\n",getpid());
		count++;
		//		printf("%d:father\n",count);
		//	continue;
	}
	return 0;
}
