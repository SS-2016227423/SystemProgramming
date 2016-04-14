#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int is_real_dir(const char *path)
{
	struct stat stats;
	if (lstat(path, &stats) == 0 && S_ISDIR(stats.st_mode))
	{
		return 0;
	}
	else
		return 1;
	//return lstat(path, &stats) == 0 && S_ISDIR(stats.st_mode);
}
int file_del(char *file_name)
{
	// 파일 삭제
	int unlink_retval = unlink(file_name); // 원본파일 제거하여 반환값을 넘겨준다.
	if (unlink_retval != 0)// 파일을 올바르게 제거하였는지 검사한다.
	{
		return 1; // 실패했다면 반환값 1을 넘겨준다.
	}
	return 0; // 성공했다면 반환값 0을 넘겨준다.
}


int main(int argc, char *argv[])
{
	char buffer[1024];
	FILE* fdin;
	FILE* fdout;
	int nread, i;
	int file_del_retval;
	int flag;
	
	flag = is_real_dir(argv[2]);
	// 원본 읽기모드로 오픈
	fdin = fopen(argv[1], "rb");
	if (fdin == NULL)	// 원본 파일을 여는 데 실패했다면 에러메시지 출력
	{
		printf("fopen() failed, %s not found \n", argv[1]);
		exit(1);
	}

	if (flag == 1) // 2번째 명령어가 디렉토리가 아니면 이름 바꾸기 
	{
		// 타겟 쓰기모드로 오픈
		fdout = fopen(argv[2], "wb");
		if (fdout == NULL)	// 파일을 여는 데 실패했다면 에러메시지 출력
		{
			printf("fopen() filed, %s not fount \n", argv[2]);
			exit(1);
		}

		// 원본 복사
		// 정상적으로 읽어 들인 내용이 1바이트 이상인 동안 반복문 수행 
		while ((nread = fread(buffer, 1, 1, fdin)) > 0)
		{
			if (fwrite(buffer, nread, 1, fdout) < nread) // 오류시 파일 닫음
			{
				fclose(fdin);
				fclose(fdout);
			}
		}
		// 정상수행시 닫음
		fclose(fdin);
		fclose(fdout);

		file_del_retval = file_del(argv[1]); // file_del()함수에서 넘긴 반환값을 대입한다.
		if (0 != file_del_retval) { // 파일을 제거하지 못했다면 에러메시지 출력
			printf("file (%s) delete fail. \n", argv[1]);
		}
	}
	else
	{

		chdir(argv[2]);
		fdout = fopen(argv[1], "wb");
		if (fdout == NULL)	// 파일을 여는 데 실패했다면 에러메시지 출력
		{
			printf("fopen() filed, %s not fount \n", argv[1]);
			exit(1);
		}
		while ((nread = fread(buffer, 1, 1, fdin)) > 0)
		{
			if (fwrite(buffer, nread, 1, fdout) < nread) // 오류시 파일 닫음
			{
				fclose(fdin);
				fclose(fdout);
			}
		}
		// 정상수행시 닫음
		fclose(fdin);
		fclose(fdout);
		chdir("..");
		file_del_retval = file_del(argv[1]); // file_del()함수에서 넘긴 반환값을 대입한다.
		if (0 != file_del_retval) { // 파일을 제거하지 못했다면 에러메시지 출력
			printf("file (%s) delete fail. \n", argv[1]);
		}
	}
	return 0;
}
