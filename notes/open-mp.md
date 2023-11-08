# Open MP
Permet de paralléliser facilement les opérations via des instructions au compilateur

## Prérequis 
On compile avec 
```
-fopenmp
```
On défini le nombre de threads que openMP utilise avec 
```
export OMP_NUM_THREADS=4
```

## Instructions : 
On utilise openmp avec #pragma omp {instruction(s)}

Parmis les instructions disponibles : 

```cpp
// exécute le bloc de code suivant en utilisant chaque thread
#pragma omp parallel
```
```cpp
// Permet de séquencialiser le bloc de code, pour éviter des écritures concurentes par exemple. 
#pragma omp critical
```
```cpp
// Permet de paralléliser une boucle for
#pragma omp for
```
```cpp
// Permet de déplier une boucle (Les paramètres de la boucle doivent être connus à la compilation)
#pragma omp collapse(2)
    for (int i=0; i<MAX_I; ++i) {
        for (int j=0; j<MAX_J; ++j) {
        // ...
        }
    }
```
## Exemples : 
- [Réductions](../exercices/reductions.cpp)



