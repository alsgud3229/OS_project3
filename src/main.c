#include <stdio.h>
#include <unistd.h>

#include "mythread.h"

void f(void* args){
  int j=0;
  for(int i=0; i<1000000; i++){
    if(i%30000==0){
      j++;
      fprintf(stderr, "f#%d\n", j);
    }
  }
}

void g(void* args){
  const int n = *((int*)args);
  int k=0;
  for (int i = 0; i < n; i++) {
    if(i%30000==0){
      k++;
      fprintf(stderr, "g#%d\n",k);
    }
  }
}

int main(){  
  int type;
  printf("Choose between '1(R)' and '2(F)': ");
  scanf("%d", &type);

  if(type==1){
    mythread_init(MYTHREAD_RR);
  }
  else if(type==2){
    mythread_init(MYTHREAD_FIFO);
  }
  else{
    printf("Invalid Type\n");
    return 0;
  }
  
  int n = 1000000;
  int tid1 = mythread_create(f, NULL);
  int tid2 = mythread_create(g, &n);

  mythread_join(tid1);
  mythread_join(tid2);

  printf("main finished\n");
  return 0;
}


