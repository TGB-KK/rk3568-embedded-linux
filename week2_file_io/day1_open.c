#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int fd = open("text.txt", O_CREAT | O_RDWR, 0644);
	if(fd == -1){
		perror("open");
		exit(1);
	}
	printf("文件打开成功，文件描述符 fd = %d\n", fd);
	close(fd);
	return 0;
}
