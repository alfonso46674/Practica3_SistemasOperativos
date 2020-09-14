//Juan Pablo Ramos Robles 715592
//Alfonso Ramirez Castro 713932
#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ITERATIONSPI 2000000000
#define PROCESOS 4
#define STACK 1024 * 64

double global = 0;

double piCalculation();
int threadFunction();

int main()
{
    void *stack;
    pid_t pid;
    int status;
    int i;
    int params[PROCESOS];
    long long start_ts;
    long long stop_ts;
    long long elapsedTime;
    long lElapsedTime;
    struct timeval ts;

    stack = malloc(STACK * PROCESOS);

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; //Tiempo inicial

    if (stack == 0)
    {
        perror("No se pudo guardar en Stack");
        exit(1);
    }

    for (i = 0; i < PROCESOS; i++)
    {
        params[i] = i;
        // Call the clone system call to create the child thread
        pid = clone(&threadFunction, (char *)stack + STACK * (i + 1),
                    SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &params[i]);
        if (pid == -1)
        {
            perror("clone");
            exit(2);
        }
        usleep(1000);
    }

    for (i = 0; i < PROCESOS; i++)
    {
        pid = wait(&status);
        if (pid == -1)
        {
            perror("waitpid");
            exit(3);
        }
    }

    free(stack);

    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; //Tiempo final

    elapsedTime = stop_ts - start_ts;
    printf("----------------------\n");
    printf("Tiempo total, %lld segundos\n", elapsedTime);
    printf("Resultado, %f\n", global);
}

int threadFunction(void *args)
{
    int nthread = *((int *)args);
    int inicio = nthread * (ITERATIONSPI / PROCESOS);
    int final = (nthread + 1) * (ITERATIONSPI / PROCESOS);
    double result = 0;
    for (long i = inicio; i < final; i++)
    {
        result += ((pow(-1, i)) / ((2 * i) + 1));
    }
    result = result * 4;

    global += global + result;
    printf("resultado %f para thread %d\n", result, nthread);
    printf("Global: %f\n", global);

}
