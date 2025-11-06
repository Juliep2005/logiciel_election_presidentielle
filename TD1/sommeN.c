#include <stdio.h>

int main() {
    int n, nombre, somme = 0, compteur = 0;

    printf("Entrez le nombre d'entiers à saisir : ");
    scanf("%d", &n);

    while (compteur < n) {
        printf("Entrez un entier (ou -1 pour terminer) : ");
        scanf("%d", &nombre);

        if (nombre == -1) {
            break; 
        }

        somme += nombre; 
        compteur++;      
    }

    printf("La somme des entiers saisis est : %d\n", somme);

    return 0;
}
