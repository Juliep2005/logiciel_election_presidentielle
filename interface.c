#include <windows.h>
#include "interface.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR args, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.hInstance = hInst;
    wc.lpszClassName = "ElectionApp";
    wc.lpfnWndProc = WndProc;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("ElectionApp",
                             "Gestion des Élections Présidentielles",
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                             NULL, NULL, hInst, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE:
        CreateWindow("BUTTON", "Gérer les candidats", WS_VISIBLE | WS_CHILD,
                     50, 50, 200, 40, hwnd, (HMENU)1, NULL, NULL);

        CreateWindow("BUTTON", "Gérer les électeurs", WS_VISIBLE | WS_CHILD,
                     50, 110, 200, 40, hwnd, (HMENU)2, NULL, NULL);

        CreateWindow("BUTTON", "Lancer le vote", WS_VISIBLE | WS_CHILD,
                     50, 170, 200, 40, hwnd, (HMENU)3, NULL, NULL);

        CreateWindow("BUTTON", "Afficher les résultats", WS_VISIBLE | WS_CHILD,
                     50, 230, 200, 40, hwnd, (HMENU)4, NULL, NULL);

        CreateWindow("BUTTON", "Quitter", WS_VISIBLE | WS_CHILD,
                     50, 290, 200, 40, hwnd, (HMENU)5, NULL, NULL);
        break;

        AjouterMenus(hwnd);

    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case 1: MessageBox(hwnd, "Gestion des candidats", "Info", MB_OK); break;
        case 2: MessageBox(hwnd, "Gestion des électeurs", "Info", MB_OK); break;
        case 3: MessageBox(hwnd, "Vote lancé", "Info", MB_OK); break;
        case 4: MessageBox(hwnd, "Affichage des résultats", "Info", MB_OK); break;
        case 5: PostQuitMessage(0); break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}
