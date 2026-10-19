#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

#define LED_PATH "/sys/class/leds/work/brightness"

// 线程执行函数：控制LED闪烁
void *led_thread(void *arg){
	int fd = open(LED_PATH, O_WRONLY);
	if(fd == -1){
		perror("open led");
		exit(1);
	}
	printf("LED线程启动\n");
	for(int i = 0;i < 10;i++){
		write(fd, "1", 1);
		usleep(300000);	// 300ms
		write(fd, "0", 1);
		usleep(300000);
	}
	close(fd);
	printf("LED线程结束\n");
	return NULL;
}

int main(){
	pthread_t tid;

	// 创建线性控制LED
	int ret = pthread_create(&tid, NULL, led_thread, NULL);
	if(ret == -1){
		perror("pthread_create");
		exit(1);
	}

	// 主线程打印状态
	for(int i = 0;i < 10;i++){
		printf("主线程运行中...(%d)\n",i);
		sleep(1);
	}

	pthread_join(tid, NULL);	// 等待LED线程结束
	printf("主线程结束\n");
	return 0;
}
