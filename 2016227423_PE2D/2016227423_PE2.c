#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>


void do_ls(char[], unsigned int);
void dostat(char *);
void show_file_info( char *, struct stat *);
void dostat2(char *);
void show_file_info2( char *, struct stat *);

int main(int ac, char *av[])
{
	unsigned int lsflag = 0;
	int i, j;
	
	// 인자로 받은 옵션을 확인
	if(ac < 1)
		printf("usage : ls [-ali] directory_name");
	
	i=1;
	while(av[i])
	{
		// ls 옵션에 해당되는 내용이 있으면 플래그 설정
		if(av[i][0] == '-')
		{
			for(j=1; j<strlen(av[i]); j++)
			{
				// 옵션이 중복체크 되지 않도록 함
				switch(av[i][j])
				{
					case 'R':
						if((lsflag & 0x2) == 0)
							lsflag |= 0x2;
						break;
				}
			}
		}
		
		i++;
	}
   if ((lsflag & 0x2) == 0 )
         do_ls( ".",lsflag );
   else
    while ( --ac ){
         printf("%s:\n", *++av );
         do_ls( ".", lsflag );
      }
   return 0;
}

void do_ls( char dirname[],unsigned int lsflag )
{
   DIR      *dir_ptr;      /* the directory */
   struct dirent   *direntp;      /* each entry    */

   if ( ( dir_ptr = opendir( dirname ) ) == NULL )
      printf("cannot open\n");
   else
   {
      while ( ( direntp = readdir( dir_ptr ) ) != NULL ) {
         if (strcmp(direntp->d_name,".")==0 ||strcmp(direntp->d_name,"..")==0) 
			 continue;
         dostat(direntp->d_name);
		 if(direntp->d_type == DT_DIR && lsflag != 0)
		 {
			if (strcmp(direntp->d_name,".")==0 ||strcmp(direntp->d_name,"..")==0) 
			 continue;
			 dostat2(direntp->d_name);
		 }
      }
      closedir(dir_ptr);
   }
}

void dostat( char *filename  )
{
   struct stat info;

   if ( stat(filename, &info) == -1 )      /* cannot stat    */
      perror( filename );         /* say why    */
   else               /* else show info    */
      show_file_info( filename, &info );
}
void show_file_info( char *filename, struct stat *info_p )
{
   printf( "[Directory]%s\n"  , filename );
}

void dostat2( char *filename  )
{
   struct stat info;

   if ( stat(filename, &info) == -1 )      /* cannot stat    */
      perror( filename );         /* say why    */
   else               /* else show info    */
      show_file_info2( filename, &info );
}
void show_file_info2( char *filename, struct stat *info_p )
{
   printf( "[UD]%s\n\n"  , filename );
}
