#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]){
	if (argc != 3){
		fprintf(stderr, "用法： %s 源文件 目标文件\n", argv[0]);
		exit(1);
	}

	// 1.打开源文件（只读）
	int fd_src = open(argv[1], O_RDONLY);
	if (fd_src == -1){
		perror("open src");
		exit(1);
	}

	// 2.创建目标文件（读写、不存在则创建、清空已有内容）
	int fd_dst = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd_dst == -1){
		perror("open dst");
		exit(1);
	}

	// 3.循环读写：从源文件读一块 -> 写入目标文件
	char buf[BUFSIZE];
	ssize_t n;
	while((n = read(fd_src, buf, BUFSIZE)) > 0){
		ssize_t ret = write(fd_dst, buf, n);
		if (ret != n){
			perror("write");
			close(fd_src);
			close(fd_dst);
			exit(1);
		}
	}
	if (n == -1){
		perror("read");
	}

	printf("文件拷贝成功！\n");
	close(fd_src);
	close(fd_dst);
	return 0;
}
