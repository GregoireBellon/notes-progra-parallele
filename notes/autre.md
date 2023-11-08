# Autres notions

## Prendre des paramètres en ligne de commande

On utilise opt

```cpp
#include <getopt.h>

int main(int argc, char *argv[]) {

    int M,N,K;
  
  int opt;

  while ((opt = getopt(argc, argv, "m:n:k:")) != -1) {
    // remplacer m:n:k: par les lettres voulues
    switch(opt){
        case 'm':
            M = atoi(optarg);
            break;
        case 'n':
            N = atoi(optarg);
            break;
        case 'k':
            K = atoi(optarg);
            break;
    }
  }
}

```