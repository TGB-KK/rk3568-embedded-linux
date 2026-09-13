#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int fd = open("text.txt", O_RDONLY);
	if (fd == -1){
		perror("open");
		exit(1);
	}

	char buf[100] = {0};
	ssize_t ret = read(fd, buf, sizeof(buf) - 1);
	if (ret == -1){
		perror("read");
		exit(1);
	}
	buf[ret] = '\0';
	printf("读取到 %ld 字节，内容：%s", ret, buf);
	close(fd);
	return 0;
}
