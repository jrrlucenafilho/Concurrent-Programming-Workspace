//To bidu@ci.ufpb.br
//Title: [PROVA-LPII-231] José Ricardo Rodrigues de L. Filho - 20210025762
//Caesar's cypther moving 1 position forward (can be changed from macro)

//Comentarios sobre a execucao no final do codigo
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <string.h>
#include <ctype.h>

/**
 * Pegar os argv's e dividir cada arg para um proc, where each proc will be responsible for
 * encrypting a portion of the string.
 * By using args, the program will already separate the strings for me in the argvector (eases my life)
*/

#define CAESAR_SHIFT 1

//Apply the Caesar cipher to a single character
void* caesar_code_char(void* ptr)
{
    int shift = CAESAR_SHIFT;   //For modularity

    //Converting void ptr to char for manip.
    char* str_char = (char*)ptr;

    if(isalpha(str_char)){
        //Trying to use to ASCII table here to do it (but need to be fast)
        //So i need to check th ascii table to see if should get from uppercase or lowercase portion first
        char base = isupper(str_char) ? 'A' : 'a';
        return (void*)((str_char - base + shift)); //% 26 + base);
    }

    return (void*)str_char;
}

//Apply the Caesar cipher to a whole str, using the char-only one as help for modularity
void* caesar_code_str(void* ptr)
{
    //Now onverting void ptr to char for str manip.
    char* str = (char*)ptr;
    
    int length = strlen(str);

    for(int i = 0; i < length; i++){
        str[i] = caesar_code_char(&str[i]);
    }
}

int main(int argc, char* argv[])
{
    int* child_pids;
    char* proc_strs;
    int original_pid = getpid();

    clock_t proc_start_clk, proc_end_clk;
    double total_cpu_time;


    if(argc == 1){  //Arg quant checking
        printf("USO INCORRETO, ESCREVA A MENSAGEM A SER CRIPTOGRAFADA\n");
        return 0;
    }

    //Dynamic alloc for arrays
    //Array for child processes' pids
    child_pids = (int*) malloc(sizeof(int) * (argc));
    
    //Array for strings that will be encripted in each process
    proc_strs = (char*) malloc(sizeof(char) * (argc));

    //PROCESSES BEGIN (properly)
    proc_start_clk = clock();

    //Makes process for each element in agrv (each arg is a word)
    for(int i = 1; i <= argc; i++){

        if(getpid() == original_pid){ //Og process routine goes here
            child_pids = fork(); //Since fork() returns the pid of the child on the main proc
                                 //(and returns 0 ON THE CHILD, so let's check for that to direct the proper routine)

            if(child_pids[i] == 0){ //Now here goes the child processes' routine, from here downwards
            //So it'll have to call the Caesar code encrypt function, just does that and exits

            exit(caesar_code_str(&proc_strs[i]));
            //NOW, this way it'll call the function on exit and return it's return value
            //(which is the encrypted string) and it's over here for child procs (they end exec)
            }
        }
    }

    //FROM HERE DOWNARDS: Will only happen in the main process, since the child procs have already
    //exited on line 89

    //Waiting for the procs to finish processing their given strings
    //While also adding the now-processed strings into the procs_strs array
    for(int i = 1; i < argc; i++){
        //Now, time to wait for the procs and get their converted returns
        waitpid(child_pids[i], &proc_strs, 0);

        //Now this will get the returns from the child_procs status' that they returned while exit()'ing with
        //the caesar_code_str() func as a param.
        //And puts these strs in the proc_strs array in ther corresponding positions
        proc_strs[i] = WEXITSTATUS(proc_strs[i]);
    }

    //Now iterates through the array of now-processed strings that were returned
    //from the procs.
    //And also prints it, showing the encrypted version of the string
    for(int i = 1; i < argc; i++){
        //This part alone is the same as with threads, since it's just iterating through the retunred strings
        //that were converted from argvector in each proc
        //Formatting printing with spaces (First str has only 1 space to the left,
        //middle str has spaces on both sides, and last str has a space only on the right (and a newline char))
        //May not be the best way to do it, but gotta speed to finish it in time
        if(i == 1){
            printf("%s ", argv[i]);
        }

        printf(" %s ", argv[i]);

        if(i == argc){
            printf(" %s\n", argv[i]);
        }
    }

    //PROCESSES END
    proc_end_clk = clock();

    //Calculating time used in seconds
    total_cpu_time = ((double)(proc_end_clk - proc_start_clk)) / CLOCKS_PER_SEC;

    //Now prints the time used while processess processed the strings
    printf("-> Total processing time (Processes): %f\n", total_cpu_time);

    return 0;
}

/** Comments (Same on both files): 
 * Well, the implementation using processes should use more resources and mem since there's a bigger
 * overhead while dealing with them, and because they copy the entirety of the .c file source code, as oposed
 * to threads, which only execute one function at a time and have shared mem in the form
 * of global variables, (which wasn't really needed in my implementations of this program since it was more string-based).
 * 
 * So, Processes version should take longer thanks to this overhead i just explained that is needed to run them
 * and not on threads.
 * 
 * This same overhead in memory needed also affects the execution time. So the threads version should be faster and
 * need less memory overhead
*/