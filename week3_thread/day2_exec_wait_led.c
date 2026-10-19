#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define LED_PATH "/sys/class/leds/work/brightness"

int main(){
	pid_t pid = fork();

	if(pid == -1){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		// 子进程：执行 ls/sys/class/leds/ 查看LED设备
		printf("子进程即将执行 ls/sys/class/leds/\n");
		execlp("ls","ls","/sys/class/leds/",NULL);
		exit(1);
	}else{
		// 父进程：等待子结束后点亮LED
		printf("父进程等待子进程执行完毕...\n");
		int status;
		wait(&status);
		printf("子进程已经结束，父进程点亮LED\n");
		int fd = open(LED_PATH,O_WRONLY);
		if(fd != -1){
			write(fd, "1", 1);	// 点亮
			sleep(3);
			write(fd, "0", 1);	// 熄灭
			close(fd);
		}
	}
	return 0;
}
