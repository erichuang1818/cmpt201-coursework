#include <pthread.h>
#include <stdio.h>

int cnt = 0;

void *func(void *arg) {
  for (int i = 0; i < 10000000; i++)
    cnt++;
}

int main() {

  pthread_t tid;
  void *res;
  int s;

  pthread_t tid2;
  void *res2;
  int s2;

  pthread_create(&tid, NULL, func, "t1\n");
  s = pthread_join(tid, &res);
  if (s != 0)
    perror("pthread join");
  pthread_join(tid, res);

  pthread_create(&tid2, NULL, func, "t2\n");
  s = pthread_join(tid2, &res2);
  if (s2 != 0)
    perror("pthread join");
  pthread_join(tid2, res);

  printf("%d", cnt);
}
