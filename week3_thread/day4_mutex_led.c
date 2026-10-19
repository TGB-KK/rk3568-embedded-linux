#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

#define LED_PATH "/sys/class/leds/work/brightness"

int led_fd;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 线程A：快闪(100ms)
void *thread_fast(void *arg){
	for(int i = 0;i < 10;i++){
		pthread_mutex_lock(&mutex);
		write(led_fd, "1", 1);
		usleep(100000);
		write(led_fd, "0", 1);
		usleep(100000);
		pthread_mutex_unlock(&mutex);
	}
	printf("快闪进程结束\n");
	return NULL;
}

// 线程B：慢闪（500ms）
void *thread_slow(void *arg){
	for(int i = 0;i < 10;i++){
		pthread_mutex_lock(&mutex);
		write(led_fd, "1", 1);
		usleep(500000);
		write(led_fd, "0", 1);
		usleep(500000);
		pthread_mutex_unlock(&mutex);
	}
	printf("慢闪线程结束\n");
	return NULL;
}

int main(){
	led_fd = open(LED_PATH, O_WRONLY);
	if(led_fd == -1){perror("open led");exit(1);}

	pthread_t t1,t2;
	pthread_create(&t1, NULL, thread_fast, NULL);
	pthread_create(&t2, NULL, thread_slow, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	close(led_fd);
	printf("主线程结束\n");
	return 0;
}
