/*
https://leria-info.univ-angers.fr/~jeanmichel.richer/ensm2_para_crs5.php

Exercice 5.2

Objectif : Écrire un programme MPI pour rechercher un élément dans un tableau.

Description :

    créer 5 processus
    générer un tableau d'un million d'entiers aléatoires sur le processus maître (processus de rang 0)
    répartir le tableau entre les différents processus esclaves en utilisant la fonction MPI_Scatter
    le maître génère un nombre aléatoire x et demande à chaque esclave de rechercher le nombre d'occurrences de x dans son tableau et de retourner le résultat
    afficher sur le maître, le nombre d'occurrences par esclave

*/


#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mpi.h>

int rechercheOccurence(int *array, int size, int nombreRecherche) {

  int occurences = 0;

  for (int i = 0; i < size; i++) {
    if (array[i] == nombreRecherche)
      occurences++;
  }

  return occurences;
}

int * generateRandomArray(int size) {

  int *array = new int[size];

  std::srand(std::time(nullptr));

  for (int i = 0; i < size; i++) {
    array[i] = std::rand() % 10000;
  }

  return array;
}

int main(int argc, char *argv[]) {
  int max_cpus, cpu_rank;
  // Allocate ressources
  MPI::Init(argc, argv);

  max_cpus = MPI::COMM_WORLD.Get_size();
  cpu_rank = MPI::COMM_WORLD.Get_rank();

  int *array = nullptr;

  const long arr_size = 10000000;
  const int nombre_a_rechercher = atoi(argv[1]);

  if (cpu_rank == 0) {
    array = generateRandomArray(arr_size);
    assert(array != nullptr);
    std::cout << "Tableau de taille " << arr_size << " généré" << std::endl;
  }

  const int elements_par_proc = arr_size / max_cpus;

  int *rec_array = new int[elements_par_proc];
  assert(rec_array != nullptr);


  MPI::COMM_WORLD.Scatter(array, elements_par_proc, MPI::INT, rec_array,
                          elements_par_proc, MPI::INT, 0);


  int result =
      rechercheOccurence(rec_array, elements_par_proc, nombre_a_rechercher);

  int *somme = nullptr;

  if (cpu_rank == 0) {
    somme = new int[max_cpus];
  }

  MPI::COMM_WORLD.Gather(&result, 1, MPI::INT, somme, 1, MPI::INT, 0);

  if (cpu_rank == 0) {

    int somme_occurences = 0;

    for (int i = 0; i < max_cpus; i++) {
      somme_occurences += somme[i];
    }

    std::cout << somme_occurences << " occurences trouvées" << std::endl;

    delete[] array;
    delete[] somme;
  }

  MPI::Finalize();

  exit(0);
}