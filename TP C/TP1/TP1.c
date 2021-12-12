#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <time.h>


int exo1FunctCopy(char* a, char* b){

    FILE *fd, *fd2;

    long size;

    if((fd = fopen(a,"r"))==NULL) {
        printf("Error, opening file");
        exit(1);
    }
        fseek(fd, 0, SEEK_END);
        size = ftell(fd);
        fseek(fd, 0, SEEK_SET);
        char buff[size];

        fd2 = fopen(b,"w");
        fgets(buff,size+1,fd);

        int i = 0;
        do{
          fputc(buff[i],fd2);
          i++;

        }while(buff[i]!='\0');
        fputc('\n',fd2);
        fclose(fd);
        fclose(fd2);
    return 0;
}

int exo2ReverseFile(char* a){

    char buff;
    int filesize;
    int i,n;

     int fd = open(a,O_RDWR|O_CREAT);


    if(fd == -1) {
        printf("Error, opening file");
        exit(1);
    }
    filesize = lseek(fd, (off_t) 0, SEEK_END);


    char *extension = strchr(a,'.txt');

    if(extension != NULL){
        for ( i = filesize; i>=0; i--)
        {
            lseek(fd, (off_t) i, SEEK_SET);
            n = read(fd,&buff,1);
            printf("%c",buff);
        }
        write(STDOUT_FILENO, "DONE\n", 5);
        close(fd);
        return 0;
    }
    else{
        printf("Error, not a file txt");
        exit(1);
    }
    
}

int exo3NewLs(char *path){

    DIR *directory;
	struct dirent *ent;
	struct stat _statFileLs;
	char buffer[256];

	directory = opendir(path);

	while((ent = readdir(directory)) != NULL){
		strcpy(buffer, ent->d_name);
		if(stat(buffer, &_statFileLs) < 0){
			return 1;
		}

	printf("\033[0;33m");
	printf("%s  ", buffer);
	printf("\033[0m");

	printf((S_ISDIR(_statFileLs.st_mode)) ? "d" : "-");
	printf((_statFileLs.st_mode & S_IRUSR) ? "r" : "-");
	printf((_statFileLs.st_mode & S_IWUSR) ? "w" : "-");
	printf((_statFileLs.st_mode & S_IXUSR) ? "x" : "-");
	printf((_statFileLs.st_mode & S_IRGRP) ? "r" : "-");
	printf((_statFileLs.st_mode & S_IWGRP) ? "w" : "-");
	printf((_statFileLs.st_mode & S_IXGRP) ? "x" : "-");
	printf((_statFileLs.st_mode & S_IROTH) ? "r" : "-");
	printf((_statFileLs.st_mode & S_IWOTH) ? "w" : "-");
	printf((_statFileLs.st_mode & S_IXOTH) ? "x" : "-");

	printf(" %d : %d - %ld -", _statFileLs.st_uid, _statFileLs.st_gid, _statFileLs.st_size);

	time_t last = time(NULL);
	last = _statFileLs.st_mtime;
	struct tm * now = localtime(&last);
	printf("%4d-%02d-%02d @ %02d:%02d:%02d \n", now->tm_mday, now->tm_mon+1, now->tm_year+1900, now->tm_hour, now->tm_min, now->tm_sec);
    }
    return 0;
}


// Remplacer les commentaires afin d'essayer chaque fonction
int main(int argc, char **argv)
{
    /*
    if(argc<3){
        perror("Vous devez entrer au minimum 2 arguments");
        exit(1);
    }

    int test = exo1FunctCopy(argv[1],argv[2]);
    */
/*
    if (argc < 2)
    {
        perror("Vous devez entrer au minimum 2 arguments");
        exit(1);
    }
    int test = exo2ReverseFile(argv[1]);
    return 0;
*/

if (argc < 2)
    {
        perror("Vous devez entrer au minimum 2 arguments");
        exit(1);
    }
    int test = exo3NewLs(argv[1]);

return 0;
}

