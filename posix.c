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

double resultado = 0;

double piCalculation();

void *piCalculationThreads(void *args){
    int nthread = *((int *)args);

    int inicio = nthread * (ITERATIONSPI/NTHREADS);
    int final = (nthread+1) * (ITERATIONSPI/NTHREADS);

    double result=0;
    for(long i = inicio; i < final; i++ ){
        result +=  ((pow(-1,i))/((2*i)+1));
    }
    result = result * 4;

    resultado += resultado + result;
    printf("resultado %f para thread %d\n",result,nthread);

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

 double piCalculation(){
     double result=0;
    for(long i = 0; i < ITERATIONSPI; i++ ){
        result +=  ((pow(-1,i))/((2*i)+1));
    }
    result = result * 4;

    return result;
}