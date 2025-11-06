#include <stdio.h>

int main() {
    int n, i, somme = 0;

    printf("Entrez un nombre entier positif : ");
    scanf("%d", &n);

    if (n < 0) {
        printf("Erreur : le nombre doit être positif.\n");
        return 1;
    }

    for (i = 1; i <= n; i++) {
        somme += i;
    }

    printf("La somme des entiers de 1 à %d est : %d\n", n, somme);

    return 0;
}
