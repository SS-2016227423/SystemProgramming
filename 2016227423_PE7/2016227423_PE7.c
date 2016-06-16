
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define oops(m,x) { perror(m) ; exit(x);}

main(int ac, char **av)
{
	int thepipe[2], newfd, pid; // 파이프[2]개 , pid 생성
	
	if ( ac != 4 ){  // 명령어 3개 받지않을경우 에러처리
	fprintf(stderr, "usage : pipe cmd1 cmd2 cmd3 \n");
	exit(1);
	}	

	if ( pipe( thepipe ) == -1) // 파이프 생성
	oops("Cannot get a pipe", 1);

	if ( (pid = fork() ) == -1 ) // pid FORK
	oops("Cannot fork", 2);


	if ( pid > 0 ) { //pid가 자식일때
		int innerpipe[2], innerpid; // Innerpipe와 innerpid 생성
		if (pipe(innerpipe) == -1) // PIPE 생성
		oops("Cannot get a pipe",3);

		if ( (innerpid = fork() ) == -1 ) // INNERPID FORK
		oops("Cannot fork",4);

		if ( innerpid == 0 ) { // innerpid의  자식일때 
			close(thepipe[0]); // 파이프 닫기
			close(thepipe[1]);	
			close(innerpipe[0]);
			if ( dup2(innerpipe[1], 1) == -1 ) // innerpipe[1]에 1을 복제
				oops("could not redirect stdin", 5);
			close(innerpipe[1]); // 파이프 닫기

			execlp( av[1], av[1], NULL); // 받은 명령어중 1번째 실행
			oops(av[1],6);
		}
		if ( innerpid > 0 ) { //innerpid의  부모실행
		close(thepipe[0]);
		close(thepipe[1]);
		close(innerpipe[1]);
		if ( dup2(innerpipe[0], 0 ) == -1 ) // 복제
			oops("could not redirect stdout", 6);

		close(innerpipe[0]);
		execlp( av[2], av[2], NULL); // 받은 명령어중 2번째 실행
		oops(av[2], 7);
		}
	}
	if ( pid == 0 ) { // pid의  부모일때
	close(thepipe[0]);

	if ( dup2(thepipe[1], 1) == -1 ) // 위와 동일
		oops("could not redirect stdout", 7);

	close(thepipe[1]);
	execlp( av[3], av[3], NULL); // 마지막 명령어 실행
	oops(av[3], 8);
	}
}
