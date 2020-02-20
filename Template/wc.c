#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>

void wc(int mode, char* path){
	/*insert coed here*/
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