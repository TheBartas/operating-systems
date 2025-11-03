#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<errno.h>
#include<locale.h>
#include<string.h>
#include<process.h>
#include<windows.h>

int FirstWarning(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s", "Invalid number of arguments!\n");
        return 1;
    }
    if (strlen(argv[1]) < 1 || strlen(argv[1]) > 25) {
        if (strlen(argv[1]) < 1) {
            fprintf(stderr, "%s", "Too small (only the number 1) data format!\n");
            return 1;
        }
        if (strlen(argv[1]) > 25) {
            fprintf(stderr, "%s", "Too big data format!\n");
            return 1;
        }
    }
    return 0;
}
void SplitTheString(int counter, char* epicString, char** part1, char** part2) {
    //printf("Długość danych: %d\n", counter);
    if (counter == 1) {
        *part1 = (char*)malloc(counter + 1);
        strcpy(*part1, epicString);
        (*part1)[counter] = '\0';  // Dodaj null terminator
        *part2 = NULL;  // Nie ma drugiej części, więc ustaw na NULL
    }
    else if (counter % 2 == 0) {
        int halfLenght = counter / 2;
        *part1 = (char*)malloc(halfLenght + 1);
        *part2 = (char*)malloc(halfLenght + 1);
        // Kopiuje określoną liczbę znaków z łańcucha źródłowego (src) do łańcucha docelowego (dest).
        strncpy(*part1, epicString, halfLenght);
        (*part1)[halfLenght] = '\0';
        strcpy(*part2, epicString + halfLenght); // zaczynam od środka + idę do końca
    }
    else {
        int halfLenghtOfPart1 = (counter - 1) / 2;
        int halfLenghtOfPart2 = counter - halfLenghtOfPart1;
        *part1 = (char*)malloc(halfLenghtOfPart1 + 1);
        *part2 = (char*)malloc(halfLenghtOfPart2 + 1);
        // Kopiuje określoną liczbę znaków z łańcucha źródłowego (src) do łańcucha docelowego (dest).
        strncpy(*part1, epicString, halfLenghtOfPart1);
        (*part1)[halfLenghtOfPart1] = '\0';
        strcpy(*part2, epicString + halfLenghtOfPart1); // zaczynam od środka, ...Part1 bo do tego momentu skopiowałem dane
    }
}

int main(int argc, char* argv[]) {
    //setlocale(LC_ALL, "Polish"); z jakiegoś powodu nie działa w terminalu Windowsa
    if (FirstWarning(argc, argv) == 1) {
        return 1;
    }

    if (strlen(argv[1]) == 1) {
        return atoi(argv[1]);
    }

    char* part1;
    char* part2;
    SplitTheString(strlen(argv[1]), argv[1], &part1, &part2); // przekazanie wskaźników do funkcji, aby były widoczne poza main
    char* endarg;
    errno = 0;
    long long int arg1 = strtoll(part1, &endarg, 10); // konwertuje do long long int, 10 to zakres liczb dziesiętnych, &endarg, patrzy czy są liczby
    long long int arg2 = strtoll(part2, &endarg, 10); // konwertuje do long long int, 10 to zakres liczb dziesiętnych, &endarg, patrzy czy są liczby

    if (argc == 2) {
        if (*endarg != '\0' || errno != 0) {
            fprintf(stderr, "%s", "Invalid data format!\n");
            return 1;
        }
    }
    else {
        fprintf(stderr, "%s", "\n");
        return 1;
    }

    // Procesy
    STARTUPINFOA si1;
    PROCESS_INFORMATION pi1;
    DWORD exit1, exit2;
    memset(&si1,0, sizeof(si1));
    si1.cb = sizeof(si1);
    memset(&pi1,0, sizeof(pi1));
    char argline1[50];
    sprintf(argline1, "a.exe %s", part1);
    if (CreateProcessA(NULL, argline1, NULL, NULL, 0, 0, NULL , NULL, &si1, &pi1) == 0){
        printf( "CreateProcess \"1\" failed (%d).\n", GetLastError());
        return 2;
    }

    STARTUPINFOA si2;
    PROCESS_INFORMATION pi2;
    memset(&si2,0, sizeof(si2));
    si1.cb = sizeof(si2);
    memset(&pi2,0, sizeof(pi2));
    char argline2[50];
    sprintf(argline2, "a.exe %s", part2);
        if (CreateProcessA(NULL, argline2, NULL, NULL, 0, 0, NULL , NULL, &si2, &pi2) == 0){
        printf( "CreateProcess \"2\" failed (%d).\n", GetLastError());
        return 2;
    }

    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);
    GetExitCodeProcess(pi1.hProcess, &exit1);
    GetExitCodeProcess(pi2.hProcess, &exit2);
    printf("%ld  %ld     %s  %d\n", GetCurrentProcessId(), pi1.dwProcessId, part1, exit1);
    printf("%ld  %ld     %s  %d\n", GetCurrentProcessId(), pi2.dwProcessId, part2, exit2);
    CloseHandle( pi1.hProcess ); // zwalnia uchwyt do procesu
    CloseHandle( pi1.hThread ); // zwalnia uchwyt do wątku
    CloseHandle( pi2.hProcess ); // zwalnia uchwyt do procesu
    CloseHandle( pi2.hThread ); // zwalnia uchwyt do wątku
    free(part1);
    free(part2);
    return exit1 + exit2;
}
