
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define READ   0
#define WRITE  1
#define Sh_Prompt "msh?: "


void history (char **cmdLine, FILE * hisf, int count){
    for (int i = 0; i < 20; i++){
        
        hisf = fopen(*cmdLine, "r");
        printf(" %s \n", cmdLine[i]);
        cmdLine++;
    }
}


void parser (char *cmd, char **cmdLine){
    while (*cmd != '\0') {
        while (*cmd == ' ' || *cmd == '\n')
            *cmd ++= '\0';
        
        *cmdLine++ = cmd;
        while (*cmd != '\0' && *cmd != ' ' && *cmd != '\n')
            cmd++;
        
    }
    *cmd = '\0';
}

void execute (char **cmdLine) {
    /*
     pid_t pid = fork();
     int fd [2];
     pipe (fd);
     
     if (pid == 0)
     {
     
     close(fd[READ]);
     
     if (execvp(*cmdLine, cmdLine) <0) {
     perror("Execution of command failed");
     exit(1); }
     
     close (fd[WRITE]);
     }
     else
     {
     
     close (fd[WRITE]);
     
     int status = 0;
     while (wait(&status) != pid) {}
     
     close (fd[READ]);
     
     }
     }*/
    
    pid_t pid = fork();
    
    int fd [2];
    pipe(fd);
    
    if (pid == -1) {
        perror("\nError");
        exit(1); }
    else if (pid == 0) {
        
        close(fd[0]);
        
        if (execvp(*cmdLine, cmdLine) <0) {
            perror("Execution of command failed");
            exit(1); }
        
        close(fd[1]);
        //dup(fd[0]);
    }
    else {
        close(fd[1]);
        
        int status = 0;
        while (wait(&status) != pid) {}
        
        close(fd[0]);
    }
}


void semicol( char **cmdLine){
    
    execute(cmdLine--);
    execute(cmdLine++);
    
}

void exLast (char ** cmdLine, int count){
    //count - 1;
    // *cmdLine - 1;//= cmdLine[count];
    execute(cmdLine --);}


int main (int argc, char * argv[]) {
    
    //FILE *mshrc;
    FILE *historyRem;
    
    const int LINE_LENGTH = 1024;
    const int ARGU_SIZE = 64;
    
    int counter = 0;
    char cmd[LINE_LENGTH];
    char *cmdLine[ARGU_SIZE];
    while(1) {
        
        printf(Sh_Prompt);
        gets(cmd);
        historyRem = fopen(cmd, "w");
        counter++;
        printf("\n");
        
        parser(cmd, cmdLine);
        if (strcmp(cmd, "exit") == 0)
            exit(0);
        else if (strcmp (cmd, "history") == 0){
            history(cmdLine, historyRem, counter);
        }
        else if(*cmd == ';'){
            semicol( cmdLine);
        }
        else if (strcmp(cmd, "!!") == 0)
            exLast(cmdLine, counter);
        else
            execute(cmdLine);
        
        
        fclose(historyRem);
    }
}


