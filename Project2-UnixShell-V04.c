#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> //allow bool use
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void parse(char* input, char* cargs[]) {
    const char break_chars[] = " ";
    char* token;
    int cargs_Index = 0;
    //printf("Input: %s", input);
    token = strtok(input, break_chars);

    //loading input token elements into cargs
    while (token != NULL) {
        //printf("token was: %s\n", token);
        cargs[cargs_Index] = token;
        cargs_Index++;
        token = strtok(NULL, break_chars);
    }
}


int main(int argc, const char * argv[]) 
{  
  char input[BUFSIZ];
  char last_command[BUFSIZ];
  
  //clear buffers
  memset(input, 0, BUFSIZ * sizeof(char));
  memset(last_command, 0, BUFSIZ * sizeof(char));

  bool finished = false;
  
  while (!finished) 
  {
    printf("osh> ");    //command line prefix
    fflush(stdout);     //clear/flush output buffer and move buffered data to console

    //if no input
    if ((fgets(input, BUFSIZ, stdin)) == NULL) // or gets(input, BUFSIZ);
    {
        fprintf(stderr, "no command entered\n");
        exit(1);
    }

    //get input
    //fgets(input, BUFSIZ, stdin);
    input[strlen(input) - 1] = '\0';    // wipe out newline at end of string
    printf("input was: '%s'\n", input);

    // check for history (!!) command
    if (strncmp(input, "!!", 2) == 0) // only compare first 2 characters
    {
        //last command copy
        strcpy(last_command, input);
        if (strlen(last_command) == 0) 
        {
        fprintf(stderr, "no last command to execute\n");
        }
        printf("last command was: %s\n", last_command);
    }
    //exit command
    if (strncmp(input, "exit", 4) == 0) // only compare first 4 letters
    {
        finished = true;
    }
    else 
    {   
        pid_t pid = fork();

        if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 0;
        }
        if(pid != 0){ /* parent process */
        /* parent will wait for the child to complete */
            wait(NULL);
            //printf("Child Complete \n");
        }
        else{ /*child process*/
            char* cargs[BUFSIZ];  //buffer to hold command line arguments
            memset(cargs, 0, BUFSIZ * sizeof(char));

            //printf("You entered: %s\n", input);   // you will call fork/exec
            parse(input, cargs);
            //printf("after parse, what is input: %s\n", input);

            //Printing elements of cargs
            printf("Elements of input array are: \n");
            for(int index = 0; index < sizeof(*cargs); index++)
            {
                if(cargs[index] == NULL)
                    break;
                printf("args[%d] = %s ", index, cargs[index]);
                printf("\n");
            }
            
            if(execvp(cargs[0], cargs) == -1)
            {
                printf("Invalid command \n");
                exit(1);
            }

            //Can only execute single string commands
            execvp(cargs[0], cargs);
        }
    }
  }
  
  printf("osh exited\n");
  printf("program finished\n");
  
  return 0;
}