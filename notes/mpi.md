# MPI
Permet de distribuer des calculs sur plusieurs coeurs, ou plusieurs machines.

## Pré-requis
### Compiler et exécuter
On utilise les outils spécifiques à MPI

On compile avec 
```
mpic++ src.cpp ... -o prog
```
On exécute avec :
```
mpirun -n 4 ./c3_mpi_ex_1.exe
```
n étant le nombre de coeurs à allouer au programme

### Boilerplate
```cpp
#include <mpi.h>

int main(int argc, char * argv[]){
    int max_cpus, cpu_rank; 
    char cpu_name[MPI::MAX_PROCESSOR_NAME];
    int name_length; 

    // Allocate ressources 
    MPI::Init(argc, argv);

    max_cpus = MPI::COMM_WORLD.Get_size();
    cpu_rank = MPI::COMM_WORLD.Get_rank();

    // get name
    MPI::Get_processor_name(cpu_name, name_length);

    // ... more instructions

    MPI::Finalize();
}
```
## Types de données : 

### MPI_Datatype
C'est une enum, les valeurs possibles sont : 
- MPI_SHORT
- MPI_INT
- MPI_LONG
- MPI_LONG_LONG
- MPI_UNSIGNED_CHAR
- MPI_UNSIGNED_SHORT
- MPI_UNSIGNED (équivaut unsigned int)
- MPI_UNSIGNED_LONG
- MPI_UNSIGNED_LONG_LONG
- MPI_FLOAT
- MPI_DOUBLE
- MPI_LONG_DOUBLE
- MPI_BYTE (équivaut à un char)

### MPI_Status

C'est une structure définie comme il suit : 
```cpp
typedef struct _MPI_Status {
  int count;
  int cancelled;
  int MPI_SOURCE;
  int MPI_TAG;
  int MPI_ERROR;
} MPI_Status, *PMPI_Status;
```

## Fonctions 

### Envoyer et recevoir

On utilise MPI_Send et MPI_Recv

```cpp
MPI::COMM_WORLD.Send(
    const void* buf,  // la donnée à envoyer (probablement un tableau)
	int count, // le nombre d'éléments du tableau
	MPI::Datatype& datatype, // le type de donnée (voir les [types de données](#mpi_datatype))
	int dest, // processus auquel envoyer la donnée
	int tag // identifiant arbitraire défini par l'utilisateur 
    );

MPI::COMM_WORLD.Recv(
    void* buf, // la donnée reçue (probablement un tableau)
	int count, // le nombre d'éléments du tableau
	MPI::Datatype& datatype, // le type de donnée reçu (voir les [types de données](#mpi_datatype))
	int source, // processus que l'on écoute 
	int tag, // identifiant arbitraire défini par l'utilisateur 
	MPI::Status* status // statut (voir les [types de données](#mpi_status))
    )
```

Généralement, on défini le cpu 0 comme le master, et les autres comme les slaves.

Donc pour envoyer un message du master à tous les slaves, on écrira : 

```cpp
if(cpu_rank == 0 ){

    for(int i = 1 ; i < max_cpus; i++){
        MPI::COMM_WORLD.Send(
        buf,  // la donnée à envoyer (probablement un tableau)
        count, // le nombre d'éléments du tableau
        datatype, // le type de donnée (voir les [types de données](#mpi_datatype))
        i, // processus auquel envoyer la donnée
        tag // identifiant arbitraire défini par l'utilisateur 
        );
    }
    
}
```

## Exemples : 
- [Send et Recv](../exercices/MPI/MPI_Syracuse.cpp)
- [Recherche à l'intérieur d'un tableau](../exercices/MPI/MPI_Recherche_tableau.cpp)