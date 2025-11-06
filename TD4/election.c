#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"

Electeur electeurs[MAX_ELECTEURS];
Candidat candidats[MAX_CANDIDATS];
int nb_electeurs = 0;
int nb_candidats = 0;

void charger_donnees() {
    FILE *fe = fopen("electeurs.txt", "r");
    FILE *fc = fopen("candidats.txt", "r");

    if (fe) {
        while (fscanf(fe, "%49s %49s %49s %d %d\n",
                      electeurs[nb_electeurs].nom,
                      electeurs[nb_electeurs].identifiant,
                      electeurs[nb_electeurs].quartier,
                      &electeurs[nb_electeurs].age,
                      &electeurs[nb_electeurs].a_vote) == 5)
        {
            nb_electeurs++;
        }
        fclose(fe);
    }

    if (fc) {
        while (fscanf(fc, "%49s %49s %19s %49s %d %d %d %[^\n]\n",
                      candidats[nb_candidats].nom,
                      candidats[nb_candidats].identifiant,
                      candidats[nb_candidats].date_naissance,
                      candidats[nb_candidats].parti,
                      &candidats[nb_candidats].age,
                      &candidats[nb_candidats].votes,
                      &candidats[nb_candidats].accepte,
                      candidats[nb_candidats].motif_rejet) == 8)
        {
            nb_candidats++;
        }
        fclose(fc);
    }
}

void sauvegarder_donnees() {
    FILE *fe = fopen("electeurs.txt", "w");
    FILE *fc = fopen("candidats.txt", "w");

    if (fe) {
        for (int i = 0; i < nb_electeurs; i++) {
            fprintf(fe, "%s %s %s %d %d\n",
                    electeurs[i].nom,
                    electeurs[i].id,
                    electeurs[i].quartier,
                    electeurs[i].age,
                    electeurs[i].a_vote);
        }
        fclose(fe);
    }

    if (fc) {
        for (int i = 0; i < nb_candidats; i++) {
            fprintf(fc, "%s %s %s %s %d %d %d %s\n",
                    candidats[i].nom,
                    candidats[i].id,
                    candidats[i].date_naissance,
                    candidats[i].parti,
                    candidats[i].age,
                    candidats[i].votes,
                    candidats[i].accepte,
                    candidats[i].motif_rejet);
        }
        fclose(fc);
    }
}

void inscrire_electeur(void) {
    if (nb_electeurs >= MAX_ELECTEURS) {
        printf(" \n❌ Nombre maximal d'electeurs atteint.\n");
        return;
    }

    Electeur e;
    printf("\n=== INSCRIPTION ELECTEUR ===\n");
    printf("Nom : ");
    fgets(e.nom, NOM_MAX, stdin); e.nom[strcspn(e.nom, "\n")] = 0;

    printf("Identifiant unique : ");
    fgets(e.id, ID_MAX, stdin); e.id[strcspn(e.id, "\n")] = 0;

    // Vérifie identifiant unique
    for (int i = 0; i < nb_electeurs; i++) {
        if (strcmp(electeurs[i].id, e.id) == 0) {
            printf("\n❌ Cet identifiant existe deja.\n");
            return;
        }
    }

    printf("Quartier : ");
    fgets(e.quartier, QUARTIER_MAX, stdin); e.quartier[strcspn(e.quartier, "\n")] = 0;

    printf("Age : ");
    scanf("%d", &e.age);
    getchar();

    e.a_vote = 0;
    electeurs[nb_electeurs++] = e;

    sauvegarder_donnees();

    printf("\n✅ Electeur %s inscrit avec succes !\n", e.nom);
}

void inscrire_candidat(void) {
    if (nb_candidats >= MAX_CANDIDATS) {
        printf("\n❌ Nombre maximal de candidats atteint.\n");
        return;
    }

    Candidat c;
    printf("\n=== INSCRIPTION CANDIDAT ===\n");
    printf("Nom : ");
    fgets(c.nom, NOM_MAX, stdin); c.nom[strcspn(c.nom, "\n")] = 0;

    printf("Identifiant : ");
    fgets(c.id, ID_MAX, stdin); c.id[strcspn(c.id, "\n")] = 0;

    // printf("Date de naissance (jj/mm/aaaa) : ");
    // fgets(c.date_naissance, DATE_MAX, stdin); c.date_naissance[strcspn(c.date_naissance, "\n")] = 0;

    printf("Age : ");
    scanf("%d", &c.age);
    getchar();

    printf("Nom du parti politique : ");
    fgets(c.parti, PARTI_MAX, stdin); c.parti[strcspn(c.parti, "\n")] = 0;

    c.votes = 0;
    c.accepte = 1;
    strcpy(c.motif_rejet, "Aucun");

    // Vérification
    if (c.age < 35) {
        c.accepte = 0;
        strcpy(c.motif_rejet, "Age inferieur à 35 ans");
    } else if (strlen(c.parti) == 0) {
        c.accepte = 0;
        strcpy(c.motif_rejet, "Parti politique non renseigne");
    } else if (strstr(c.id, "paiement") == NULL && strstr(c.id, "PAIEMENT") == NULL) {
        c.accepte = 0;
        strcpy(c.motif_rejet, "Identifiant incorrect (doit contenir 'paiement')");
    }

    candidats[nb_candidats++] = c;
    sauvegarder_donnees();
    if (c.accepte)
        printf("\n✅ Candidature acceptee pour %s (%s)\n", c.nom, c.parti);
    else
        printf("\n❌ Candidature rejetee pour %s — Motif : %s\n", c.nom, c.motif_rejet);
}

void afficher_candidatures(void) {
    printf("\n======== LISTE DES CANDIDATURES ========\n");

    printf("\n--- Candidatures acceptees ---\n");
    printf("%-20s | %-15s | %-20s | %-5s | %-5s\n", "Nom", "ID", "Parti", "Âge", "Votes");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < nb_candidats; i++) {
        if (candidats[i].accepte) {
            printf("%-20s | %-15s | %-20s | %-5d | %-5d\n",
                   candidats[i].nom, candidats[i].id, candidats[i].parti,
                   candidats[i].age, candidats[i].votes);
        }
    }

    printf("\n------- Candidatures rejetees -------\n");
    printf("%-20s | %-15s | %-40s\n", "Nom", "ID", "Motif du rejet");
    printf("---------------------------------------------------------------------------------\n");
    for (int i = 0; i < nb_candidats; i++) {
        if (!candidats[i].accepte) {
            printf("%-20s | %-15s | %-40s\n",
                   candidats[i].nom, candidats[i].id, candidats[i].motif_rejet);
        }
    }
}

int a_deja_vote(char id[]) {
    for (int i = 0; i < nb_electeurs; i++) {
        if (strcmp(electeurs[i].id, id) == 0) {
            return electeurs[i].a_vote;
        }
    }
    return -1;
}

void voter(void) {
    char id_electeur[ID_MAX], id_candidat[ID_MAX];

    if (nb_candidats == 0) {
        printf("Aucun candidat enregistre.\n");
        return;
    }

    printf("\nEntrez votre identifiant d'electeur : ");
    scanf("%s", id_electeur);
    getchar();

    int deja = a_deja_vote(id_electeur);
    if (deja == -1) {
        printf("⚠️ Electeur non trouve.\n");
        return;
    } else if (deja == 1) {
        printf(" ⚠️ Vous avez deja vote.\n");
        return;
    }

    afficher_candidatures();
    printf("\nEntrez l'identifiant du candidat choisi : ");
    scanf("%s", id_candidat);
    getchar();

    int trouve = 0;
    for (int i = 0; i < nb_candidats; i++) {
        if (strcmp(candidats[i].id, id_candidat) == 0 && candidats[i].accepte) {
            candidats[i].votes++;
            trouve = 1;
            break;
        }
    }

    if (!trouve) {
        printf("Candidat invalide ou rejete.\n");
        return;
    }

    for (int i = 0; i < nb_electeurs; i++) {
        if (strcmp(electeurs[i].id, id_electeur) == 0) {
            electeurs[i].a_vote = 1;
            break;
        }
    }

    printf("Vote enregistre !\n");
}

void trier_candidats_par_votes(void) {
    for (int i = 0; i < nb_candidats - 1; i++) {
        for (int j = i + 1; j < nb_candidats; j++) {
            if (candidats[i].votes > candidats[j].votes) {
                Candidat temp = candidats[i];
                candidats[i] = candidats[j];
                candidats[j] = temp;
            }
        }
    }

    printf("\n=== Classement des candidats par votes ===\n");
    printf("%-20s | %-15s | %-20s | %-5s | %-5s\n", "Nom", "ID", "Parti", "Âge", "Votes");
    printf("--------------------------------------------------------------------------\n");
    
    for (int i = 0; i < nb_candidats; i++) {
        if (candidats[i].accepte)
            printf("%-20s | %-15s | %-20s | %-5d | %-5d\n",
                   candidats[i].nom, candidats[i].id, candidats[i].parti,
                   candidats[i].age, candidats[i].votes);
    }
}
