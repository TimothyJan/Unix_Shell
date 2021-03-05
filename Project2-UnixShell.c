#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> //allow bool use



void parse(char* s) {
  const char break_chars[] = " \t";
  char* p;
  p = strtok(s, break_chars);
  while (p != NULL) {
    printf("token was: %s\n", p);
    p = strtok(NULL, break_chars);
  }
}


int main(int argc, const char * argv[]) {  
  char input[BUFSIZ];
  char last_command[BUFSIZ];
  
  //clear buffers
  memset(input, 0, BUFSIZ * sizeof(char));
  memset(input, 0, BUFSIZ * sizeof(char));

  bool finished = false;
  
  while (!finished) {
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
      strcpy(last_command, input);
      printf("You entered: %s\n", input);   // you will call fork/exec
      parse(input);
			printf("after parse, what is input: %s\n", input);
      printf("\n");
    }
  }
  
  printf("osh exited\n");
  printf("program finished\n");
  
  return 0;
}