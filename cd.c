#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>

void cd(char* arg){
	const char * path = arg;
	int retvalue = chdir(path);
	if(retvalue==-1)
		printf("Couldn't change the directory\n");
	char cwd[150];
	getcwd(cwd,sizeof(cwd));
	printf("Currently in %s\n",cwd);
}

int main(int argc, char** argv){

	if(argc<2){
		printf("Pass the path as an argument\n");
		return 0;
	}
	cd(argv[1]);
	return 0;
}