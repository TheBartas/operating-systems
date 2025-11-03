#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <string.h>
#include <process.h>
#include <windows.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Podaj argument!\n");
        return 1;
    }

    int var = atoi(argv[1]);
    var--;

    if (var == 0) {
        return 1;
    }

    // Procesy
    STARTUPINFOA si1;
    PROCESS_INFORMATION pi1;
    DWORD exit1;

    memset(&si1, 0, sizeof(si1));
    si1.cb = sizeof(si1);
    memset(&pi1, 0, sizeof(pi1));
    char argline1[50];
    printf("%d", var);
    sprintf(argline1, "Kuba.exe %d", var);
    if (CreateProcessA(NULL, argline1, NULL, NULL, 0, 0, NULL, NULL, &si1, &pi1) == 0) {
        printf("CreateProcess \"1\" failed (%d).\n", GetLastError());
        return 2;
    }

    WaitForSingleObject(pi1.hProcess, INFINITE);
    GetExitCodeProcess(pi1.hProcess, &exit1);

    printf("%ld  %ld     %d  %d\n", GetCurrentProcessId(), pi1.dwProcessId, var, exit1);

    CloseHandle(pi1.hProcess); // zwalnia uchwyt do procesu
    CloseHandle(pi1.hThread);  // zwalnia uchwyt do wątku

    return exit1;
}

