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
    d->iterations=20000000;
    mpf_init2(d->added,1000);
    mpf_init2(d->substracted,1000);
    return 0;
}

void *calc_add(void *arg){
   printf("addition thread \n");
    DataShared *data = arg;
    int itr = data->iterations;
    int x;
    mpf_t denom;
    mpf_t denomvalue;
    mpf_init2(denomvalue,1000);
    mpf_init2(denom,1000);
    mpf_add_ui(denom,denom,1); 
    for(x=0;x<itr;x++){
        mpf_ui_div(denomvalue,1,denom);
        mpf_add_ui(denom,denom,4);
        mpf_add (data->added,data->added,denomvalue);
    }
    return NULL;
}

void *calc_sub(void *arg){
    
    printf("substraction thread \n");
    DataShared *data = arg;
    int itr = data->iterations;
    int x;
    mpf_t denom;
    mpf_t denom_val;
    mpf_init2(denom_val,1000);
    mpf_init2(denom,1000);
    mpf_add_ui(denom,denom,3); 
    for(x=0;x<itr;x++){
        mpf_ui_div(denom_val,1,denom);
        mpf_add_ui(denom,denom,4);
         mpf_add(data->substracted,data->substracted,denom_val);
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
    mpf_init2(pi,1000);
    mpf_sub(pi,cache->added,cache->substracted);
    mpf_mul_ui(pi,pi,4);
    mp_exp_t exponent ;
    exponent = 1;
    gmp_printf("%s\n", mpf_get_str(NULL,&exponent,10,0,pi)); 
    exit(0);
}