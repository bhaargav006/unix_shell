#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <limits.h>
#include <fcntl.h>

#include "util.h"

int parse_line(char *input, char *tokens[], char *delim) {
    int i = 0;
    char *tok = strtok(input, delim);

    while(tok != NULL) {
        tokens[i] = tok;
        i++;
        tok = strtok(NULL, delim);
    }
    tokens[i] = NULL;
    return i;
}

void change_dir(char *path) {
  if (chdir(path) != 0) {
      printf("chdir() to %s failed", path);
  }
}

void redirect_to_file(char *file_name, bool overwrite_flag) {
  FILE *fptr;
  file_name = trimwhitespace(file_name);
  if(overwrite_flag) {
    fptr = fopen(file_name,"w");
  } else {
    fptr = fopen(file_name,"a");
  }
  dup2(fileno(fptr), 1); // stdout is file descriptor 1
  fclose(fptr);
}

void execute_command(char *command) {
  char *redir_file = NULL;
  bool redirection_flag = false;
  bool overwrite_flag = true;

  // check for redirection
  char *redir_tokens[2];
  if(strstr(command , ">>") != NULL) { // redirection append mode
    parse_line(command, redir_tokens, ">>");
    command = redir_tokens[0];
    redir_file = redir_tokens[1];
    redirection_flag = true;
    overwrite_flag = false;
  } else if(strstr(command , ">") != NULL) {
    parse_line(command, redir_tokens, ">");
    command = redir_tokens[0];
    redir_file = redir_tokens[1];
    redirection_flag = true;
  }

  char *tokens[MAX_TOKENS];
  int token_count = parse_line(command, tokens, " ");
  enum command_type type = get_command_type(tokens[0]);

  if(type == CD) {
    change_dir(tokens[1]);
  } else if(type == EXIT)  {
    printf("Exiting shell\n");
    exit(0);
  } else {
    int pid = fork();
    if(pid == 0) {
      if(redirection_flag) {
        redirect_to_file(redir_file, overwrite_flag);
      }
      // handle command execution
      switch(type) {
        case LS:
          execvp("./ls", tokens);
          break;
        case WC:
          execvp("./wc", tokens);
          break;
        default:
          printf("%s: command not found\n", tokens[0]);
      }
      // Exec didn't work correctly. So killing child process
      exit(0); // exit child process
    } else {
      waitpid(pid, NULL, 0);
    }
  }
}

void print_prompt()
{
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("[4061-shell]%s $", cwd);
    fflush(stdout);
  } else {
    perror("getcwd() error");
  }
}

int main(){
	/*write code here*/
    char inbuf[MAX_CMD_LEN];
    fcntl(STDIN_FILENO, F_SETFL, fcntl(0, F_GETFL));

    while(1) {
        memset(inbuf, '\0', MAX_CMD_LEN);
        int nbytes = read(STDIN_FILENO, inbuf, MAX_CMD_LEN);

        if(nbytes > 0) {
            
            inbuf[nbytes-1] = '\0';
            char *tokens[128];

            if(strlen(inbuf) > 0) {

                int saved_stdin = dup(0);
                int saved_stdout = dup(1);

                int count = parse_line(inbuf, tokens, "|");
                if(count > 2) {
                    printf("Error:Only one pipe allowed\n");
                } else if(count == 2) {
                    int pd[2];
                    pipe(pd);

                    int pid1 = fork();
                    if(pid1 == 0) {
                        dup2(pd[0], 0);
                        close(pd[0]);
                        close(pd[1]);
                        execute_command(tokens[1]);
                        exit(0);
                    }

                    int pid2 = fork();
                    if(pid2 == 0) {
                        dup2(pd[1], 1);
                        close(pd[0]);
                        close(pd[1]);
                        execute_command(tokens[0]);
                        exit(0);
                    }

                    close(pd[0]);
                    close(pd[1]);

                    // Wait for all children to finish
                    waitpid(pid1, NULL, 0);
                    waitpid(pid2, NULL, 0);

                    // Restoring stdin and stdout if needed
                    dup2(saved_stdin, 1);
                    close(saved_stdin);
                    dup2(saved_stdout, 1);
                    close(saved_stdout);

                } else {
                    // No pipe case
                    execute_command(tokens[0]);
                }   
            }
            print_prompt();
        }
    }
}
