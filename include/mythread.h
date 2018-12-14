#ifndef MYTHREAD_H_
#define MYTHREAD_H_

enum mythread_scheduling_policy {
  MYTHREAD_RR,
  MYTHREAD_FIFO
};

/**
 * mythread_init: 우리 user level thread library를 initialize 하는 함수
 * Scheduling 원칙 (e.g. First-in First-out, Round robin) 을 초기화 해주고,
 * 추후 mythread_create() 을 통해 만들어질 thread들을 위한 자료구조를 initialize 한다.
 */
void mythread_init(enum mythread_scheduling_policy policy);

/**
 * mythread_create: thread를 생성하는 함수. 
 * stub 내용이 user level thread에 의해 실행된다. 해당 함수에 인자로 args를 넘겨주며, thread id를 return 한다.
 */
int mythread_create(void (*stub)(void*), void* args);

/**
 * mythread_join: 이 함수를 부른 thread는 tid의 thread가 모두 끝나기 전에 기다린다.
 */
void mythread_join(int tid);

#endif // MYTHREAD_H_
