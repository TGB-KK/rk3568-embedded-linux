#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

#define LED_PATH "/sys/class/leds/work/brightness"
#define KEY_PATH "/dev/input/event5"

int led_fd;
int led_state = 0;	// 0：灭，1：亮
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// 按键线程（生产者）：读取按键事件，改变led_state
void *key_thread(void *arg){
	int fd = open(KEY_PATH,O_RDONLY);
	if(fd == -1){perror("open key");exit(1);}

	struct input_event ev;
	printf("按键线程启动，请按VOL+或VOL-\n");

	while(1){
		if(read(fd, &ev, sizeof(ev)) > 0){
			if(ev.type == EV_KEY && ev.value == 1){
				// 按下事件
				pthread_mutex_lock(&mutex);
				if(ev.code == KEY_VOLUMEUP){
					led_state = 1;	// VOL+ -> 亮
					printf("按键：VOL+ -> LED亮\n");
				}else if(ev.code == KEY_VOLUMEDOWN){
					led_state = 0;	// VOL- -> 灭
					printf("按键：VOL- -> LED灭\n");
				}
				pthread_cond_signal(&cond);	// 通过LED线程
				pthread_mutex_unlock(&mutex);
			}
		}
	}
	close(fd);
	return NULL;
}

// LED线程（消费者）：等待条件变量，根据led_state控制LED
void *led_thread(void *arg){
	printf("LED线程启动\n");
	while(1){
		pthread_mutex_lock(&mutex);
		while(led_state == -1)	pthread_cond_wait(&cond, &mutex);
		int state = led_state;
		pthread_mutex_unlock(&mutex);

		if(state == 1){
			write(led_fd, "1", 1);
			printf("LED亮\n");
		}else{
			write(led_fd, "0", 1);
			printf("LED灭\n");
		}
		// 重置状态，防止重复触发
		pthread_mutex_lock(&mutex);
		led_state = -1;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main(){
	led_fd = open(LED_PATH,O_WRONLY);
	if(led_fd == -1){perror("open led");exit(1);}

	led_state = -1;	// 初始状态 无动作
	
	pthread_t t_key,t_led;
	pthread_create(&t_key, NULL, key_thread, NULL);
	pthread_create(&t_led, NULL, led_thread, NULL);

	pthread_join(t_key, NULL);
	pthread_join(t_led, NULL);

	close(led_fd);
	return 0;
}
