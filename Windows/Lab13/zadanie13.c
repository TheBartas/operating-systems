#include<windows.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define MEM_SIZE 1024

typedef struct {
    char* Board[3][3];
    char* Message[100];
    char* currentPlayer[10];
    char* ifEnd[10];
    char* specialEnd[10];
}GameBoard;

typedef struct {
    int x;
    int y;
}Point;

void DisplayBoard(GameBoard* gameBoard){
    system("cls");
    for (int i = 0; i < 5;i++){
        if (i == 1 || i == 3){
            printf("\t\t\t\t\t\t--+---+--\n");
        }
        else{
            for (int j = 0;j < 3;j++){
                if (i == 0){                
                    if (j % 2){
                        printf("| %s | ", gameBoard->Board[0][j]);
                    }
                    else {
                        if(j == 0){
                            printf("\t\t\t\t\t\t%s ", gameBoard->Board[0][j]);
                        }
                        else{
                            printf("%s ", gameBoard->Board[0][j]);
                        }
                    }
                }
                if (i == 2){                
                    if (j % 2){
                        printf("| %s | ", gameBoard->Board[1][j]);
                    }
                    else {
                        if(j == 0){
                            printf("\t\t\t\t\t\t%s ", gameBoard->Board[1][j]);
                        }
                        else{
                            printf("%s ", gameBoard->Board[1][j]);
                        }
                    }
                }
                if (i == 4){
                    if (j % 2){
                        printf("| %s | ", gameBoard->Board[2][j]);
                    }
                    else {
                        if(j == 0){
                            printf("\t\t\t\t\t\t%s ", gameBoard->Board[2][j]);
                        }
                        else{
                            printf("%s ", gameBoard->Board[2][j]);
                        }
                    }
                }
            }
        printf("\n");
        }
    }
}

void SupportDisplayBoard(){
    printf("\n\n");
    for (int i = 0; i < 5;i++){
        if (i == 1 || i == 3) {
            printf("\t\t\t\t\t\t--+---+--\n");
        }
        else{
            for (int j = 0;j < 3;j++){
                if (i == 0){                
                    if (j % 2){
                        printf("| 2 | ");
                    }
                    else {
                        if(j == 0){
                            printf("\t\t\t\t\t\t1 ");
                        }
                        else{
                            printf("3 ");
                        }
                    }
                }
                if (i == 2){                
                    if (j % 2){
                        printf("| 5 | ");
                    }
                    else {
                        if(j == 0){
                            printf("\t\t\t\t\t\t4 ");
                        }
                        else{
                            printf("6 ");
                        }
                    }
                }
                if (i == 4){
                    if (j % 2){
                        printf("| 8 | ");
                    }
                    else {
                        if(j == 0){
                            printf("\t\t\t\t\t\t7 ");
                        }
                        else{
                            printf("9 ");
                        }
                    }
                }
            }
        printf("\n");
        }
    }
    printf("\n\n");
}

void LetGetPoint(Point* point, int index, int x, int y){
    switch(index){
        case 1:
            point->x = 0; 
            point->y = 0;
            break;
        case 2:
            point->x = 0; 
            point->y = 1;
            break;
        case 3:
            point->x = 0; 
            point->y = 2;
            break;
        case 4:
            point->x = 1; 
            point->y = 0;
            break;
        case 5:
            point->x = 1; 
            point->y = 1;
            break;
        case 6:
            point->x = 1; 
            point->y = 2;
            break;
        case 7:
            point->x = 2; 
            point->y = 0;
            break;
        case 8:
            point->x = 2; 
            point->y = 1;
            break;  
        case 9:
            point->x = 2; 
            point->y = 2;
            break;
        default:
            return;
    }
}

bool WhoWin(GameBoard* gameBoard) {
    if ((gameBoard->Board[0][0]== "X" && gameBoard->Board[0][1]== "X" && gameBoard->Board[0][2]== "X") | 
        (gameBoard->Board[1][0]== "X" && gameBoard->Board[1][1]== "X" && gameBoard->Board[1][2]== "X") |
        (gameBoard->Board[2][0]== "X" && gameBoard->Board[2][1]== "X" && gameBoard->Board[2][2]== "X") |
        (gameBoard->Board[0][0]== "X" && gameBoard->Board[1][0]== "X" && gameBoard->Board[2][0]== "X") |
        (gameBoard->Board[0][1]== "X" && gameBoard->Board[1][1]== "X" && gameBoard->Board[2][1]== "X") |
        (gameBoard->Board[0][2]== "X" && gameBoard->Board[1][2]== "X" && gameBoard->Board[2][2]== "X") |
        (gameBoard->Board[0][0]== "X" && gameBoard->Board[1][1]== "X" && gameBoard->Board[2][2]== "X") |
        (gameBoard->Board[2][0]== "X" && gameBoard->Board[1][1]== "X" && gameBoard->Board[0][2]== "X")
        ) {
            gameBoard->ifEnd[10] = "Player X";
            return true;
    }
    if ((gameBoard->Board[0][0]== "O" && gameBoard->Board[0][1]== "O" && gameBoard->Board[0][2]== "O") | 
        (gameBoard->Board[1][0]== "O" && gameBoard->Board[1][1]== "O" && gameBoard->Board[1][2]== "O") |
        (gameBoard->Board[2][0]== "O" && gameBoard->Board[2][1]== "O" && gameBoard->Board[2][2]== "O") |
        (gameBoard->Board[0][0]== "O" && gameBoard->Board[1][0]== "O" && gameBoard->Board[2][0]== "O") |
        (gameBoard->Board[0][1]== "O" && gameBoard->Board[1][1]== "O" && gameBoard->Board[2][1]== "O") |
        (gameBoard->Board[0][2]== "O" && gameBoard->Board[1][2]== "O" && gameBoard->Board[2][2]== "O") |
        (gameBoard->Board[0][0]== "O" && gameBoard->Board[1][1]== "O" && gameBoard->Board[2][2]== "O") |
        (gameBoard->Board[2][0]== "O" && gameBoard->Board[1][1]== "O" && gameBoard->Board[0][2]== "O")
        ) {
            gameBoard->ifEnd[10] = "Player O";
            return true;
    }

}

void FreeGameBoardMemory(GameBoard* gameBoard) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            free(gameBoard->Board[i][j]);
        }
    }
    free(gameBoard->Message[100]);
}

int main(int argc, char*argv[]) {
    if (argc != 2){
        printf("Invalid number of arguments!\n");
        return -1;
    }
    // ----- Tworzenie Bloków Pamięci ----- //

    // [PROGRAM OTWARTY JAKO DRUGI] // (gracz O)
    HANDLE MapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,                 
        argv[1]
    ); 

    if (MapFile == NULL) {
        // [PROGRAM OTWARTY JAKO PIERWSZY] // (gracz X)
        char* player = "X";
        MapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL, 
            PAGE_READWRITE,
            0,
            MEM_SIZE,
            argv[1]
        );

        if (MapFile == NULL) {
            DWORD error = GetLastError();
            fprintf(stderr, "Could not create file mapping object. Error: %lu\n", error);
            return -1;
        }

        // ----- Budowa Tablicy i Przydzielnie Pamięci ----- //
        GameBoard* gameBoard = (GameBoard*)MapViewOfFile(
            MapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            MEM_SIZE
        );

        if (gameBoard == NULL) {
            DWORD error = GetLastError();
            fprintf(stderr, "Could not map view of file. Error %lu\n", error);
            CloseHandle(MapFile);
            return -1;
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                gameBoard->Board[i][j] = " ";
            }
        }
        gameBoard->Message[100] = player;
        gameBoard->currentPlayer[10] = "X";
        gameBoard->ifEnd[10] = "None";
        gameBoard->specialEnd[10] = "None";

        // ----- Ładowanie ekranu ----- //
        printf("Waiting for enemy...");       
        while(strcmp(gameBoard->Message[100], "Ready") != 0){
            for (int i = 0;i < 3;i++){
                for (int j = 0; j < 3;j++){
                    if (j == 2){
                        printf(".");
                    }
                    Sleep(60);
                }
                //system("cls"); 
            }
        }
        int round = 5;
        printf("\nYou play as \"X\"\n");
        printf("\n");
        DisplayBoard(gameBoard);
        int x = -1, y = -1;
        int pkt;
        bool info = false;
        Point* point = (Point*)malloc(sizeof(Point));
        while(round > 0) {
            if (strcmp(gameBoard->currentPlayer[10], "X") == 0){
                if (WhoWin(gameBoard) || (strcmp(gameBoard->ifEnd[10], "None") != 0)){
                    DisplayBoard(gameBoard);
                    printf("\n\t\t\t\t\t  Winner: %s", gameBoard->ifEnd[10]);
                    gameBoard->currentPlayer[10] = "O";
                    return -1;
                };  
                if (round < 5){
                    DisplayBoard(gameBoard);
                }
                SupportDisplayBoard();
                int var = 0;
                do{
                    if (var != 0){
                        printf("\nWybrano niepoprawny punkt!\n");
                    }
                    printf("\n\t\t\t\t\t\tPunkt: ");
                    scanf("%d", &pkt);
                    LetGetPoint(point, pkt, x, y);
                    var++;
                }while (strcmp(gameBoard->Board[point->x][point->y], " ") != 0);
                var = 0;
                gameBoard->Board[point->x][point->y] = "X";   
                round--;
                DisplayBoard(gameBoard);
                gameBoard->currentPlayer[10] = "O";
                info = true;
            }
            if (info && round > 0) {
                printf("\n\t\t\t\t\t    Wait for enemy...");
                info = false;
            }
            Sleep(1);
        }

        if(WhoWin(gameBoard)){
            printf("\n\t\t\t\t\t\tWinner: %s", gameBoard->ifEnd[10]);
            gameBoard->specialEnd[10] = "WaitForX";
        }
        else{
            printf("\n\t\t\t\t\t\t  Draw!\n\n");
            gameBoard->specialEnd[10] = "WaitForX";
        }
        
        if (MapFile == NULL) {
            DWORD error = GetLastError();
            fprintf(stderr, "Could not create file mapping object. Error %lu\n", error);
            return -1;
        }  
        if(!UnmapViewOfFile(gameBoard)) { 
            fprintf(stderr, "Could not unmap view of file\n");
            CloseHandle(MapFile);
            return -1;
        }
        FreeGameBoardMemory(gameBoard); 
        free(point);
    }
    else{
        // ----- Budowa Tablicy i Przydzielnie Pamięci ----- //
        GameBoard* gameBoard = (GameBoard*)MapViewOfFile(
            MapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            MEM_SIZE
        );

        if (gameBoard == NULL){
            DWORD error = GetLastError();
            fprintf(stderr, "Could not map view of file. Error %lu\n", error);
            CloseHandle(MapFile);
            return -1;
        }

        // ----- Moduł do ładowania ekranu ----- //
        gameBoard->Message[100] = "Ready";
        int round = 4;
        printf("\nYou play as \"O\"\n");
        printf("\n");
        DisplayBoard(gameBoard);
        int x = -1, y = -1;
        int pkt;
        bool info = true;
        Point* point = (Point*)malloc(sizeof(Point));
        while(round > 0) {
            if (strcmp(gameBoard->currentPlayer[10], "O") == 0){
                if (WhoWin(gameBoard) || (strcmp(gameBoard->ifEnd[10], "None") != 0)){
                    DisplayBoard(gameBoard);
                    printf("\n\t\t\t\t\t  Winner: %s", gameBoard->ifEnd[10]);
                    gameBoard->currentPlayer[10] = "X";
                    return -1;
                }; 
                DisplayBoard(gameBoard);
                SupportDisplayBoard();
                int var = 0;
                do{
                    if (var != 0){
                        printf("\nWybrano niepoprawny punkt!\n");
                    }
                    printf("\n\t\t\t\t\t\tPunkt: ");
                    scanf("%d", &pkt);
                    LetGetPoint(point, pkt, x, y);
                    var++;
                }while (strcmp(gameBoard->Board[point->x][point->y], " ") != 0);
                var = 0;
                gameBoard->Board[point->x][point->y] = "O";
                DisplayBoard(gameBoard);       
                round--;
                gameBoard->currentPlayer[10] = "X";
                info = true;
            }
            if (info && round > 0){
                printf("\n\t\t\t\t\t    Wait for enemy...");
                info = false;
            }
            Sleep(1);
        }

        while(strcmp(gameBoard->specialEnd[10], "WaitForX") != 0){
            Sleep(1);
        }

        if (strcmp(gameBoard->ifEnd[10], "None") != 0) {
            system("cls");
            DisplayBoard(gameBoard);
            printf("\n\t\t\t\t\t\tWinner: %s", gameBoard->ifEnd[10]);
            return 1;
        }
        DisplayBoard(gameBoard);
        printf("\n\t\t\t\t\t\t  Draw!\n\n");
        if(!UnmapViewOfFile(gameBoard)) { 
            fprintf(stderr, "Could not unmap view of file\n");
            CloseHandle(MapFile);
            return -1;
        }
        free(point);
        CloseHandle(MapFile);
        FreeGameBoardMemory(gameBoard); 
    }
    return 0;
}