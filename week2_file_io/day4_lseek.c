#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int fd = open("text_lseek.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1){
		perror("open");
		exit(1);
	}

	write(fd, "ABCDEFG", 7);

	lseek(fd, 2, SEEK_SET);

	write(fd, "ZZ", 2);

	lseek(fd, 0, SEEK_SET);
	char buf[100] = {0};
	read(fd, buf, sizeof(buf)-1);
	printf("最终文件内容：%s\n", buf);
	close(fd);
	return 0;
}
