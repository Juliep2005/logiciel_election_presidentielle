#include <stdio.h>

int main() {
    int N, NB = 0, NR;

    printf("Entrez le nombre total d'oeufs : ");
    scanf("%d", &N);

    NR = N; 

    while (NR >= 12) {
        NR -= 12;
        NB++; 
    }

    printf("Nombre de boîtes remplies : %d\n", NB);
    printf("Nombre d'œufs restants : %d\n", NR);

    return 0;
}
