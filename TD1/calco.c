#include <stdio.h>

int main(){

    double nbre1, nbre2, result;
    char opera;

    printf(" *** Mini calco *** \n");

    printf("entrer le premier nombre : \n");
    if(scanf("%lf", &nbre1) != 1) {
        printf("Erreur");
        return 1;
    }

    printf("entrer le deuxième nombre : \n");
    if (scanf("%lf", &nbre2) != 1){
        printf("erreur");
        return 1;
    }
    
    printf("entrer un opérateur : + - * / \n");
    scanf(" %c", &opera);

    if (opera == '+'){
        result = nbre1 + nbre2;
    }else if (opera == '-'){
        result = nbre1 - nbre2;
    }else if (opera == '*'){
        result = nbre1 * nbre2;
    }else if (opera == '/');{
        if (nbre2 == 0){
            printf("erreur");
            return 1;
        }
        result = nbre1 / nbre2;

        } else {
            printf("erreur");
            return 1;
        } 
    }
    printf(" %2.lf Resultat : ", result);
    
    return 0;
}