#include <stdio.h>

int main() {
    int nombre, somme = 0;

    printf("Entrez des entiers (entrez -1 pour terminer) :\n");

    while (1) {
        scanf("%d", &nombre);

        if (nombre == -1) {
            break; 
        }

        somme += nombre; 
    }

    printf("La somme des entiers saisis est : %d\n", somme);

    return 0;
}
