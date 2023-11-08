#include "pthread.h"
#include <iostream>
#include <pthread.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>

typedef struct {
  char id[15];
} thread_data_t;

void *routine(void *data) {

  std::cout << "Routine exécutée par " << ((thread_data_t *)data)->id
            << std::endl;

  return 0;
}

int main() {

  pthread_t threads[10];
  thread_data_t data[10];

  pthread_attr_t attributes;

  pthread_attr_init(&attributes);
  pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);

  std::srand(std::time(NULL));

  for (int i = 0; i < 10; i++) {

    snprintf(data[i].id, 15, "ID_%d_%d", i, std::rand());

    pthread_create(&threads[i], &attributes, routine, &data[i]);
  }

  void *status;

  for (int i = 0; i < 10; i++) {
    pthread_join(threads[i], &status);
  }

  pthread_attr_destroy(&attributes);

}