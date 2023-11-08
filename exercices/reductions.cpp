// EXERCICE TIRÉ DE
// https://leria-info.univ-angers.fr/~jeanmichel.richer/ensm2_para_td2.php Les
// méthodes à implanter sont les suivantes :
// méthode 1 : réduction classique sans optimisation
// méthode 2 : réduction en utilisant les POSIX Threads avec 8 threads
// méthode 3 : version utilisant la classe thread de C++11
// 8 threads et répartition des indices
// méthode 4 : version OpenMP avec 8 threads

#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <getopt.h>
#include <iostream>
#include <math.h>
#include <ostream>
#include <pthread.h>
#include <thread>
#include <vector>

typedef double real;
typedef real (*ReductionMethod)(real *t, size_t n);

real function(real v) {
  for (int i = 0; i < 100; ++i) {
    v = sin(v + 0.01);
  }
  return v;
}

// méthode 1 : réduction classique sans optimisation
real reduction_serial(real *t, size_t n) {
  real sum = 0;
  for (size_t i = 0; i < n; ++i)
    sum += function(t[i]);
  return sum;
}

typedef struct {
  real *t;
  size_t n;
  int thread_number;
  real result;
} arg_thread_c;

// méthode 2 : réduction en utilisant les POSIX Threads avec 8 threads
void *reduction_threads_cb(void *args) {
  arg_thread_c *data = (arg_thread_c *)args;
  data->result = 0;
  // diviser le tableau en 8
  // faire le calcul
  for (int i = 0; i < data->n; i++) {
    if (i % 8 == data->thread_number) {
      data->result += function(data->t[i]);
    }
  }

  return data;
}

real reduction_threads(real *t, size_t n) {

  // initialiser les threads

  pthread_t threads[8];
  arg_thread_c thread_data[8];
  pthread_attr_t attrs;
  pthread_attr_init(&attrs);
  pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE);

  for (int i = 0; i < 8; i++) {
    thread_data[i].n = n;
    thread_data[i].t = t;
    thread_data[i].thread_number = i;

    pthread_create(&threads[i], &attrs, reduction_threads_cb, &thread_data[i]);
  }

  void *status;

  real result = 0;

  for (int i = 0; i < 8; i++) {
    // On additionne les résultats
    pthread_join(threads[i], &status);
    result += ((arg_thread_c *)status)->result;
  }

  pthread_attr_destroy(&attrs);

  return result;
}

void reduction_treads_cpp_cb(real *t, size_t n, int thread_number,
                             real *result) {
  *result = 0;
  for (int i = 0; i < n; i++) {
    if (i % 8 == thread_number) {
      *result += function(t[i]);
    }
  }
}

real reduction_threads_cpp(real *t, size_t n) {
  std::vector<std::thread> threads;

  real results[8];

  for (int i = 0; i < 8; i++) {
    threads.push_back(
        std::thread(reduction_treads_cpp_cb, t, n, i, &results[i]));
  }

  real result = 0;

  for (int i = 0; i < 8; i++) {
    threads.at(i).join();
    result += results[i];
  }

  return result;
}

// méthode 4 : version OpenMP avec 8 threads
real reduction_open_mp(real *t, size_t n) {
  real sum = 0;

#pragma omp parallel for reduction(+ : sum)
  for (size_t i = 0; i < n; ++i)
    sum += function(t[i]);

  return sum;
}

int main(int argc, char *argv[]) {

  int methode = 1;
  int array_size = 1 << 20;

  int opt;

  while ((opt = getopt(argc, argv, "m:n:")) != -1) {
    // remplacer m:n:k: par les lettres voulues
    switch (opt) {
    case 'm':
      methode = atoi(optarg);
      break;
    case 'n':
      array_size = atoi(optarg);
      break;
    }
  }

  real *arr = new real[array_size];

  srand(time(NULL));

  for (int i = 0; i < array_size; i++) {
    arr[i] = rand();
  }

  real result = 0;

  switch (methode) {
  case 1:
    std::cout << "Utilisation de la méthode 1 (cthreads)" << std::endl;
    result = reduction_threads(arr, array_size);
    break;
  case 2:
    std::cout << "Utilisation de la méthode 2 (cpp threads)" << std::endl;
    result = reduction_threads_cpp(arr, array_size);
    break;
  case 3:
    std::cout << "Utilisation de la méthode 3 (openMP)" << std::endl;
    result = reduction_open_mp(arr, array_size);
    break;
  }

  delete[] arr;
  std::cout << "Resultat " << result;
}