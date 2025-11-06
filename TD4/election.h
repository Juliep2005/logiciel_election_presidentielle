#ifndef ELECTION_H
#define ELECTION_H

#define MAX_CANDIDATS 20
#define MAX_ELECTEURS 100
#define NOM_MAX 50
#define PARTI_MAX 50
#define ID_MAX 30
#define DATE_MAX 15
#define QUARTIER_MAX 50

typedef struct {
    char nom[NOM_MAX];
    char id[ID_MAX];
    char quartier[QUARTIER_MAX];
    int age;
    int a_vote; // 0 = pas encore voté, 1 = a déjà voté
} Electeur;

typedef struct {
    char nom[NOM_MAX];
    char id[ID_MAX];
    char date_naissance[DATE_MAX];
    char parti[PARTI_MAX];
    int age;
    int accepte; // 1 = accepté, 0 = rejeté
    int votes;
    char motif_rejet[100]; // cause du rejet
} Candidat;

// Variables globales
extern Electeur electeurs[MAX_ELECTEURS];
extern Candidat candidats[MAX_CANDIDATS];
extern int nb_electeurs;
extern int nb_candidats;

// Prototypes
void charger_donnees();
void sauvegarder_donnees();
void inscrire_electeur(void);
void inscrire_candidat(void);
void afficher_candidatures(void);
int a_deja_vote(char id[]);
void voter(void);
void trier_candidats_par_votes(void);

#endif

