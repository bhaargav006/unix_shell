#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include<stdbool.h>

void wc(int mode, char* filename){
	FILE * fp;
  	char * line = NULL;
  	size_t len = 0;
  	char ch;

	// initialsing the file pointer to read
	if(filename != NULL) {
		fp = fopen(filename, "r");
	} else {
		fp = stdin;
	}

	if (fp == NULL) {
		exit(EXIT_FAILURE);
	}

	int l=0;
	int w=0;
	int c=0;
	bool isRead=false;
	while ((ch = getc (fp)) != EOF) {
		c++;
		if(ch==' ' || ch=='\n' || ch=='\t' || ch=='\0'){
			if(ch=='\n')
				l++;
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
	if(isRead)
		w++;

	// int c = 0;
	// fseek(fp, 0L, SEEK_END);
	// c = ftell(fp);
	fclose(fp);
	if(mode == 0)
		printf("\t%d\t%d\t%d\n",l,w,c);
	else if(mode == 1)
		printf("\t%d\n",l);
	else if(mode == 2)
		printf("\t%d\n",w);
	else printf("\t%d\n",c);
}

int main(int argc, char** argv){
	if(argc>2){
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