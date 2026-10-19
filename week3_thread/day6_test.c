#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <linux/input.h>

#define LED_PATH "/sys/class/leds/work/brightness"
#define KEY_PATH "/dev/input/event5"

int led_state = 0;
int key_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *led_thread(void *arg){
	int fd = open(LED_PATH, O_WRONLY);
	if(fd == -1){
		perror("open led");
		return NULL;
	}

	while(1){
		pthread_mutex_lock(&mutex);
		led_state = !led_state;
		if(led_state) write(fd, "1", 1);
		else write(fd, "0", 1);
		pthread_mutex_unlock(&mutex);
	}
	close(fd);
	return NULL;
}

void *key_thread(void *arg){
	int fd = open(KEY_PATH, O_RDONLY);
	if(fd == -1){
		perror("open key");
		return NULL;
	}

	struct input_event ev;
	while(1){
		if(read(fd, &ev, sizeof(ev)) > 0){
			if(ev.code == KEY_VOLUMEUP){
				pthread_mutex_lock(&mutex);
				key_count++;
				pthread_mutex_unlock(&mutex);
			}
		}
	}
	close(fd);
	return NULL;
}

void *print_thread(void *arg){
	while(1){
		sleep(2);
		pthread_mutex_lock(&mutex);
		printf("[监控] 当前按键次数: %d, LED状态: %s\n",
			key_count, led_state ? "亮" : "灭");
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main(){
	pthread_t tid_led, tid_key, tid_print;

	printf("系统启动...\n");

	if(pthread_create(&tid_led, NULL, led_thread, NULL) != 0){
		perror("create led thread");
		return 1;
	}
	if(pthread_create(&tid_key, NULL, key_thread, NULL) != 0){
		perror("create key thread");
		return 1;
	}
	if(pthread_create(&tid_print, NULL, print_thread, NULL) != 0){
		perror("create print thread");
		return 1;
	}

	pthread_join(tid_led, NULL);
	pthread_join(tid_key, NULL);
	pthread_join(tid_print, NULL);

	return 0;
}
