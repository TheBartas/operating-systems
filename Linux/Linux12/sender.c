#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define MEM_SIZE 100

typedef struct {
    char fromFile[100];
    char message[20];
    char messageEnd[20];
    char backMgs[10];
} Data;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "\nInvalid Number of Arguments!\n");
        return -1;
    }

    // Otwieranie pliku z argumentu
    int varOpened = open(argv[1], O_RDONLY);
    if (varOpened < 0) {
        fprintf(stderr, "\nCannot open file \"%s\" \n", argv[1]);
        perror("Def ");
        printf("\n");
        return -1;
    }

    // Generowanie klucza
    key_t key = ftok(argv[1], 'a');
    if (key < 0) {
        fprintf(stderr, "Error Creating Key!\n");
        perror("Def ");
        printf("\n");
        return -1;
    } else {
        printf("Key: %d\n", key);
    }

    // Przydzielanie bloku pamięci
    int shmid = shmget(key, MEM_SIZE, 0666 | IPC_CREAT | IPC_EXCL);
    if (shmid < 0) {
        fprintf(stderr, "\nError getting shared memory!\n");
        perror("Def ");
        printf("\n");
        return -1;
    }

    Data *sharedMemory = (Data *)shmat(shmid, NULL, 0);
    if (sharedMemory == (Data *)-1) {
        fprintf(stderr, "\nError getting shared memory!\n");
        perror("Def ");
        printf("\n");
        return -1;
    }

    // Czekanie na info zwrotne
    printf("Waiting for information from receiver...\n");
    while (strcmp(sharedMemory->message, "Ready!") != 0) {
        sleep(1);
    }

    printf("Status: %s\n\n", sharedMemory->message);


    while(1) {
        if (strcmp(sharedMemory->messageEnd, "Now") == 0) {
            int rd = read(varOpened, sharedMemory->fromFile, 100);
            if (rd > 0) {
                strcpy(sharedMemory->messageEnd, "NotNow");
                sharedMemory->fromFile[rd] = '\0';
                while(strcmp(sharedMemory->backMgs, "Go") != 0) {
                    sleep(1);
                }
                strcpy(sharedMemory->backMgs, "Stop");
            }
            else { 
                break;
            }
        }
    }
    strcpy(sharedMemory->message, "End");


    while (strcmp(sharedMemory->message, "Gut gemacht!") != 0) {
        sleep(1);
    }
    printf("Task completed!\n");

    // Odlaczanie pamięci współdzielonej
    if (shmdt(sharedMemory) == -1) {
        fprintf(stderr, "Error detaching shared memory segment!\n");
        perror("Def ");
        printf("\n");
        return -1;
    }

    // Usuwanie pamięci współdzielonej
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "Error removing shared memory\n");
        perror("Def ");
        printf("\n");
        return -1;
    }

    close(varOpened);
    return 0;
}
