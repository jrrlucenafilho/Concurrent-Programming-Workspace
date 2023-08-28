#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#define EXE_LIMIT 10

typedef struct{
    float temperature;
    float pressure;
} t_situation;

t_situation status;

void* execute(void* p) //This one only reads
{
    printf("Execute() - start!\n");
    printf("considering %f - %f\n", status.temperature, status.pressure);

    float exe_param = 0;

    for(int i = 0; i < EXE_LIMIT; i++){
        exe_param = (status.temperature * 10) + pow(status.pressure, 2);
        sleep(2);
        printf("exe_param = %f\n", exe_param);
        sleep(2);
        
    }
    sleep(2);
}

void* manage(void* p) //This one does the ops
{
    printf("(M) manage() - start!\n");
    printf("(M) considering %f - %f\n", status.temperature, status.pressure);

    for(int i = 0; i < EXE_LIMIT; i++){
        float t = status.temperature;
        float d = status.pressure;

        //Inconsistency here, it's adding up un-updated variables
        //So we gotta use mutex here
        t = t + 10.5;
        status.temperature = t;

        sleep(1);

        d = d + 0.01;
        status.pressure = d;
        
        printf("(M) changed = %f, %f", status.temperature, status.pressure);
        sleep(1);
    }
    sleep(2);
}

int main(void)
{
    pthread_t thread_exe, thread_manage;
    
    status.temperature = 100;
    status.pressure = 1;

    //pthread_create(&thread_exe)
}