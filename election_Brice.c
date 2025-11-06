#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CANDIDATS 100
#define MAX_ELECTEURS 100

struct Candidat {
    int id;
    char nom[100];
    char parti[50];
    int votes;
    int est_rejete; 
};

struct Electeur {
    int id;
    char nom[100];
    int a_vote; 
    int candidat_vote; 
};

void saisirCandidats(struct Candidat *candidats, int *nbre_candidats) {
    printf("Entrez le nombre de candidats: ");
    scanf("%d", nbre_candidats);
    getchar(); 

    for (int i = 0; i < *nbre_candidats; i++) {
        printf("Candidat %d:\n", i + 1);
        printf("ID: ");
        scanf("%d", &candidats[i].id);
        getchar(); 

        printf("Nom: ");
        fgets(candidats[i].nom, sizeof(candidats[i].nom), stdin);
        candidats[i].nom[strcspn(candidats[i].nom, "\n")] = 0; 

        printf("Parti: ");
        fgets(candidats[i].parti, sizeof(candidats[i].parti), stdin);
        candidats[i].parti[strcspn(candidats[i].parti, "\n")] = 0;
        candidats[i].votes = 0;
        candidats[i].est_rejete = 0; 
    }

    for (int i = 0; i < *nbre_candidats; i++) {
        for (int j = i + 1; j < *nbre_candidats; j++) {
            if (candidats[i].id == candidats[j].id || strcmp(candidats[i].parti, candidats[j].parti) == 0) {
                candidats[i].est_rejete = 1;
                candidats[j].est_rejete = 1;
            }
        }
    }
}

void saisirElecteurs(struct Electeur *electeurs, int *nbre_electeurs) {
    printf("Entrez le nombre d'electeurs: ");
    scanf("%d", nbre_electeurs);
    getchar(); 
    for (int i = 0; i < *nbre_electeurs; i++) {
        printf("Electeur %d:\n", i + 1);
        printf("ID: ");
        scanf("%d", &electeurs[i].id);
        getchar(); 

        printf("Nom: ");
        fgets(electeurs[i].nom, sizeof(electeurs[i].nom), stdin);
        electeurs[i].nom[strcspn(electeurs[i].nom, "\n")] = 0; 

        electeurs[i].a_vote = 0;
        electeurs[i].candidat_vote = -1;
    }
}

void afficherCandidats(struct Candidat *candidats, int nbre_candidats) {
    printf("Candidats disponibles:\n");
    for (int i = 0; i < nbre_candidats; i++) {
        if (candidats[i].est_rejete == 0) {
            printf("ID: %d, Nom: %s, Parti: %s\n", candidats[i].id, candidats[i].nom, candidats[i].parti);
        } else {
            printf("Candidat: %s (Rejeté)\n", candidats[i].nom);
        }
    }
}

void enregistrerVotes(struct Electeur *electeurs, int nbre_electeurs, struct Candidat *candidats, int nbre_candidats) {
    for (int i = 0; i < nbre_electeurs; i++) {
        if (electeurs[i].a_vote == 0) { 
            printf("\n%s, veuillez voter:\n", electeurs[i].nom);
            afficherCandidats(candidats, nbre_candidats); 
            int id_candidat;
            printf("Entrez l'ID du candidat pour lequel vous votez: ");
            scanf("%d", &id_candidat);
            for (int j = 0; j < nbre_candidats; j++) {
                if (candidats[j].id == id_candidat && candidats[j].est_rejete == 0) {
                    candidats[j].votes++;
                    electeurs[i].a_vote = 1; 
                    electeurs[i].candidat_vote = id_candidat;
                    printf("Vote enregistré pour %s.\n", candidats[j].nom);
                    break;
                }
            }
        }
    }
}

void afficherResultats(struct Candidat *candidats, int nbre_candidats) {
    int total_votes = 0;
    for (int i = 0; i < nbre_candidats; i++) {
        if (candidats[i].est_rejete == 0) {
            total_votes += candidats[i].votes;
        }
    }

    printf("\nResultats des votes:\n");
    for (int i = 0; i < nbre_candidats; i++) {
        if (candidats[i].est_rejete == 0) {
            double pourcentage = total_votes > 0 ? (double)candidats[i].votes / total_votes * 100 : 0;
            printf("Candidat: %s, Votes: %d, Pourcentage: %.2f%%\n", candidats[i].nom, candidats[i].votes, pourcentage);
        } else {
            printf("Candidat: %s (Rejeté)\n", candidats[i].nom);
        }
    }
    printf("\nCandidats dont la caution sera remboursee (30%% ou plus):\n");
    for (int i = 0; i < nbre_candidats; i++) {
        if (candidats[i].est_rejete == 0) {
            double pourcentage = total_votes > 0 ? (double)candidats[i].votes / total_votes * 100 : 0;
            if (pourcentage >= 30) {
                printf("Candidat: %s, Votes: %d, Pourcentage: %.2f%%\n", candidats[i].nom, candidats[i].votes, pourcentage);
            }
        }
    }
    printf("\nCandidats rejetes et raisons:\n");
    for (int i = 0; i < nbre_candidats; i++) {
        if (candidats[i].est_rejete == 1) {
            printf("Candidat: %s, Raison: ID ou parti en double\n", candidats[i].nom);
        }
    }
}

int main() {
    struct Candidat candidats[MAX_CANDIDATS];
    struct Electeur electeurs[MAX_ELECTEURS];
    int nbre_candidats, nbre_electeurs;

    saisirCandidats(candidats, &nbre_candidats);
    saisirElecteurs(electeurs, &nbre_electeurs);
    enregistrerVotes(electeurs, nbre_electeurs, candidats, nbre_candidats);
    afficherResultats(candidats, nbre_candidats);

    return 0;
}