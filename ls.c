#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include <sys/stat.h> 
#include <dirent.h>
#include <limits.h> 

struct Dir{
	char path[PATH_MAX];
};


bool isFile(char *path){
	struct stat path_stat;
    stat(path,&path_stat);
    return S_ISREG(path_stat.st_mode);
}

bool isDir(char *path){
	struct stat path_stat;
    stat(path,&path_stat);
    return S_ISDIR(path_stat.st_mode);
}


void ls(char *path, bool recurse_flag) {
	char cwd[PATH_MAX];
	if(path == NULL) {
    	getcwd(cwd, sizeof(cwd));
    	path = cwd;
    }

    if(isFile(path)){
    	printf("%s\n",path);
    }

    else if(isDir(path)){
    	printf("In directory: %s\n",path);
        int i=0;
        struct dirent **namelist;
    	int n;
   		n = scandir(path, &namelist, NULL, alphasort);
	    if (n < 0){
	        perror("scandir");
	        return;
	    }
	    char **dirPaths;
		dirPaths = malloc(n * sizeof(char*));
		int count =0;
        while (i<n) {
        	//To avoid . and ..
            if(i < 2 ){
                i++;
                continue;
            }
            printf("%s\n", namelist[i]->d_name);
            char next_path[PATH_MAX];
            snprintf(next_path, PATH_MAX, "%s/%s", path, namelist[i]->d_name);
            if(isDir(next_path)){
            	dirPaths[count] = malloc((PATH_MAX + 1) * sizeof(char));
                strcpy(dirPaths[count], next_path);    
                //printf("%s\n",dirPaths[count]); 
                count++;          
            }
            free(namelist[i]);
            ++i;
        }
        //printf("Directories are :\n");
        if(recurse_flag){
        	printf("\n");
        	for(int i=0;i<count;i++){
        		//printf("%s\n",dirPaths[i]);
        		ls(dirPaths[i],true);
        		free(dirPaths[i]);
        	}
        }
        free(dirPaths);
        free(namelist);

    }

}

int main(int argc, char **argv){
	if(argc < 2){ // No -R flag and no path name
		ls(NULL, false);
	} else if(strcmp(argv[1], "-R") == 0) { 
		if(argc == 2) { // only -R flag
			ls(NULL, true);
		} else { // -R flag with some path name
			ls(argv[2], true);
		}
	} else { // no -R flag but path name is given
    	ls(argv[1], false);
  }
	return 0;
}
