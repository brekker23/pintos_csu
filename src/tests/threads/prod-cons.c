#include <stdio.h>
#include <string.h>
//#include <errno.h>
#include <threads/thread.h> // change
#include <threads/synch.h> // no change?
#define N 10
#define BUFSIZE 3
static int buffer[BUFSIZE];
static int bufin = 0, bufout = 0;

static struct semaphore empty, full;
struct lock buffer_lock;

static void *producer(void *arg1) {
   int i, item;

   for (i = 0; i < N; i++) {
      sem_down(&empty);
      item = i*i*i;
      lock_acquire(&buffer_lock);
      buffer[bufin] = item;    
      bufin = (bufin + 1) % BUFSIZE;
      lock_release(&buffer_lock);
      printf("p: put item %d\n", item); 
      sem_up(&full);
   }   
   return NULL;
}

static void *consumer(void *arg2) {
   int i, item;

   for (i = 0; i < N; i++) {
      sem_down(&full);
      lock_acquire(&buffer_lock);
      item = buffer[bufout];
      bufout = (bufout + 1) % BUFSIZE;
      lock_release(&buffer_lock);
      printf("c: get item %d\n",item);
      sem_up(&empty);
   }   
   return NULL;
}
 
void test_prod_cons(void) {
   lock_init(&buffer_lock);
   sem_init(&full, 0);
   sem_init(&empty, BUFSIZE);
   thread_create("producer", PRTY_DEFAULT, producer, NULL);
   thread_create("consumer", PRTY_DEFAULT, consumer, NULL);
   thread_exit();
}

