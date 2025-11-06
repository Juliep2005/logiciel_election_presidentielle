#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK CandidatDlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrev, PWSTR args, int nCmdShow) {
    WNDCLASSW wc = {0};
    wc.hInstance = hInst;
    wc.lpszClassName = L"ElectionApp";
    wc.lpfnWndProc = WndProc;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(
        L"ElectionApp",
        L"Gestion des Élections Présidentielles",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInst, NULL
    );

    MSG msg = {0};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

// --- Menu ---
HMENU CreateAppMenu(HWND hwnd) {
    HMENU hMenubar = CreateMenu();
    HMENU hMenuCandidats = CreateMenu();
    AppendMenuW(hMenuCandidats, MF_STRING, 101, L"Enregistrer un candidat");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuCandidats, L"Candidats");
    AppendMenuW(hMenubar, MF_STRING, 999, L"Quitter");
    SetMenu(hwnd, hMenubar);
    return hMenubar;
}

// --- Formulaire enregistrement candidat ---
void OpenCandidatForm(HWND hwndParent) {
    HWND hDlg = CreateWindowW(
        L"STATIC", L"Nom du candidat:", 
        WS_VISIBLE | WS_CHILD,
        50, 50, 120, 20,
        hwndParent, NULL, NULL, NULL
    );

    HWND hEdit = CreateWindowW(
        L"EDIT", L"", 
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        180, 50, 200, 20,
        hwndParent, (HMENU)2001, NULL, NULL
    );

    HWND hButton = CreateWindowW(
        L"BUTTON", L"Enregistrer",
        WS_VISIBLE | WS_CHILD,
        180, 80, 100, 25,
        hwndParent, (HMENU)3001, NULL, NULL
    );
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE:
        CreateAppMenu(hwnd);
        break;

    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case 101: OpenCandidatForm(hwnd); break;
        case 999: PostQuitMessage(0); break;

        case 3001: { // bouton enregistrer
            wchar_t nom[100];
            HWND hEdit = GetDlgItem(hwnd, 2001);
            GetWindowTextW(hEdit, nom, 100);
            wchar_t msg[150];
            wsprintfW(msg, L"Candidat enregistré : %s", nom);
            MessageBoxW(hwnd, msg, L"Info", MB_OK);
        } break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hwnd, msg, wp, lp);
    }
    return 0;
}
