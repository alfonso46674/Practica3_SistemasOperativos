#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define ITERATIONSPI 1000000 // total de iteraciones a hacer para calcular PI
#define SHAREDMEMORYSIZE 1024
#define NUMPROCESOS 4

double piCalculation(int numProceso, double *piGlobal);

int main()
{
    //variables para manejar el tiempo
    long long start_ts;
    long long stop_ts;
    long long elapsedTime;
    long lElapsedTime;
    struct timeval ts;

    int status = 0;
    
    
    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; //Tiempo inicial


    //******
    //inicializar memoria compartida

    // shmget regresa un identificador asociado al segmento creado en la memoria
    int shmid = shmget(1232, SHAREDMEMORYSIZE, 0666|IPC_CREAT); 

    //shmat asocia el id generado con shmget, regresando un apuntador a esa ubicacion de memoria
    double piGlobal = *( double*) shmat(shmid, (void*)0,0);
     piGlobal = 10.0;
    //printf("PiGlobal: %f\n", piGlobal);
    
    int p;
    for (int i = 0; i < NUMPROCESOS; i++)
    {
        p = fork();
        if (p == 0){
           // piGlobal = piGlobal + 1;
           //printf("PiGlobal: %f\n", piGlobal);
           piCalculation(i, &piGlobal);
            exit(0);
        }
        else{
            wait(&status);
        }
    }

    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; //Tiempo final

    elapsedTime = stop_ts - start_ts;
    printf("----------------------\n");
    printf("Tiempo total, %lld segundos\n", elapsedTime);
     printf("Resultado, %f\n", piGlobal);
}

double piCalculation(int numProceso, double *piGlobal)
{

    //definir rango de calculacion en funcion del numero del proceso
    long inicio = numProceso * (ITERATIONSPI / NUMPROCESOS);
    long final = (numProceso + 1) * (ITERATIONSPI / NUMPROCESOS);

    double result = 0;
    for (long i = inicio; i < final; i++)
    {
        result += ((pow(-1, i)) / ((2 * i) + 1)); // formula que fue proporcionada para calcular PI/4
    }
    result = result * 4; // Dividir el resultado entre 4 para obtener PI
    *piGlobal = *piGlobal +  1;
    printf("PID proceso: %d\n", getpid());
    printf("Proceso %d, inicio: %lu, final %lu\n", numProceso, inicio, final);
    printf("Resultado del proceso %d con resultado %.20f\n", numProceso, result);
    printf("------------\n");
    return result;
}