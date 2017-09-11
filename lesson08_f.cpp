#include<iostream>
#include<pthread.h>
#include<errno.h>
#include<string.h>
#include<fstream>
#include<stdlib.h>

#define error(en, msg)\
		do {errno=en;\
			perror(msg);\
			exit(EXIT_FAILURE);\
		}while(false)

using namespace std;

void *cat_file(void* fName){
	FILE* fp = NULL;
	char str[256];
	char* fname = (char *)fName;
	
	memset(str, '\0', sizeof(str));
	
	if((fp = fopen(fname, "r")) == NULL){
		cout << "ファイルが存在しません" << endl;
	}else{
		while(fgets(str, sizeof(str), fp) != NULL){
			cout << str;
		}
		fclose(fp);
	}
}

void *cat_char(void* ){
	string str = "";
	
	while(true){
		getline(cin, str);
		if(str == "END"){
			break;
		}
		cout << str << endl;
	}
}

int main(int argc, char *argv[]){
	int result = 0;
	pthread_t thr;

	if(argc == 1){
		result = pthread_create(&thr,NULL,cat_char,NULL);
		if(result != 0){
			error(result, "pthread_create");
		}
		result = pthread_join(thr, NULL);
		if(result != 0){
			error(result, "pthread_join");
		}
	}else if(argc > 1){
		for(int i=1;i<argc;i++){
			result = pthread_create(&thr,NULL,cat_file,argv[i]);
			if(result != 0){
				error(result, "pthread_create");
			}
			result = pthread_join(thr, NULL);
			if(result != 0){
				error(result, "pthread_join");
			}
		}
	}
	return 0;
}


