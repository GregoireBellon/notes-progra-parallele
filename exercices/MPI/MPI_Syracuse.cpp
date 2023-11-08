/*
https://leria-info.univ-angers.fr/~jeanmichel.richer/ensm2_para_crs5.php

Exercice 5.1

Une suite de Syracuse est une suite d'entiers naturels définie de la manière suivante : on part d'un nombre entier strictement positif

    s'il est pair, on le divise par 2
    s'il est impair, on le multiplie par 3 et on ajoute 1

Cette suite possède la propriété de converger vers 1 après un certain nombre d'étapes.

Mettre en place une solution MPI avec trois instances :

    le maître génére un nombre entier aléatoire x
    si le nombre x est pair, il l'envoie à l'esclave n°1 qui retourne x2
    si le nombre x est impair, il l'envoie à l'esclave n°2 qui retourne 3x+1
    à chaque étape, le maître affiche la nouvelle valeur de x

Le maître comptera le nombre d'appels à l'esclave n°1 et l'esclave n°2.

Lorsque le maître recoît la valeur 1, il s'arrête et envoie un code d'arrêt aux esclaves (un nombre négatif par exemple). Puis il affiche le nombre d'appels à chaque esclave.

*/


#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mpi.h>
#include <ostream>

void slave_pair() {
  // reçoit le nombre
  int *nombre_recu;
  MPI::Status status;

  while (true) {
    MPI::COMM_WORLD.Recv(nombre_recu, 1, MPI::INT, 0, 1, status);

    if (*nombre_recu == -1) {
      break;
    }

    // le divise par deux
    *nombre_recu = *nombre_recu / 2;

    // le renvoie
    MPI::COMM_WORLD.Send(nombre_recu, 1, MPI::INT, 0, 1);
  }
}

void slave_impair() {
  // reçoit le nombre
  int *nombre_recu;
  MPI::Status status;

  while (true) {
    MPI::COMM_WORLD.Recv(nombre_recu, 1, MPI::INT, 0, 1, status);

    if (*nombre_recu == -1) {
      break;
    }

    // le transforme en 3x + 1
    *nombre_recu = 3 * (*nombre_recu) + 1;

    // le renvoie
    MPI::COMM_WORLD.Send(nombre_recu, 1, MPI::INT, 0, 1);
  }
}

void master() {

  // assigne à nombre une val aléatoire entre 500 et 10.000
  unsigned int nombre;
  unsigned int dernier_com_envoi;

  std::srand(std::time(nullptr));

  nombre = (std::rand() % 9500) + 500;

  // annonce la valeur du nombre
  std::cout << "Nombre généré : " << nombre << std::endl;

  MPI::Status status;

  // tant que nombre != 1{
  while (nombre != 1) {

    // si pair envoie à slave pair
    if (nombre % 2 == 0) {
      dernier_com_envoi = 2;
    } else {
      // si impair....impair
      dernier_com_envoi = 1;
    }

    MPI::COMM_WORLD.Send(&nombre, 1, MPI::INT, dernier_com_envoi, 1);

    // reçoit le nouveau nombre
    MPI::COMM_WORLD.Recv(&nombre, 1, MPI::INT, dernier_com_envoi, 1, status);

    // annonce la valeur du nouveau nombre
    std::cout << "Le nouveau nombre est " << nombre << std::endl;
  }

  // envoi un signal d'arret;
  nombre = -1;
  for (int i = 1; i <= 2; i++) {
    MPI::COMM_WORLD.Send(&nombre, 1, MPI::INT, i, 1);
  }
}

int main(int argc, char *argv[]) {
  int max_cpus, cpu_rank;

  // Allocate ressources
  MPI::Init(argc, argv);

  max_cpus = MPI::COMM_WORLD.Get_size();
  cpu_rank = MPI::COMM_WORLD.Get_rank();

  // get name

  switch (cpu_rank) {
  case 0:
    master();
    break;
  case 1:
    slave_impair();
    break;
  case 2:
    slave_pair();
    break;
  default:
    std::cout << "Trop de CPU alloués ! " << std::endl
              << "3 sont suffisants !" << std::endl;
  }

  MPI::Finalize();

  std::cout << "Programme terminé !" << std::endl;
}