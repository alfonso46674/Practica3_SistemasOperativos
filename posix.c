#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <pthread.h>

#define ITERATIONSPI 2000000000
#define NTHREADS 4

double resultado = 0; // variable global para los 4 hilos

double piCalculation();

void *piCalculationThreads(void *args){
    int nthread = *((int *)args);

    //Sacar los rangos de inicio y fin para cada hilo basandose en su ID, numero de hilos y las iteraciones totales
    int inicio = nthread * (ITERATIONSPI/NTHREADS);
    int final = (nthread+1) * (ITERATIONSPI/NTHREADS);

    double result=0;// variable local para almacenar el resultado de un hilo
    for(long i = inicio; i < final; i++ ){
        result +=  ((pow(-1,i))/((2*i)+1));
    }
    result = result * 4;

    resultado = resultado + result; // guardar el resultado local en la variable global
   // printf("resultado %f para thread %d\n",result,nthread);

}

int main(){
    //variables para calcular tiempo
    long long start_ts;
    long long stop_ts;
    long long elapsedTime;
    long lElapsedTime;
    struct timeval ts;
    

    //Variables para hilos
    pthread_t tid[NTHREADS];
    int args [NTHREADS];
  
    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; //Tiempo inicial
    


    //Crear hilos
    for(int i = 0; i < NTHREADS; i++){
        args[i]=i;
        pthread_create(&tid[i],0,piCalculationThreads,&args[i]);
    }


     


     //Unir hilos
     for(int i = 0; i < NTHREADS; i++){
         pthread_join(tid[i],NULL);
     }


    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; //Tiempo final

    elapsedTime = stop_ts - start_ts;
    printf("----------------------\n");
    printf("Tiempo total, %lld segundos\n", elapsedTime);
    printf("Resultado, %f\n", resultado);
}
