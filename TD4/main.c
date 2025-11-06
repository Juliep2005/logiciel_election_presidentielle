#include <stdio.h>
#include <stdlib.h>
#include "election.h"

#pragma execution_character_set("utf-8")
#ifdef _WIN32
#include <windows.h>

void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004;
    SetConsoleMode(hOut, dwMode);
}
#endif

void afficher_menu() {
    system("cls || clear");
    printf("\033[1;36m");
    printf("╔═════════════════════════════════════════════════════════════════════╗\n");
    printf("║        🗳️  SYSTÈME D'ÉLECTION PRÉSIDENTIELLE                       ║\n");
    printf("╠═════════════════════════════════════════════════════════════════════╣\n");
    printf("║  \033[1;33m1.\033[0;36m Enregistrer un électeur                     ║\n");
    printf("║  \033[1;33m2.\033[0;36m Enregistrer un candidat                     ║\n");
    printf("║  \033[1;33m3.\033[0;36m Afficher les candidatures                   ║\n");
    printf("║  \033[1;33m4.\033[0;36m Voter                                       ║\n");
    printf("║  \033[1;33m5.\033[0;36m Afficher classement par votes               ║\n");
    printf("║  \033[1;33m0.\033[0;36m Quitter                                     ║\n");
    printf("╚═════════════════════════════════════════════════════════════════════╝\n");
    printf("\033[0m");
    printf("\n👉 Faites votre choix : ");
}

int main() {
    system("chcp 65001 > nul");

    charger_donnees();

    #ifdef _WIN32
        enableANSI();
    #endif

    int choix;
    do {
        afficher_menu();
        scanf("%d", &choix);
        getchar();

        switch (choix) {
            case 1: inscrire_electeur(); break;
            case 2: inscrire_candidat(); break;
            case 3: afficher_candidatures(); break;
            case 4: voter(); break;
            case 5: trier_candidats_par_votes(); break;
            case 0: printf("\n👋 Merci d’avoir utilisé le programme.\n"); break;
            default: printf("\n❌ Choix invalide.\n");
        }

        if (choix != 0) {
            printf("\nAppuyez sur Entrée pour continuer...");
            getchar();
        }
    } while (choix != 0);

    return 0;
}
