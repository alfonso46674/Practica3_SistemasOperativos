#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#define ITERATIONSPI 2000000000 // total de iteraciones a hacer para calcular PI

double piCalculation();

int main(){
    //variables para manejar el tiempo
    long long start_ts;
    long long stop_ts;
    long long elapsedTime;
    long lElapsedTime;
    struct timeval ts;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; //Tiempo inicial
    
    //calcular PI
     double piResult = piCalculation();

    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; //Tiempo final

    elapsedTime = stop_ts - start_ts;
    printf("----------------------\n");
    printf("Tiempo total, %lld segundos\n", elapsedTime);
    printf("Resultado, %f\n", piResult);
}

 double piCalculation(){
     double result=0;
    for(long i = 0; i < ITERATIONSPI; i++ ){
        result +=  ((pow(-1,i))/((2*i)+1)); // formula que fue proporcionada para calcular PI/4
    }
    result = result * 4; // Dividir el resultado entre 4 para obtener PI

    return result;
}