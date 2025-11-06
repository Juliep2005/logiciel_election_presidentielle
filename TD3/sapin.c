#include <stdio.h>

int main() {
    int taille, i, j, k, t;

    printf("Entrer la taille du sapin : ");
    scanf("%d", &taille);

    for (i = 1; i <= taille; i++) {
        for (j = 1; j <= taille - i; j++) {
            printf(" ");
        }
        for (k = 1; k <= 2 * i - 1; k++) {
            printf("*");
        }
        printf("\n");
    }
    for (t = 1; t <= taille/2; t++) {
        for (j = 1; j <= taille - 1; j++) {
            printf(" ");
        }
        printf("**\n");
    }

    return 0;
}
