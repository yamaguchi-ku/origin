#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/stat.h>

#define STR_MAX 256 //文字列入力用配列長
#define OPT_N "-n" //オプション-n
#define handle_error_en(en, msg) do{errno = en; perror(msg); exit(EXIT_FAILURE);}while(0)

char g_Buff[STR_MAX];
//memset(buff, 0x00, sizeof(buff)); //buff初期化
int g_Line = 0; //行数カウンタ

void* cat_repetition(void* args){
	int ch = 0;
	while((ch = getchar()) != atoi("\n")){
		fputc(ch, stdout);
	}
	pthread_exit(0);
}

void* cat_fileopen (void* fname){
	struct stat st;
	FILE* fp = NULL;
	int ch = 0;
	int result = 0;
	result = stat((char*)fname, &st);// 渡された名前のファイルがあるかの判定
	if(result != 0){
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
	memset(g_Buff, 0x00, sizeof(g_Buff));
	while(1){
		fgets(g_Buff, STR_MAX, stdin);
		g_Line++;
		printf("%6d %s\n", g_Line, g_Buff);
	}
	pthread_exit(0);
}

void* cat_fileopen_number(void* fname){
	memset(g_Buff, 0x00, sizeof(g_Buff));
	FILE* fp = NULL;
	if((fp = fopen(fname, "rb")) == NULL){
		printf("file not found\n");
	}else{
		while(fgets(g_Buff, STR_MAX, fp) != NULL){
			g_Line++;
			printf("%6d %s", g_Line, g_Buff);
		}
		fclose(fp);
	}
	pthread_exit(0);
}

int main(int argc, char *argv[]){
	int i = 0;
	int ret = 0; //pthrad戻り値チェック変数
	pthread_t th;
	//memset(&th, 0x00, sizeof(th));  //th初期化
	if(argc == 1){
		ret = pthread_create(&th, NULL, cat_repetition, (void *) NULL);
		if(ret != 0){
			handle_error_en(ret, "pthread_create");
		}
		ret = pthread_join(th, NULL);
		if(ret != 0){
			handle_error_en(ret, "pthread_jion");
		}
	}
	if(argc > 1){
		//オプション'-n'がcat_threadコマンドの１つ後ろにあるかの判定
		if(strncmp(OPT_N, argv[1], strlen(argv[1])) == 0){
			if(argc == 2){
				ret = pthread_create(&th, NULL, cat_repetition_number, (void*)NULL);
				if(ret != 0){
					handle_error_en(ret, "pthread_create");
				}
				ret = pthread_join(th, NULL);
				if(ret != 0){
					handle_error_en(ret, "pthread_join");
				}
			}
			for(i=2; i<argc; i++){
				ret = pthread_create(&th, NULL, cat_fileopen_number, argv[i]);
				if(ret != 0){
					handle_error_en(ret, "pthread_create");
				}
				ret = pthread_join(th, NULL);
				if(ret != 0){
					handle_error_en(ret, "pthread_join");
				}
			}
		//'-n'なしの場合
		}else{
			for(i=1; i<argc; i++){
				ret = pthread_create(&th, NULL, cat_fileopen , argv[i]);
				if(ret != 0){
					handle_error_en(ret, "pthread_create");
				}

				ret = pthread_join(th, NULL);
				if(ret != 0){
					handle_error_en(ret, "pthread_join");
				}
			}
		}	
	}
	return 0;
}
