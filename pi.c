#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
 typedef struct {
    /*Number of iterations that will process through infinite series*/
    int iterations;
    float added;
    float substracted;
}DataShared;

int DataSharedConst(DataShared* d){
    d->iterations=10000;
    d->added=0.0;
    d->substracted=0.0;
    return 0;
}
void *calc_add(void *arg){
    printf("addition thread \n");
    DataShared *data = arg;
    int itr = data->iterations;
    int x;
    float total = 0.0;
    int denom=1;
    for(x=0;x<itr;x++){
        total+=1.0/denom;
        denom+=4;
        
    }
    data->added=total;
    return NULL;
}

void *calc_sub(void *arg){
    printf("substraction thread \n");
    DataShared *data = arg;
    DataSharedConst(data);
    int itr = data->iterations;
    int x;
    float total = 0.0;
    int denom = 3;
    for(x=0;x<itr;x++){
        total+=1.0/denom;
        denom+=4;
        
        
    }
    data->substracted=total;
    return NULL;
}
  
int main()
{
    /*Initial Setup*/
    pthread_t tid;
    pthread_t tid1;
    DataShared *cache;
    cache = malloc(sizeof(DataShared));
    /*Processing*/
    if(pthread_create(&tid, NULL, calc_add, cache)){
        return 1;
    }
    
    if(pthread_create(&tid1, NULL, calc_sub, cache)){
        return 2;
    }
    pthread_join(tid, NULL);
    pthread_join(tid1, NULL);
    /*Program Does Final Calculation*/
    
    double pi = cache->added - cache->substracted;
    pi*=4;
    printf("%f\n",pi);
    exit(0);
}