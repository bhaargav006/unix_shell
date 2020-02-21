#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include<stdbool.h>

void wc(int mode, char* filename){
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen(filename, "r");

	if (fp == NULL) {
		exit(EXIT_FAILURE);
	}

	int l=0;
	int w=0;
	while ((read = getline(&line, &len, fp)) != -1) {
		l++;
		bool isRead=false;
		for(int i=0;i<len;i++){
			if(line[i]==' ' || line[i]=='\n' || line[i]=='\t' || line[i]=='\0'){
				if(isRead){
					// printf("\n");
					w++;
					isRead=false;
				}
			}
			else {
				// printf("%c",line[i]);
				isRead = true;
			}
		}
		line = NULL;
		
	}

	int c = 0;
	fseek(fp, 0L, SEEK_END);
	c = ftell(fp);
	fclose(fp);
	if(mode == 0)
		printf("\t%d\t%d\t%d\n",l-1,w,c);
	else if(mode == 1)
		printf("\t%d\n",l-1);
	else if(mode == 2)
		printf("\t%d\n",w);
	else printf("\t%d\n",c);
}

int main(int argc, char** argv){
	if(argc < 2){ 
		printf("Pass arguments to WC and filename as input\n");
		return 0;
	} else if(argc>2){
		if(strcmp(argv[1], "-l") == 0) { 
			wc(1, argv[2]);
		} else if(strcmp(argv[1], "-w") == 0) { 
			wc(2, argv[2]);
		} else if(strcmp(argv[1], "-c") == 0) { 
			wc(3, argv[2]);
		} else {
			printf("Invalid arguments\n");
		}
	} else { 
    	wc(0, argv[1]);
  }
	return 0;
}