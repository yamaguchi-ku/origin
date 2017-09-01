#include <stdio.h>
#include <pthread.h>

void* thread(void* args){
	printf("Hello World!\n");
	return NULL;
}

int main(void){
	pthread_t th;

	pthread_create(&th, NULL, thread, (void *)NULL);

	pthread_join(th, NULL);

	return 0;
}
