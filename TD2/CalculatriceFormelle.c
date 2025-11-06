#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
    float nbre1, nbre2, resultat, a, nombre, racine;
    char operation, continuer;
    int i, fact, n, mod, ppcm, nombre1, nombre2;

    do {
        printf("\033[1;36m\n===================== CALCULATRICE SCIENTIFIQUE =====================\033[0m\n");
        printf("\033[1;33m|  [+] Addition                  |  [-] Soustraction               |\n");
        printf("|  [*] Multiplication            |  [/] Division                   |\n");
        printf("|  [!] Factoriel                 |  [^] Puissance                  |\n");
        printf("|  [#] Racine n-ieme             |  [m] Modulo                     |\n");
        printf("|  [p] PGCD                      |  [c] PPCM                       |\n");
        printf("|  [e] Exponentiel               |  [l] Logarithme                 |\n");
        printf("|  [q] Quitter la calculatrice                                     |\033[0m\n");
        printf("\033[1;36m======================================================================\033[0m\n");
        printf("\033[1;32m→ Entrez votre choix d'operation : \033[0m");
        scanf(" %c", &operation);

        if (operation == 'q' || operation == 'Q') {
            printf("\033[1;35mAu revoir ! Merci d'avoir utilise la calculatrice.\033[0m\n");
            break;  
        }

        if (operation != '+' && operation != '-' && operation != '*' &&
            operation != '/' && operation != '!' && operation != '^' && operation != '#' &&
            operation != 'm' && operation != 'p' && operation != 'c' && 
            operation != 'e' && operation != 'l') {
            printf("\033[1;31m\n❌ Erreur : veuillez entrer une operation valide.\033[0m\n");
            continue;
        }

        switch (operation) {
            case '+':
                printf("Entrer les valeurs de vos deux nombres\n");
                scanf("%f %f", &nbre1, &nbre2);
                resultat = nbre1 + nbre2;
                printf("%.2f + %.2f = %.2f\n", nbre1, nbre2, resultat);
                break;

            case '-':
                printf("Entrer les valeurs de vos deux nombres\n");
                scanf("%f %f", &nbre1, &nbre2);
                resultat = nbre1 - nbre2;
                printf("%.2f - %.2f = %.2f\n", nbre1, nbre2, resultat);
                break;

            case '*':
                printf("Entrer les valeurs de vos deux nombres\n");
                scanf("%f %f", &nbre1, &nbre2);
                resultat = nbre1 * nbre2;
                printf("%.2f * %.2f = %.2f\n", nbre1, nbre2, resultat);
                break;

            case '/':
                printf("Entrer les valeurs de vos deux nombres\n");
                scanf("%f %f", &nbre1, &nbre2); 
                if(nbre2 != 0) {
                    resultat = nbre1 / nbre2;
                    printf("%.2f / %.2f = %.2f\n", nbre1, nbre2, resultat);
                } else {
                    printf("Erreur : un nombre ne peut etre divisible par 0.\n");
                }
                break;

            case '!':
                printf("Entrer un nombre\n");
                scanf("%d", &nbre1);
                if (nbre1 < 0) {
                    printf("Erreur : le factoriel d'un nombre negatif n'est pas defini.\n");
                } else if (nbre1 == 0) {
                    fact = 1;
                    printf("le factoriel de %d vaut : %d\n", nbre1, fact);
                } else {
                    fact = 1;
                    for (i = 1; i <= nbre1; i++) fact *= i; 
                    printf("Le factoriel de %d vaut : %d\n", nbre1, fact);
                }
                break;

            case '^':
                printf("Entrer un nombre\n");
                scanf("%f", &nbre1);
                printf("Entrer l'exposant n\n");
                scanf("%d", &n);
                resultat = 1;
                if (n == 0) {
                    resultat = 1;
                } else if (n > 0) {
                    for (i = 1; i <= n; i++) {
                        resultat *= nbre1;
                    }
                } else {
                    for (i = 1; i <= -n; i++) {
                        resultat *= nbre1;
                    }
                    
                    resultat = 1 / resultat;
                }
                printf("Le resultat de %f ^ %d vaut %f\n", nbre1, n, resultat);
                break;

            case '#':
                printf("Entrer un nombre\n");
                scanf("%f", &nbre1);
                printf("Entrer le degre de la racine\n");
                scanf("%d", &n);

                if (n == 0){
                    printf("Erreur: n ne peut etre nul.\n");
                } else if (nbre1 < 0 && fmod(n, 2) == 0){
                    printf("La racine paire d'un nombre negatif n'est pas definit\n");
                } else if (nbre1 < 0) {
                    racine = -pow(-nbre1, 1.0/n);
                    printf("la racine %d ieme de %f vaut %f\n", n, nbre1, racine);
                } else {
                    racine = pow(nbre1, 1.0/n);
                    printf("La racine %d ieme de %f vaut %f\n", n, nbre1, racine);
                }
                break;

            case 'm':
                printf("Entrer la valeur de vos deux nombres:\n");
                scanf("%f %f", &nbre1, &nbre2);
                mod = nombre1 % nombre2;
                printf("Le reste de la division entre %d et %d vaut: %d\n", nombre1, nombre2, mod);
                break;

            case 'p': 
                
                printf("Entrer vos deux nombres:\n");
                scanf("%d %d", &nombre1, &nombre2);
                int nbre1_uti = nombre1;
                int nbre2_uti = nombre2;

                if (nombre1 < 0) {
                    nombre1 =- nombre1;
                }
                if (nombre2 < 0) {
                    nombre2 =- nombre2;
                }

                while (nombre2 != 0) {
                    mod = nombre1 % nombre2;
                    nombre1 = nombre2;
                    nombre2 = mod;
                }

                printf("Le pgcd de %d et %d vaut: %d\n", nbre1_uti, nbre2_uti, nombre1);
                break;

            case 'c': 
                printf("Entrer vos deux nombres : \n");
                scanf("%d %d", &nombre1, &nombre2);
                int nombre1_uti = nombre1;
                int nombre2_uti = nombre2;

                if (nombre1 < 0) {
                    nombre1 =- nombre1;
                }
                if (nombre2 < 0) {
                    nombre2 =- nombre2;
                }

                while (nombre2 != 0) {
                    mod = nombre1 % nombre2;
                    nombre1 = nombre2;
                    nombre2 = mod;
                }
                ppcm = (abs(nombre1_uti * nombre2_uti)) / nombre1;
                printf("Le ppcm de %d et %d vaut : %d\n", nombre1_uti, nombre2_uti, ppcm);
                break;

            case 'e':
                printf("Entrer un nombre de votre choix : \n");
                scanf("%f", &nbre1);
                resultat = exp(nbre1);
                printf("L'exponentiel de %f vaut: %f\n", nbre1, resultat);
                break;

            case 'l':
                printf("Entrer un nombre de votre choix : \n");
                scanf("%f", &nbre1);
                if (nbre1 <= 0) printf("Le logarithme des nombres negatifs et nuls n'est pas definit\n");
                else {
                    nombre = log(nbre1);
                    resultat = log10(nbre1);
                    printf("Le logarithme decimal log(%f) = %f\n", nbre1, resultat);
                    printf("Le logarithme naturel ln(%f) = %f\n", nbre1, nombre);
                }
                break;
        }
        
        if (operation != 'q' && operation != 'Q') {
            printf("\nVoulez-vous continuez a faire des opérations? ('o' pour oui ou 'n' pour non)\n");
            scanf(" %c", &continuer);
        }

    } while (continuer == 'o' || continuer == 'O');

    return 0;
}
