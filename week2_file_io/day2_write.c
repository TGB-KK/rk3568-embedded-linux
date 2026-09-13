#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int fd = open("text.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1){
		perror("open");
		exit(1);
	}
	char *buf = "Hallo Linux!\n";
	ssize_t ret = write(fd, buf, strlen(buf));
	if (ret == -1){
		perror("write");
		close(fd);
		exit(1);
	}
	printf("成功写入 %ld 字节\n", ret);
	close(fd);
	return 0;
}
