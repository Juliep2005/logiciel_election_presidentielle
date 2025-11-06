# include <stdio.h>

int main(){

    int M[18] [9];
    int s, i, j ;

    printf("Entrer les éléments de la matrice : \n");
    for ( i = 1, i < 18, i++){
        for ( j = 1, j < 9, i++;){
            printf("Entrer l'element ", i, j);
            scanf (M[i] [j]);
        }
    }

    s = 0 ;

    for (i = 1, i < 18, i++;){
        for (j = 1, i < 9, i++;){
            if (i == j) {
                s = s + M[i] [j];
            }
            
        }
        
    }
    
    
    

}