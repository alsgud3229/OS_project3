#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "mythread.h"

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

static enum mythread_scheduling_policy sched_policy = MYTHREAD_RR; 

typedef struct tcb {
  // TODO: Fill your own code
  int finished;
  int id;
  ucontext_t *context;
  int stack[16384];
}tcb;

static struct tcb* tcbs[1024];
static int n_tcbs = 0;
static int current_thread_id = 0;
static struct itimerval time_quantum;
static struct sigaction ticker;
static int count = 0;

static int nextTcb() {
  if (sched_policy == MYTHREAD_RR) {
    // TODO: Fill your own code
    int next2;
    int cur2=current_thread_id;
    for(int i=n_tcbs; i>0; i-- ){
      if(i!=cur2 && tcbs[i]->finished==0){
        return i;
      }
    }
  } 
  else if (sched_policy == MYTHREAD_FIFO) {
    // TODO: Fill your own code
  }
}

static void tick(int sig) {
  // HINT: 이 함수에서 nextTcb() 함수를 통해 scheduling 하고, 해당 schedule 함수를 실행.
  if(sched_policy==MYTHREAD_FIFO){
    return;
  }

  else{
    int next = nextTcb();
    int cur = current_thread_id;

    static int cnt=0;
    static int tick=0;
    tick++;

    if(tick%2==0){
      fprintf(stderr, "\tTICK!\n");
    }
   
    if(tick==1){
      current_thread_id=n_tcbs;
      if(swapcontext(tcbs[0]->context, tcbs[n_tcbs]->context)==-1){
        handle_error("swapcontext 0->ntcbs");
      }
    }
    else{
      cnt++;
      fflush(stdout);
      if(cnt==2) {
        cnt=0;
        return;
      }
      current_thread_id=next;
      if(swapcontext(tcbs[cur]->context, tcbs[next]->context)==-1){
        handle_error("swapcontext");
      }
    }
  } 
}

void mythread_init(enum mythread_scheduling_policy policy){
  for(int i=0; i<1024; i++){
    tcb *tbs=(tcb*)malloc(sizeof(tcb)+1);
    tcbs[i]=tbs;
  } 
  sched_policy = policy;
  n_tcbs = 0;
  current_thread_id = 0;
 
  memset (&ticker, 0, sizeof (ticker));
  ticker.sa_handler = &tick;
  sigaction (SIGALRM, &ticker, NULL);
    
  time_quantum.it_value.tv_sec = 0;
  time_quantum.it_value.tv_usec = 150;
    
  time_quantum.it_interval.tv_sec = 0;
  time_quantum.it_interval.tv_usec = 150;
    
  setitimer (ITIMER_REAL, &time_quantum, NULL);

  tcb *tb=(tcb*)malloc(sizeof(tcb)+1);
  tcbs[0]=tb;
  ucontext_t* context = (ucontext_t*)malloc(sizeof(ucontext_t));
  getcontext(context);
  tcbs[0]->context = context;
  // TODO: Implement your own code
  // HINT: 이 함수에서 tick 에 대해서 timer signal를 돌려 주어야함 -> 주기마다 tick이라는 함수가 호출 될수 있도록.
}

void cover_stub(void (*stub)(void*), void* args, struct tcb* tcbss){
  stub(args);
  tcbss->finished=1;
  printf("function#%d finished\n", tcbss->id);  
}

int mythread_create(void (*stub)(void*), void* args){
  static int tid = -1; // Thread ID
  n_tcbs++;
  tid = n_tcbs;
  tcbs[tid]->id = tid;

  ucontext_t* con=(ucontext_t*)malloc(sizeof(ucontext_t));

  if (getcontext(con) == -1){
    handle_error("getcontext");
  }
  con->uc_stack.ss_sp = tcbs[tid]->stack;
  con->uc_stack.ss_size = sizeof(tcbs[tid]->stack);

  if(sched_policy==MYTHREAD_FIFO){
    con->uc_link = tcbs[0]->context;
  }

  else{
    con->uc_link = tcbs[tid-1]->context;
  }

  tcbs[tid]->context = con;
  makecontext((tcbs[tid]->context), cover_stub, 3, stub, args, tcbs[tid]);
  // TODO: Implement your own code
  return tid;
}

void mythread_join(int tid){
  if(sched_policy==MYTHREAD_FIFO){
    swapcontext(tcbs[0]->context, tcbs[tid]->context);
    return;
  }

  else{
    while(1){
      if(tcbs[tid]->finished==1){
      break;
      }
    }
  }
// TODO: Implement your own code
}


