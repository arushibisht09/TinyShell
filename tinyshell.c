#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//REPL - 
// It’s a program pattern where the computer:
// Reads what you type
// Evaluates (runs/interprets) it
// Prints the result
// Loops back and waits for the next input
// This keeps happening until you quit. 

#define MAX_INPUT 1024 
#define MAX_ARGS 64 

// program was successful
int main(){

    char input[MAX_INPUT] ; 
    char* args[MAX_ARGS]; 

    // we create a never ending loop/ a loop we can only break from inside 
    while(1){
        printf("mysh> ");
        // It empties the buffer for stdout by pushing whatever is waiting out to the terminal immediately.
        fflush(stdout); 

        if(fgets(input, MAX_INPUT, stdin)== NULL) {
            perror("fgets failed"); 
            continue; 
        }

        input[strcspn(input, "\n")] = '\0' ; 

        if(strcmp(input, "exit") ==0 ){
            printf("Have nice day!\n"); 
            break;
        }

        char* token = strtok(input, " ") ; 
        int i = 0 ;

        while (token != NULL && i< MAX_ARGS)
        {
            args[i] = token ;
            token = strtok(NULL, " ") ;
            i++ ; 
        }
        args[i] = NULL ; 

        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1]== NULL)
            {
               perror("cd: missing arguments");
            }
            else if(chdir(args[1]) != 0 ){
                perror("cd failed");
            }
            continue; 
        }
        

        pid_t pid = fork(); 

        if(pid == 0){
            execvp(args[0], args); 
            perror("execvp failed"); 
            exit(EXIT_FAILURE); 
        }
        else if (pid>0){
            int status;
            waitpid(pid, &status, 0) ; 
            printf("Exit status: %d\n", status) ;
        }
        else{
            perror("fork failed"); 
        }

       }
    return 0 ;
}

