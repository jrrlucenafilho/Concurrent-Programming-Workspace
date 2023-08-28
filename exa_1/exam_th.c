//To bidu@ci.ufpb.br
//Title: [PROVA-LPII-231] José Ricardo Rodrigues de L. Filho - 20210025762
//Caesar's cypther moving 1 position forward (can be changed from macro)

//Comentarios sobre a execucao no final do codigo
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <string.h>
#include <ctype.h>

/**
 * Pegar os argv's e dividir cada arg para uma thread, where each thread will be responsible for
 * encrypting a portion of the string.
 * By using args, the program will already separate the strings for me in the argvector (eases my life)
*/

#define CAESAR_SHIFT 1

//Apply Caesar Cipher to a single character
void* caesar_code_char(void* ptr)
{
    int shift = CAESAR_SHIFT;   //For modularity

    //Converting void ptr to char for manip.
    char* str_char = (char*)ptr;

    if(isalpha(str_char)){
        //Trying to use to ASCII table here to do it (but need to be fast)
        //So need to check the table to see if should get from uppercase or lowercase portion of it (only the chars)
        char base = isupper(str_char) ? 'A' : 'a';
        return (void*)((str_char - base + shift) % 26 + base);
    }

    return (void*)str_char;
}

//Apply Caesar Cipher to a whole str
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
    pthread_t* threads;
    char* thread_strs;

    clock_t th_start_clk, th_end_clk;
    double total_cpu_time;


    if(argc == 1){  //Arg quant checking
        printf("USO INCORRETO, ESCREVA A MENSAGEM A SER CRIPTOGRAFADA\n");
        return 0;
    }

    //Dynamic alloc for arrays
    //Array for threads
    threads = (pthread_t*) malloc(sizeof(pthread_t) * (argc));
    
    //Array for strings that will be encripted in each thread
    thread_strs = (char*) malloc(sizeof(char) * (argc));

    //THREADS PROCESSING PORTION BEGIN (properly)
    th_start_clk = clock();

    //Makes the a thread for each element in agrv (each arg is a word)
    for(int i = 1; i < argc; i++){
        pthread_create(&threads[i], NULL, caesar_code_str, argv[i]);
        //Here it's assigning each thread in the threads array to a string (argv[i])
        //And calling it to encrypt the string it's given
    }

    //Waiting for the threads to finish processing their given strings
    //While also adding the now-processed strings into the thread_strs array
    for(int i = 1; i < argc; i++){
        pthread_join(threads[i], (void**)thread_strs[i]);
    }

    //Now iterates through the array of now-processed strings that were returned
    //from the threads.
    //And also prints it, showing the encrypted version of the string
    for(int i = 1; i < argc; i++){
        //Formatting printing with spaces (First str has only 1 space to the left,
        //middle str has spaces on both sides, and last str has a space only on the right (and a newline char))
        //May not be the best way to do it, but gotta speed it to finish it in time
        if(i == 1){
            printf("%s ", argv[i]);
        }

        printf(" %s ", argv[i]);

        if(i == argc){
            printf(" %s\n", argv[i]);
        }
    }

    //THREADS PROCESSING PORTION END (properly)
    th_end_clk = clock();

    //Calculating time used in seconds
    total_cpu_time = ((double)(th_end_clk - th_start_clk)) / CLOCKS_PER_SEC;

    //Now prints the time used while threads processed the strings
    printf("-> Total processing time (Threads): %f\n", total_cpu_time);

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