#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define LED_PATH "/sys/class/leds/work/brightness"

int main() {
	printf("父进程开始,PID = %d\n", getpid());

	pid_t pid = fork();	// 创建子进程
	
	if(pid == -1){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		// 子进程：每秒翻转一次LED
		int fd = open(LED_PATH, O_WRONLY);
		if(fd == -1){perror("open LED"); exit(1);}

		printf("子进程开始控制LED闪烁\n");
		for(int i = 0;i < 10;i ++){
			write(fd, "1", 1);	// 亮
			sleep(1);
			write(fd, "0", 1);	// 灭
			sleep(1);
		}
		close(fd);
		printf("子进程结束\n");
	}else{
		// 父进程：打印状态
		printf("父进程创建了子进程，子进程PID = %d\n", pid);
		for(int i = 0;i < 10;i ++){
			printf("父进程运行中...(%d)\n",i);
			sleep(2);
		}
		printf("父进程结束\n");
	}
	return 0;
}
