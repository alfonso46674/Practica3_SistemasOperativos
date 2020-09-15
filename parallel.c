#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ITERATIONSPI 2000000000 // total de iteraciones a hacer para calcular PI
#define SHAREDMEMORYSIZE 1024
#define NUMPROCESOS 4

void piCalculation(int numProceso);
double *sharedMemory();

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


    int p;
    for (int i = 0; i < NUMPROCESOS; i++)
    {
        p = fork();
        if (p == 0)
        {
            piCalculation(i);
            exit(0);
        }
        else
        {
            wait(&status);
        }
    }



    // sumar todos las tareas hechas de cada proceso para dar un resultado final
    double resultadoFinal = 0;
    double *piGlobalMain = sharedMemory();
    for (int i = 0; i < NUMPROCESOS; i++)
    {
        resultadoFinal += piGlobalMain[i];
    }

    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; //Tiempo final

    elapsedTime = stop_ts - start_ts;
    printf("----------------------\n");
    printf("Tiempo total, %lld segundos\n", elapsedTime);
    printf("Resultado, %f\n", resultadoFinal);
}



//funcion que inicializa el segmento de la memoria compartida y regresa un apuntador a ella
double *sharedMemory()
{

    // shmget regresa un identificador asociado al segmento creado en la memoria
    int shmid = shmget(1232, SHAREDMEMORYSIZE, 0666 | IPC_CREAT);

    //shmat asocia el id generado con shmget, regresando un apuntador a esa ubicacion de memoria
    double *piGlobal = (double *)shmat(shmid, (void *)0, 0);

    return piGlobal; // regresa el apuntador a la memoria compartida
}




// funcion que hace los calculos de pi
void piCalculation(int indiceProceso)
{

    double *piGlobal = sharedMemory(); //apuntador a la memoria compartida

    //definir rango de calculacion en funcion del numero del proceso
    long inicio = indiceProceso * (ITERATIONSPI / NUMPROCESOS);
    long final = (indiceProceso + 1) * (ITERATIONSPI / NUMPROCESOS);

    double result = 0;
    for (long i = inicio; i < final; i++)
    {
        result += ((pow(-1, i)) / ((2 * i) + 1)); // formula que fue proporcionada para calcular PI/4
    }
    result = result * 4; // Dividir el resultado entre 4 para obtener PI

    piGlobal[indiceProceso] = result; // el resultado se almacena en la memoria compartida dependiendo del indice del proceso actual

    printf("PID proceso: %d\n", getpid());
    printf("Proceso %d, inicio: %lu, final %lu\n", indiceProceso, inicio, final);
    printf("Resultado del proceso %d con resultado %.20f\n", indiceProceso, result);
    printf("------------\n");
}