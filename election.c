/*
Election management - Windows GUI in C
Single-file example implementing:
- Structures for Candidat and Electeur
- Load candidates and electors from CSV
- Register single elector, check if voted
- Display accepted/rejected candidates
- Voting (elector logs in with ID + password), one vote per elector
- Count votes, sort candidates by votes (ascending)
- Save/load state to binary file

Compile (MinGW):
  gcc -std=c11 -municode -o election.exe election_win32.c -lcomdlg32 -lgdi32

Notes:
- This is a minimal educational example. For production use, add robust error handling, cryptography, secure password storage, and full audit logs.
- CSV expected fields (semicolon-separated) for electeurs: id;nom;quartier;age;motdepasse
- CSV expected fields for candidats: id;nom;date_naissance;parti;accepte
*/

// #define UNICODE
#define _UNICODE

#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "interface.h"


#define MAX_CANDIDATS 20
#define MAX_ELECTEURS 10000
#define MAX_STR 256

typedef struct {
    char id[32];
    char nom[100];
    char date_naissance[32];
    char parti[64];
    int accepte; // 1 accepted, 0 rejected
    int nb_voix;
} Candidat;

typedef struct {
    char id[32];
    char nom[100];
    char quartier[100];
    int age;
    char motdepasse[64];
    int a_vote; // 0/1
} Electeur;

typedef struct {
    Candidat candidats[MAX_CANDIDATS];
    int nb_candidats;
    Electeur electeurs[MAX_ELECTEURS];
    int nb_electeurs;
} Election;

static Election g_election;
static HWND g_hListCandidates = NULL;
static HWND g_hMain = NULL;

// Utility: trim newline and spaces
static void trim_newline(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n>0 && (s[n-1]=='\n' || s[n-1]=='\r' || s[n-1]==' ' || s[n-1]=='\t')) { s[n-1]='\0'; n--; }
}

// Register a candidate
void enregistrerCandidat(Election *e, const char *id, const char *nom, const char *date, const char *parti, int accepte) {
    if (e->nb_candidats >= MAX_CANDIDATS) return;
    Candidat *c = &e->candidats[e->nb_candidats++];
    strncpy(c->id, id, sizeof(c->id)-1); c->id[sizeof(c->id)-1]='\0';
    strncpy(c->nom, nom, sizeof(c->nom)-1); c->nom[sizeof(c->nom)-1]='\0';
    strncpy(c->date_naissance, date, sizeof(c->date_naissance)-1); c->date_naissance[sizeof(c->date_naissance)-1]='\0';
    strncpy(c->parti, parti, sizeof(c->parti)-1); c->parti[sizeof(c->parti)-1]='\0';
    c->accepte = accepte;
    c->nb_voix = 0;
}

// Register one elector
void enregistrerElecteur(Election *e, const char *id, const char *nom, const char *quartier, int age, const char *motdepasse) {
    if (e->nb_electeurs >= MAX_ELECTEURS) return;
    Electeur *el = &e->electeurs[e->nb_electeurs++];
    strncpy(el->id, id, sizeof(el->id)-1); el->id[sizeof(el->id)-1]='\0';
    strncpy(el->nom, nom, sizeof(el->nom)-1); el->nom[sizeof(el->nom)-1]='\0';
    strncpy(el->quartier, quartier, sizeof(el->quartier)-1); el->quartier[sizeof(el->quartier)-1]='\0';
    el->age = age;
    strncpy(el->motdepasse, motdepasse, sizeof(el->motdepasse)-1); el->motdepasse[sizeof(el->motdepasse)-1]='\0';
    el->a_vote = 0;
}

// Check if elector voted: returns 1 yes, 0 no, -1 not found
int aVote(Election *e, const char *id) {
    for (int i=0;i<e->nb_electeurs;i++) if (strcmp(e->electeurs[i].id, id)==0) return e->electeurs[i].a_vote;
    return -1;
}

// Display accepted and rejected candidates in MessageBox
void afficherCandidatures(Election *e, HWND hwnd) {
    char buf[16384]; buf[0]='\0';
    strcat(buf, "Candidatures acceptes:\n");
    for (int i=0;i<e->nb_candidats;i++) if (e->candidats[i].accepte) {
        strcat(buf, e->candidats[i].id); strcat(buf, " - "); strcat(buf, e->candidats[i].nom); strcat(buf, " (" ); strcat(buf, e->candidats[i].parti); strcat(buf, ")\n");
    }
    strcat(buf, "\nCandidatures rejetes:\n");
    for (int i=0;i<e->nb_candidats;i++) if (!e->candidats[i].accepte) {
        strcat(buf, e->candidats[i].id); strcat(buf, " - "); strcat(buf, e->candidats[i].nom); strcat(buf, " (" ); strcat(buf, e->candidats[i].parti); strcat(buf, ")\n");
    }
    MessageBoxA(hwnd, buf, "Candidatures", MB_OK);
}

// Count vote for candidate id by elector id; returns 1 success, 0 failure
int voter(Election *e, const char *id_electeur, const char *id_candidat) {
    int idxEl = -1;
    for (int i=0;i<e->nb_electeurs;i++) if (strcmp(e->electeurs[i].id, id_electeur)==0) { idxEl=i; break; }
    if (idxEl==-1) return 0;
    if (e->electeurs[idxEl].a_vote) return 0;
    for (int j=0;j<e->nb_candidats;j++) {
        if (strcmp(e->candidats[j].id, id_candidat)==0 && e->candidats[j].accepte) {
            e->candidats[j].nb_voix++;
            e->electeurs[idxEl].a_vote = 1;
            return 1;
        }
    }
    return 0;
}

// Sort candidates by ascending votes (simple bubble sort)
void trierCandidatsParVote(Election *e) {
    for (int i=0;i<e->nb_candidats-1;i++) {
        for (int j=0;j<e->nb_candidats-i-1;j++) {
            if (e->candidats[j].nb_voix > e->candidats[j+1].nb_voix) {
                Candidat tmp = e->candidats[j]; e->candidats[j]=e->candidats[j+1]; e->candidats[j+1]=tmp;
            }
        }
    }
}

// Load electeurs CSV: id;nom;quartier;age;motdepasse
int loadElecteursCSV(Election *e, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (strlen(line)==0) continue;
        char id[128], nom[256], quartier[256], pass[128]; int age=0;
        // use semicolon separator
        char *p = line; char *tok;
        tok = strtok(p, ";"); if (!tok) continue; strncpy(id, tok, sizeof(id)-1);
        tok = strtok(NULL, ";"); if (!tok) continue; strncpy(nom, tok, sizeof(nom)-1);
        tok = strtok(NULL, ";"); if (!tok) continue; strncpy(quartier, tok, sizeof(quartier)-1);
        tok = strtok(NULL, ";"); if (!tok) continue; age = atoi(tok);
        tok = strtok(NULL, ";\n\r"); if (!tok) strcpy(pass, ""); else strncpy(pass, tok, sizeof(pass)-1);
        enregistrerElecteur(e, id, nom, quartier, age, pass);
    }
    fclose(f);
    return 1;
}

// Load candidats CSV: id;nom;date_naissance;parti;accepte
int loadCandidatsCSV(Election *e, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (strlen(line)==0) continue;
        char id[128], nom[256], date[64], parti[128]; int accepte=0;
        char *p = line; char *tok;
        tok = strtok(p, ";"); if (!tok) continue; strncpy(id, tok, sizeof(id)-1);
        tok = strtok(NULL, ";"); if (!tok) continue; strncpy(nom, tok, sizeof(nom)-1);
        tok = strtok(NULL, ";"); if (!tok) continue; strncpy(date, tok, sizeof(date)-1);
        tok = strtok(NULL, ";"); if (!tok) continue; strncpy(parti, tok, sizeof(parti)-1);
        tok = strtok(NULL, ";\n\r"); if (!tok) accepte=0; else accepte = atoi(tok);
        enregistrerCandidat(e, id, nom, date, parti, accepte);
    }
    fclose(f);
    return 1;
}

// Simple save/load binary state
int saveState(const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f) return 0;
    fwrite(&g_election.nb_candidats, sizeof(int), 1, f);
    fwrite(g_election.candidats, sizeof(Candidat), g_election.nb_candidats, f);
    fwrite(&g_election.nb_electeurs, sizeof(int), 1, f);
    fwrite(g_election.electeurs, sizeof(Electeur), g_election.nb_electeurs, f);
    fclose(f);
    return 1;
}

int loadState(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    fread(&g_election.nb_candidats, sizeof(int), 1, f);
    fread(g_election.candidats, sizeof(Candidat), g_election.nb_candidats, f);
    fread(&g_election.nb_electeurs, sizeof(int), 1, f);
    fread(g_election.electeurs, sizeof(Electeur), g_election.nb_electeurs, f);
    fclose(f);
    return 1;
}

// Helper to open file dialog and return chosen path (UTF-8 to ANSI simple conversion)
int pickFilePath(HWND hwnd, char *outPath, int outMax) {
    OPENFILENAMEA ofn;
    char szFile[260] = {0};
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrFilter = "CSV Files\0*.csv\0All Files\0*.*\0";
    if (GetOpenFileNameA(&ofn)==TRUE) {
        strncpy(outPath, szFile, outMax-1); outPath[outMax-1]='\0';
        return 1;
    }
    return 0;
}

// Create/update candidate listbox
void refreshCandidateList(HWND hwnd) {
    if (!g_hListCandidates) return;
    SendMessageW(g_hListCandidates, LB_RESETCONTENT, 0, 0);
    for (int i=0;i<g_election.nb_candidats;i++) {
        char buf[256]; snprintf(buf, sizeof(buf), "%s - %s (%s) [%d]", g_election.candidats[i].id, g_election.candidats[i].nom, g_election.candidats[i].parti, g_election.candidats[i].nb_voix);
        // convert to wide
        wchar_t wbuf[256]; MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, 256);
        SendMessageW(g_hListCandidates, LB_ADDSTRING, 0, (LPARAM)wbuf);
    }
}

// Create a simple modal input dialog (returns 1 on OK, 0 cancel). Caller must free allocated strings.
int simpleInputDialog(HWND parent, const wchar_t *title, const wchar_t *label1, wchar_t **out1, const wchar_t *label2, wchar_t **out2) {
    // Create a small dialog window with two edit controls
    HWND dlg = CreateWindowW(L"STATIC", title, WS_OVERLAPPED | WS_BORDER | WS_CAPTION, CW_USEDEFAULT, CW_USEDEFAULT, 420, 220, parent, NULL, NULL, NULL);
    if (!dlg) return 0;
    HWND hLab1 = CreateWindowW(L"STATIC", label1, WS_CHILD | WS_VISIBLE, 10, 10, 380, 20, dlg, NULL, NULL, NULL);
    HWND hEdit1 = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 10, 35, 380, 24, dlg, NULL, NULL, NULL);
    HWND hLab2 = CreateWindowW(L"STATIC", label2, WS_CHILD | WS_VISIBLE, 10, 70, 380, 20, dlg, NULL, NULL, NULL);
    HWND hEdit2 = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_PASSWORD, 10, 95, 380, 24, dlg, NULL, NULL, NULL);
    HWND hOk = CreateWindowW(L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 140, 80, 30, dlg, NULL, NULL, NULL);
    HWND hCancel = CreateWindowW(L"BUTTON", L"Annuler", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 220, 140, 80, 30, dlg, NULL, NULL, NULL);

    ShowWindow(dlg, SW_SHOW);
    UpdateWindow(dlg);

    MSG msg;
    int result = 0;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        if (msg.message == WM_COMMAND) {
            if ((HWND)msg.hwnd == hOk) {
                int len1 = GetWindowTextLengthW(hEdit1);
                int len2 = GetWindowTextLengthW(hEdit2);
                *out1 = (wchar_t*)malloc((len1+1)*sizeof(wchar_t));
                *out2 = (wchar_t*)malloc((len2+1)*sizeof(wchar_t));
                GetWindowTextW(hEdit1, *out1, len1+1);
                GetWindowTextW(hEdit2, *out2, len2+1);
                result = 1; break;
            } else if ((HWND)msg.hwnd == hCancel) {
                result = 0; break;
            }
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    DestroyWindow(dlg);
    return result;
}

// Show results and allow export to PDF (we will export a simple text-based .txt renamed to .pdf placeholder)
void showResults(HWND hwnd) {
    trierCandidatsParVote(&g_election);
    char buf[16384]; buf[0]='\0';
    int total = 0;
    for (int i=0;i<g_election.nb_candidats;i++) total += g_election.candidats[i].nb_voix;
    snprintf(buf, sizeof(buf), "Résultats officiels:\nTotal votes: %d\n\n", total);
    for (int i=0;i<g_election.nb_candidats;i++) {
        char line[256];
        double pct = (total>0) ? (100.0 * g_election.candidats[i].nb_voix / total) : 0.0;
        snprintf(line, sizeof(line), "%s - %s (%s) : %d voix (%.2f%%)\n", g_election.candidats[i].id, g_election.candidats[i].nom, g_election.candidats[i].parti, g_election.candidats[i].nb_voix, pct);
        strcat(buf, line);
    }
    // Show in messagebox (may be long)
    MessageBoxA(hwnd, buf, "Résultats", MB_OK);

    // Save to text file and rename .pdf (placeholder - real PDF generation not implemented here)
    char savePath[MAX_PATH];
    if (GetSaveFileNameA(&(OPENFILENAMEA){ .lStructSize=sizeof(OPENFILENAMEA), .hwndOwner=hwnd, .lpstrFilter="PDF Files\0*.pdf\0Text Files\0*.txt\0", .lpstrFile=savePath, .nMaxFile=MAX_PATH, .Flags=OFN_OVERWRITEPROMPT })) {
        // ensure extension .pdf
        FILE *f = fopen(savePath, "w");
        if (f) {
            fprintf(f, "%s\n", buf);
            fclose(f);
            MessageBoxA(hwnd, "Rapport exporté (texte) — pour production, intégrer une bibliothèque PDF.", "Export", MB_OK);
        }
    }
}

// Handler for the Vote flow: ask for elector id & password; if valid, show list of accepted candidates and let choose
void handleVote(HWND hwnd) {
    // input dialog for ID and password
    wchar_t *wid = NULL, *wpass = NULL;
    if (!simpleInputDialog(hwnd, L"Connexion électeur", L"Identifiant électeur:", &wid, L"Mot de passe:", &wpass)) {
        if (wid) free(wid); if (wpass) free(wpass); return;
    }
    // convert to utf8
    char id[128]; char pass[128];
    WideCharToMultiByte(CP_UTF8, 0, wid, -1, id, sizeof(id), NULL, NULL);
    WideCharToMultiByte(CP_UTF8, 0, wpass, -1, pass, sizeof(pass), NULL, NULL);
    free(wid); free(wpass);
    int idxEl = -1;
    for (int i=0;i<g_election.nb_electeurs;i++) if (strcmp(g_election.electeurs[i].id, id)==0) { idxEl=i; break; }
    if (idxEl==-1) { MessageBoxA(hwnd, "Électeur non trouvé.", "Erreur", MB_OK | MB_ICONERROR); return; }
    if (strcmp(g_election.electeurs[idxEl].motdepasse, pass)!=0) { MessageBoxA(hwnd, "Mot de passe incorrect.", "Erreur", MB_OK | MB_ICONERROR); return; }
    if (g_election.electeurs[idxEl].a_vote) { MessageBoxA(hwnd, "Vous avez déjà voté.", "Info", MB_OK | MB_ICONINFORMATION); return; }

    // Build a temporary dialog with listbox of accepted candidates
    HWND dlg = CreateWindowW(L"STATIC", L"Choix du candidat", WS_OVERLAPPED | WS_BORDER | WS_CAPTION, CW_USEDEFAULT, CW_USEDEFAULT, 420, 420, hwnd, NULL, NULL, NULL);
    HWND hLb = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_VSCROLL, 10, 10, 380, 300, dlg, NULL, NULL, NULL);
    HWND hOk = CreateWindowW(L"BUTTON", L"Voter", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 330, 80, 30, dlg, NULL, NULL, NULL);
    HWND hCancel = CreateWindowW(L"BUTTON", L"Annuler", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 220, 330, 80, 30, dlg, NULL, NULL, NULL);
    // populate
    for (int i=0;i<g_election.nb_candidats;i++) if (g_election.candidats[i].accepte) {
        char buf[256]; snprintf(buf, sizeof(buf), "%s - %s (%s)", g_election.candidats[i].id, g_election.candidats[i].nom, g_election.candidats[i].parti);
        wchar_t wbuf[256]; MultiByteToWideChar(CP_UTF8,0,buf,-1,wbuf,256);
        SendMessageW(hLb, LB_ADDSTRING, 0, (LPARAM)wbuf);
    }
    ShowWindow(dlg, SW_SHOW); UpdateWindow(dlg);
    MSG msg; int chose = -1; int done=0;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        if (msg.message == WM_COMMAND) {
            if ((HWND)msg.hwnd == hOk) {
                int sel = (int)SendMessageW(hLb, LB_GETCURSEL, 0, 0);
                if (sel==LB_ERR) { MessageBoxA(dlg, "Veuillez sélectionner un candidat.", "Erreur", MB_OK); }
                else { chose = sel; done=1; break; }
            } else if ((HWND)msg.hwnd == hCancel) { done=0; break; }
        }
        TranslateMessage(&msg); DispatchMessageW(&msg);
    }
    if (chose>=0) {
        // Map selection to candidate index (accepted ones)
        int count=0; int idxCand=-1;
        for (int i=0;i<g_election.nb_candidats;i++) if (g_election.candidats[i].accepte) {
            if (count==chose) { idxCand=i; break; }
            count++; }
        if (idxCand>=0) {
            g_election.candidats[idxCand].nb_voix++;
            g_election.electeurs[idxEl].a_vote = 1;
            MessageBoxA(hwnd, "Vote enregistré. Merci.", "Succès", MB_OK);
            refreshCandidateList(hwnd);
        }
    }
    DestroyWindow(dlg);
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindowW(L"BUTTON", L"Charger electeurs (CSV)", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 220, 30, hwnd, (HMENU)1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"Charger candidats (CSV)", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 250, 10, 220, 30, hwnd, (HMENU)2, NULL, NULL);
        CreateWindowW(L"BUTTON", L"Afficher candidatures", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 50, 220, 30, hwnd, (HMENU)3, NULL, NULL);
        CreateWindowW(L"BUTTON", L"Vote (connexion)", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 250, 50, 220, 30, hwnd, (HMENU)4, NULL, NULL);
        CreateWindowW(L"BUTTON", L"Résultats & Export PDF", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 90, 220, 30, hwnd, (HMENU)5, NULL, NULL);
        CreateWindowW(L"BUTTON", L"Sauvegarder état", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 250, 90, 220, 30, hwnd, (HMENU)6, NULL, NULL);
        CreateWindowW(L"BUTTON", L"Charger état", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 130, 220, 30, hwnd, (HMENU)7, NULL, NULL);
        g_hListCandidates = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_VSCROLL, 10, 170, 460, 260, hwnd, NULL, NULL, NULL);
        break; }
    case WM_COMMAND: {
        int id = (int)wParam;
        if (id==1) {
            char path[MAX_PATH]; if (pickFilePath(hwnd, path, MAX_PATH)) {
                if (loadElecteursCSV(&g_election, path)) MessageBoxA(hwnd, "Électeurs importés.", "OK", MB_OK);
                else MessageBoxA(hwnd, "Erreur import électeurs.", "Erreur", MB_OK | MB_ICONERROR);
            }
        } else if (id==2) {
            char path[MAX_PATH]; if (pickFilePath(hwnd, path, MAX_PATH)) {
                if (loadCandidatsCSV(&g_election, path)) { MessageBoxA(hwnd, "Candidats importés.", "OK", MB_OK); refreshCandidateList(hwnd); }
                else MessageBoxA(hwnd, "Erreur import candidats.", "Erreur", MB_OK | MB_ICONERROR);
            }
        } else if (id==3) {
            afficherCandidatures(&g_election, hwnd);
        } else if (id==4) {
            handleVote(hwnd);
        } else if (id==5) {
            showResults(hwnd);
        } else if (id==6) {
            char savePath[MAX_PATH] = "election_state.dat"; if (saveState(savePath)) MessageBoxA(hwnd, "État sauvegardé.", "OK", MB_OK); else MessageBoxA(hwnd, "Erreur sauvegarde.", "Erreur", MB_OK);
        } else if (id==7) {
            char loadPath[MAX_PATH] = "election_state.dat"; if (loadState(loadPath)) { MessageBoxA(hwnd, "État chargé.", "OK", MB_OK); refreshCandidateList(hwnd);} else MessageBoxA(hwnd, "Erreur chargement.", "Erreur", MB_OK);
        }
        break; }
    case WM_DESTROY: PostQuitMessage(0); break;
    default: return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    ZeroMemory(&g_election, sizeof(g_election));
    // Register class
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ElectionAppClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);

    g_hMain = CreateWindowW(L"ElectionAppClass", L"Gestion Elections (Cameroon) - Prototype", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 500, 480, NULL, NULL, hInstance, NULL);
    if (!g_hMain) return 0;
    ShowWindow(g_hMain, nCmdShow);
    UpdateWindow(g_hMain);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}

int main() {
    // Lancer l’interface graphique
    WinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOW);
    return 0;
}
