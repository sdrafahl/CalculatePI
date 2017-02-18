#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <gmp.h>
#include <unistd.h>
 typedef struct {
    /*Number of iterations that will process through infinite series*/
    int iterations;
     mpf_t added;
     mpf_t substracted;
}DataShared;

int DataSharedConst(DataShared* d){
    d->iterations=40000000;
    mpf_init(d->added);
    mpf_init(d->substracted);
    
    return 0;
}

void *calc_add(void *arg){
    printf("addition thread \n");
    DataShared *data = arg;
    int itr = data->iterations;
    int x;
    mpf_t denom;
    mpf_init(denom);
    mpf_add_ui(denom,denom,1); 
    for(x=0;x<itr;x++){
        mpf_ui_div(data->added,1,denom);
        mpf_add_ui(denom,denom,4); 
    }
    return NULL;
}

void *calc_sub(void *arg){
    printf("substraction thread \n");
    DataShared *data = arg;
    int itr = data->iterations;
    int x;
    mpf_t denom;
    mpf_init(denom);
    mpf_add_ui(denom,denom,3); 
    for(x=0;x<itr;x++){
        mpf_ui_div(data->substracted,1,denom);
        mpf_add_ui(denom,denom,4); 
    }
    return NULL;
}
  
int main()
{
    /*Initial Setup*/
    pthread_t tid;
    pthread_t tid1;
    DataShared *cache;
    cache = malloc(sizeof(DataShared));
    

    DataSharedConst(cache);
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
    mpf_t pi;
    mpf_init(pi);
    mpf_sub(pi,cache->added,cache->substracted);
    mpf_mul_ui(pi,pi,4);
    char  *sf;
    mp_exp_t exponent ;
    exponent = 0;
    sf = mpf_get_str(NULL,&exponent,10,0,pi);
    gmp_printf("%s\n",sf); 
    exit(0);
}