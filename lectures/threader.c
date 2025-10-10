#include <pthread.h>
#include <stdio.h>

// function pointer tha tthe thread calls
void *slave(void *arg) {
  char *str = arg;
  pthread_t tid = pthread_self();
  printf("hello there %ld\n", (long)tid);
}

int main() {
  char *msg = "main thread here!\n";

  // thread id
  pthread_t tid;

  pthread_create(&tid, NULL, slave, msg);

  pthread_exit(NULL);
  pthread_detach(tid);

  return 0;
}
