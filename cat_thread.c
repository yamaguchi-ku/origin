#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define handle_error_en(en, msg) do{errno = en; perror(msg); exit(EXIT_FAILURE);}while(0)

void* cat_repetition(void* args){
	int ch;
	while((ch = getchar()) != atoi("\n")){
		fputc(ch, stdout);
	}
	pthread_exit(0);
}

void* cat_fileopen (void *fname){
	FILE *fp;
	int ch;

	//printf("%s\n", (char*)fname);

	if((fp = fopen((char*)fname, "rb")) == NULL){
		printf("file not found\n");
	}else{
		fp = fopen((char*)fname, "rb");
		while((ch = fgetc(fp)) != EOF){
			fputc(ch, stdout);
		}
	fclose(fp);
	}
	pthread_exit(0);
}

int main(int argc, char *argv[]){
	int i, s;
	pthread_t th;
	if(argc == 1){
		s = pthread_create(&th, NULL, cat_repetition, (void *) NULL);
		if(s != 0){
			handle_error_en(s, "pthread_create");
		}
		s = pthread_join(th, NULL);
		if(s != 0){
			handle_error_en(s, "pthread_jion");
		}
	}
	if(argc > 1){
		for(i=1; i<argc; i++){
			s = pthread_create(&th, NULL, cat_fileopen , argv[i]);
			if(s != 0){
				handle_error_en(s, "pthread_create");
			}

			s = pthread_join(th, NULL);
			if(s != 0){
				handle_error_en(s, "pthread_join");
			}
		}	
	}
	return 0;
}
