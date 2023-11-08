# Threads
Un thread est un processus léger, il permet de paralléliser les opérations
## c-threads 
### Prérequis
Import : 
```cpp
#include <pthread.h>
```
Compilation :
```bash
-lpthread
```

### Types de données : 

```cpp
pthread_t thread; // Le thread en lui même, on le passe 
                  // par pointeur à pthread_create

pthread_attr_t attrs; // Les attributs du thread. 
                      // Permettent de définir :
                      //   - thread joinable ? 
                      //   - scope
                      //   - stack size 
                      // ...
```

### Fonctions : 
Initialiser les attributs
```cpp
int pthread_attr_init(pthread_attr_t * attrs);
```
Rendre le thread joinable
```cpp
int pthread_attr_setdetachstate(pthread_attr_t * attrs, PTHREAD_CREATE_JOINABLE)
```
Initialiser le thread
```cpp
int pthread_create(pthread_t * thread, pthread_attr_t * attrs, void * (*fonction_a_exec)(void *), void * args);
```
Attendre la fin de son exécution (il doit être joinable)
```cpp
int pthread_join(pthread_t * thread, void * status);
```
Arrêter le thread
```cpp
pthread_exit(void * status);
```

## C++ Threads

### Différences fondamentales

- La configuration est plus simple :
  - Pas de pointeur à allouer
  - Pas d'object configuration
  - Pas de struct data à créer
  - Joinable par défaut

## Fonctions : 
### Constructeur :
Crée et LANCE le thread directement.
```cpp
std::thread(
    Function function, // fonction à exécuter de façon asynchrone
    ...args // arguments de la fonction passée en paramètre
)
```
### Join
Bloque le master jusqu'à la fin d'exécution du thread.
```cpp
thread.join();
```
## Exemples : 

- [Un exemple simple de thread](exemples/threads/threads_simples.cpp)
- [Réductions](../exercices/reductions.cpp)

## Compiler et exécuter : 
```
g++ -lpthread threads_simples.cpp -o threads_simples
./threads_simples
```