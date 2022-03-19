#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define maxInput 1024 // stores the max characters input can take can be changed here
#define maxCommands 50 // stores the max arguments input can take can be changed here
#define clear() printf("\033[H\033[J")// clear character set

//-----------------------------------------------------------------------------//

char *getCurrentDir();//print cuurent working directory
int changeCurrentDir(char *);//change directory
int builtInFunctions(char **,int);//stores the builtin functions and linux internal commands
char **parseCommands(char *,int *);//parse commands based on spaces and tabs
int externalCommands(char **);
void pipeFound(char *,char *,char *);//it will parse commands if pipe is found
void inputRedFound(char *,char *,char *);//it will parse commands if < is found
void outputRedFound(char *,char *,char *);//it will parse commands if > is found
void appendRedFound(char *,char *,char *);//it will parse commands if >> is found
void executePipe(char **,char **);
void executeOutputRedirection(char **,char **);
void executeAppendRedirection(char **,char **);
void getHistory(FILE *,char *);
int checkBothInputOutputRedirection(char *,char *,char *);//execute if both < and > found


//-----------------------------------------------------------------------------//

int main(){

    char input[maxInput];//array for storing input
    char before[maxInput];//array for storing first command in case of piping and redirection
    char after[maxInput];//array for storing second command in case of piping and redirection

    char *historyPath = (char *)malloc(maxInput*sizeof(char));//it will fetch the history file absolute path
    strcpy(historyPath,getCurrentDir());

    strcat(historyPath,"/tmp/history.txt");

    FILE *historyFile = fopen(historyPath,"w");//open history file in write mode
    
    
    while(1){

        printf("%s $ ",getCurrentDir());//promt

    //lines 50-77 will take input and store it in history if not null
        int counter = 0;
        char alpha;

        while(1){

            alpha=getchar();
            
            if(alpha == '\n'){

                input[counter] = '\0';
                break;
            }

            else{
                input[counter] = alpha;
                counter++;
            }

        }

        if(input[0] == '\0')
           continue;
        
        else
             fprintf(historyFile,"%s\n",input);
        
        //lines 77-143 will check and if found execute piping and redirection related stuff
        char *c = strchr(input,'|');
        char *d = strchr(input,'<');
        char *e = strchr(input,'>');

        if(checkBothInputOutputRedirection(d,e,input) == 1){
            continue;
        }


        if( c != NULL){

            pipeFound(input,before,after);

            int b1 = 0,a1 = 0;

            char **parsedBefore = parseCommands(before,&b1);
            char **ParsedAfter = parseCommands(after,&a1);

            executePipe(parsedBefore,ParsedAfter);

        }

        else if( d != NULL){

            inputRedFound(input,before,after);

            int b2 = 0,a2 = 0;

            char **parsedBefore = parseCommands(before,&b2);
            char **ParsedAfter = parseCommands(after,&a2);

            ParsedAfter[1] = ParsedAfter[0];
            ParsedAfter[0] = "cat";
            ParsedAfter[2]=NULL;

            executePipe(ParsedAfter,parsedBefore);

        }

        else if( e != NULL){

            if(*(e+1) == '>'){

                appendRedFound(input,before,after);

                int b4 = 0,a4 = 0;

                char **parsedBefore = parseCommands(before,&b4);
                char **ParsedAfter = parseCommands(after,&a4);

                executeAppendRedirection(parsedBefore,ParsedAfter);
                
            }

            else{
                outputRedFound(input,before,after);

                int b3 = 0,a3 = 0;

                char **parsedBefore = parseCommands(before,&b3);
                char **ParsedAfter = parseCommands(after,&a3);

                executeOutputRedirection(parsedBefore,ParsedAfter);
            }

        }

        //below section in the main execute normal commands
        else{

            int actualPresentArguments = 0;

            char **parser = parseCommands(input,&actualPresentArguments);
            

            if(strcmp(parser[0],"history") == 0){

                getHistory(historyFile,historyPath);


            }

            else{
            
                int test = builtInFunctions(parser,actualPresentArguments);

                if(test == -500){

                    int del = remove(historyPath);
                    return 0;
                    
                }

                if(test == -500){
                    return 0;
                }

                if(test != 1 && test != -500) 
                {
                    if(externalCommands(parser) != 0) 
                        printf("Command not found\n");

                }
            }
        }
        
       
    }
}

//The following section have code of the functions defined in the top of the file
//The names of functions and variables are chosen such that it is self explainatory whta the code does

//-----------------------------------------------------------------------------//

char *getCurrentDir(){

    char *buff = (char *)malloc(maxInput*sizeof(char));
    getcwd(buff,maxInput);
    return buff;

}

//-----------------------------------------------------------------------------//


char **parseCommands(char *input,int *i){

    char **parse = (char **)malloc(maxCommands * sizeof(char *));

    for(int i=0;i<maxCommands;i++)
        parse[i] = (char *) malloc(maxInput * sizeof(char));

    char *p = strtok (input, " \t");

    int j=0;
    while (p != NULL)
    {
        parse[j++] = p;
        p = strtok (NULL, " \t");
    }

    *i = j;

    parse[j] = NULL;
    return parse;
}

//-----------------------------------------------------------------------------//


int builtInFunctions(char **input,int j){

    if(strcmp(input[0],"pwd") == 0){
        printf("%s\n",getCurrentDir());
        return 1;
    }
    
    if(strcmp(input[0],"echo") == 0 && input[1][0] == '$'){
        
        char * env = getenv(&input[1][1]);
        
        if(env != NULL)
            printf("%s\n",env);
        
        else   
            printf("%s is not defined as an evironment variable\n",&input[1][1]);        

        return 1;

    }

    if(strcmp(input[0],"cd") == 0){

        if(input[1] == NULL){
            chdir(getenv("HOME"));
            return 1;

        }

        int answer = chdir(input[1]);
        
        if(answer == -1)
            printf("%s : No such directory\n",input[1]);
        
        return 1;

    }

    if(strcmp(input[0],"setenv") == 0){

        char before[maxInput];
        char after[maxInput];

        int counter = 0;

        while (1)
        {
            if(input[1][counter] != '='){
                before[counter] = input[1][counter];
                counter = counter + 1;
            }

            else{
                before[counter] = '\0';
                break;
            }

        }

        counter++;

        strcpy(after,&input[1][counter]);

        if(before==NULL || after==NULL){
            printf("Error... Set the environment variable according to the syntax\n");
            return 1;

        }

        setenv(before,after,1);
        return 1;


    }

    if(strcmp(input[0],"help") == 0){

        printf("The shell executes and set env variables and built in commands like pwd cd and all external cmds with piping and redirection\n");
        return 1;
    }

    if(strcmp(input[0],"exit") == 0){

        return -500;
    }


}

//--------------------------------------------------------------------------//

int externalCommands(char ** parse){

    pid_t pid = fork();

    if(pid == 0){
        if(execvp(parse[0],parse) < 0)
            return -1;
        return 0;
    }

    else{
        wait(NULL);
        return 0;
    }


}

//-----------------------------------------------------------------------------//

void getHistory(FILE *historyFile,char *historyPath){

        
        fclose(historyFile);

        FILE *buff = fopen(historyPath,"r+");

        int c;
        while((c=getc(buff)) != EOF)
            putchar(c);

        fclose(buff);

        historyFile=fopen(historyPath,"a+");

    }



//-----------------------------------------------------------------------------//


void pipeFound(char *inp,char *before,char *after){


    int j = 0;

    while (1)
    {
        if(inp[j] != '|'){
            before[j] = inp[j];
            j = j + 1;
        }

        else{
            before[j] = '\0';
            break;
        }

    }

    j++;

    strcpy(after,&inp[j]);

}

//--------------------------------------------------------------------------------//

void inputRedFound(char *inp,char *before,char *after){


    int j = 0;

    while (1)
    {
        if(inp[j] != '<'){
            before[j] = inp[j];
            j = j + 1;
        }

        else{
            before[j] = '\0';
            break;
        }

    }

    j++;

    strcpy(after,&inp[j]);

}

//--------------------------------------------------------------------------------//


void outputRedFound(char *inp,char *before,char *after){


    int j = 0;

    while (1)
    {
        if(inp[j] != '>'){
            before[j] = inp[j];
            j = j + 1;
        }

        else{
            before[j] = '\0';
            break;
        }

    }

    j++;

    strcpy(after,&inp[j]);

}

//--------------------------------------------------------------------------------//

void appendRedFound(char *inp,char *before,char *after){


    int j = 0;

    while (1)
    {
        if(inp[j] != '>'){
            before[j] = inp[j];
            j = j + 1;
        }

        else{
            before[j] = '\0';
            break;
        }

    }

    j=j+2;

    strcpy(after,&inp[j]);

}

//--------------------------------------------------------------------------------//


void executePipe(char** prev, char** next)
{
    int fd[2];
    pipe(fd);

    pid_t pid1 = fork();

    if(pid1 == 0 )
    {
        dup2(fd[1],STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(prev[0],prev) ;
                        
    }

    pid_t pid2 = fork();

    if(pid2 == 0)
    {
        dup2(fd[0],STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(next[0],next) ;


    }    

    close(fd[0]);
    close(fd[1]);

    
    
    waitpid(pid1,NULL,0);
    waitpid(pid2,NULL,0);
    
}

//--------------------------------------------------------------------------------//

void executeOutputRedirection(char** prev,char** next){

    pid_t pid = fork();

    if(pid == 0){

        
        freopen(next[0],"w",stdout);
        execvp(prev[0],prev);

    }

    waitpid(pid,NULL,0);
}

//--------------------------------------------------------------------------------//

void executeAppendRedirection(char **prev,char **next){

    pid_t pid = fork();

    if(pid == 0){

        
        freopen(next[0],"a",stdout);
        execvp(prev[0],prev);

    }

    waitpid(pid,NULL,0);
}

//--------------------------------------------------------------------------------//

int checkBothInputOutputRedirection(char *d,char *e,char *input){

    if(d != NULL && e != NULL){

        char before[maxInput];
        char after[maxInput];

        inputRedFound(input,before,after);

        int i = 0,j=0;
        char **parseB = parseCommands(before,&i);
        char **parseA = parseCommands(after,&j);

        char output[maxInput];
        strcpy(output,parseA[2]);        

        strcpy(parseA[1],parseA[0]);
        parseA[0]="cat";
        parseA[2]=NULL;

        pid_t pid = fork();

        if(pid == 0){

            freopen(output,"w",stdout);
           

        int fd[2];
        pipe(fd);

        pid_t pid1 = fork();

        if(pid1 == 0 )
        {
            dup2(fd[1],STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execvp(parseA[0],parseA);            
        }

        pid_t pid2 = fork();

        if(pid2 == 0)
        {
            dup2(fd[0],STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            execvp(parseB[0],parseB);
            exit(1);

        }

        close(fd[0]);
        close(fd[1]);
        
        waitpid(pid1,NULL,0);
        waitpid(pid2,NULL,0);
        exit(1);

        }

        waitpid(pid,NULL,0);
       
        return 1;


    }
        
    else
    return -11;

}

//--------------------------------------------------------------------------------//
