#include <stdio.h>

unsigned long long factoriel (int n){
    if (n < 0) return 0;
    unsigned long long resultat = 1;
    
    for (int i = 1; i <= n; i++){
        resultat *= i;
    }
    return resultat;
}

int main() {
    double nombre1, nombre2, resultat;
    char operateur;

    printf("=== Mini Calculatrice ===\n");

    // Lire le premier nombre
    printf("Entrez le premier nombre : ");
    if (scanf("%lf", &nombre1) != 1) {  // Vérifie que scanf a bien lu un nombre
        printf("Erreur : ce n'est pas un nombre.\n");
        return 1;  // quitte le programme si ce n’est pas un nombre
    }

    // Lire le deuxième nombre
    printf("Entrez le deuxième nombre : ");
    if (scanf("%lf", &nombre2) != 1) {  // Vérifie que scanf a bien lu un nombre
        printf("Erreur : ce n'est pas un nombre.\n");
        return 1;  // quitte le programme si ce n’est pas un nombre
    }

    // Lire l'opérateur
    printf("Entrez l'opérateur (+, -, *, /) : ");
    scanf(" %c", &operateur);  // espace avant %c pour ignorer les retours à la ligne

    // Effectuer le calcul
    if (operateur == '+') {
        resultat = nombre1 + nombre2;
    } else if (operateur == '-') {
        resultat = nombre1 - nombre2;
    } else if (operateur == '*') {
        resultat = nombre1 * nombre2;
    } else if (operateur == '/') {
        if (nombre2 == 0) {
            printf("Erreur : division par zéro !\n");
            return 1;
        }
        resultat = nombre1 / nombre2;
    } else if (operateur == '!') {
        // Factoriel : on prend seulement la partie entière du premier nombre
        int n = (int)nombre1;
        if (n < 0) {
            printf("Erreur : factoriel non défini pour un nombre négatif.\n");
        } else {
            unsigned long long f = factoriel(n);
            printf("Factoriel de %d = %llu\n", n, f);
        }
    } else {
        printf("Erreur : opérateur invalide !\n");
        return 1;
    }

    printf("Résultat : %.2lf\n", resultat);

    return 0;
}
