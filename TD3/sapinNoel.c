#include <stdio.h>

int main() {
    int etages, i, j, k;

    printf("Veuillez entrer le nombre d'etages du sapin : ");
    scanf("%d", &etages);

    printf("\nVoici votre sapin de Noel :\n\n");

    int largeurMax = 1;
    for(int e = 1; e <= etages; e++) {
        largeurMax += 2 * (e + 1); 
    }

    int largeurLigne = 1; 

    for(int e = 1; e <= etages; e++) {
        int lignes = e + 1; 
        for(i = 0; i < lignes; i++) {
            int espaces = (largeurMax - largeurLigne) / 2;
            for(j = 0; j < espaces; j++) {
                printf(" ");
            }
            for(k = 0; k < largeurLigne; k++) {
                printf("*");
            }
            printf("\n");
            largeurLigne += 2;
        }
    }

    int troncHauteur = etages; 
    int troncLargeur = etages % 2 == 0 ? etages - 1 : etages; 
    int espacesTronc = (largeurMax - troncLargeur) / 2;

    for(i = 0; i < troncHauteur; i++) {
        for(j = 0; j < espacesTronc; j++) {
            printf(" ");
        }
        for(k = 0; k < troncLargeur; k++) {
            printf("*");
        }
        printf("\n");
    }

    return 0;
}
