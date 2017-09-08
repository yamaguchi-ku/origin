#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define STR_MAX 256 //文字列入力用配列長
#define OPT_N "-n" //オプション-n
#define handle_error_en(en, msg) do{errno = en; perror(msg); exit(EXIT_FAILURE);}while(0)

char buff[STR_MAX];
//memset(buff, 0x00, sizeof(buff)); //buff初期化
int line = 0; //行数カウンタ

void* cat_repetition(void* args){
	int ch;
	while((ch = getchar()) != atoi("\n")){
		fputc(ch, stdout);
	}
	pthread_exit(0);
}

void* cat_fileopen (void* fname){
	FILE* fp;
	int ch;
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

void* cat_repetition_number(void* args){
	memset(buff, 0x00, sizeof(buff));
	while(1){
		fgets(buff, STR_MAX, stdin);
		line++;
		printf("%6d %s\n", line, buff);
	}
	pthread_exit(0);
}

void* cat_fileopen_number(void* fname){
	memset(buff, 0x00, sizeof(buff));
	FILE* fp;
	if((fp = fopen(fname, "rb")) == NULL){
		printf("file not found\n");
	}else{
		while(fgets(buff, STR_MAX, fp) != NULL){
			line++;
			printf("%6d %s", line, buff);
		}
		fclose(fp);
	}
	pthread_exit(0);
}

int main(int argc, char *argv[]){
	int i, s;
	pthread_t th;
	memset(&th, 0x00, sizeof(th));  //th初期化
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
		//オプション'-n'がcat_threadコマンドの１つ後ろにあるかの判定
		if(strncmp(OPT_N, argv[1], strlen(argv[1])) == 0){
			if(argc == 2){
				s = pthread_create(&th, NULL, cat_repetition_number, (void*)NULL);
				if(s != 0){
					handle_error_en(s, "pthread_create");
				}
				s = pthread_join(th, NULL);
				if(s != 0){
					handle_error_en(s, "pthread_join");
				}
			}
			for(i=2; i<argc; i++){
				s = pthread_create(&th, NULL, cat_fileopen_number, argv[i]);
				if(s != 0){
					handle_error_en(s, "pthread_create");
				}
				s = pthread_join(th, NULL);
				if(s != 0){
					handle_error_en(s, "pthread_join");
				}
			}
		//'-n'なしの場合
		}else{
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
	}
	return 0;
}
