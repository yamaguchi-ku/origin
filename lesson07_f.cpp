#include<iostream>
#include<pthread.h>

using namespace std;

void *PrintHelloWorld(void* arg)
{
	cout << "HelloWorld!" << endl;
}

int main(void)
{
	pthread_t thread;
	pthread_create(&thread, NULL, PrintHelloWorld,NULL);
	pthread_join(thread, NULL);
	return 0;
}

