#include <stdio.h>

int main() {
    int n, i;
    unsigned long long factorielle = 1; // Utilisation d'un type large pour les grandes factorielles

    // Demande du nombre
    printf("Entrez un nombre entier positif : ");
    scanf("%d", &n);

    // Vérification que le nombre est positif
    if (n < 0) {
        printf("Erreur : le factoriel d'un nombre negatif n'est pas defini.\n");
        return 1;
    }

    // Calcul de la factorielle
    for (i = 1; i <= n; i++) {
        factorielle *= i;
    }

    // Affichage du résultat
    printf("La factorielle de %d est : %llu\n", n, factorielle);

    return 0;
}
