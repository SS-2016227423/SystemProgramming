#include<stdio.h>
#include<signal.h>

main()
{
	void Inter(int);

	signal(SIGINT,Inter);
	while(1)
	{
	sleep(1);
	printf("Hello\n");
	}
}
void Inter(int signum)
{
	char yn;
	printf("Interrupted! OK to quit(y/n)?  ");
	scanf("%c",&yn);
	if(yn == 'y')
	exit(0);	
}
