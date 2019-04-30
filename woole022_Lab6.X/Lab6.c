#include "xc.h"
#include <stdio.h>
#include "woole022_Lab6.h"

#define N 1024
volatile int Buffer[N];
volatile int count = -1;

void putVal(int newValue){
    if(count<1023){
        Buffer[count++] = newValue;
    }
    else{
        count = 0;        
        Buffer[count] = newValue;
    }
}

unsigned long int getAvg(){
    int x;
    unsigned long int avg = 0;
    for(x=0; x<N; x++)
        avg += Buffer[x];
    return avg/N;
}

void initBuffer(){
    int x;
    for(x=0; x<N; x++)
        Buffer[x] = 0;
}
