#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


typedef struct {
    char fromFile[100];
    char message[20];
    char messageEnd[20];
    char backMgs[10];
}Data;

int main(int argc, char*argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Invalid Number of Arguments!\n");
        return -1; 
    }

    // ----- Uzyskiwanie klucza i przypdzielenie bloku pamięci ----- //
    key_t key = atoi(argv[1]);
    int shmid = shmget(key, 100, 0666);
    if (shmid < 0) {
        fprintf(stderr, "\nError getting shared memory!\n");
        printf("Error value : %d\n", errno);
        perror("Def ");
        printf("\n");
        return -1;
    }
    
    // ----- Tworzenie pliku z argumentu 2 ----- //
    int openFile2 = open(argv[2], O_WRONLY | O_CREAT);
    if (openFile2 < 0) {
        fprintf(stderr, "\nCannot open file \"%s\" \n", argv[2]);
        printf("Error value : %d\n", errno);
        perror("Def ");
        printf("\n");
        return -1;
    }


    // ----- Info zwrotne ----- //
    Data *sharedMemory = (Data*)shmat(shmid, NULL, 0); 

    if (sharedMemory == (Data *)-1) {
        fprintf(stderr, "Error attaching shared memory segment.\n");
        printf("Error value : %d\n", errno);
        perror("Def ");
        printf("\n");
        return -1;
    }
    strcpy(sharedMemory->message, "Ready!");
    strcpy(sharedMemory->messageEnd, "Now");
    strcpy(sharedMemory->backMgs, "Stop");


    int i = 0;
    while(strcmp(sharedMemory->message, "End") != 0) {
         if (strcmp(sharedMemory->messageEnd, "NotNow")) {
            int wr = write(openFile2, sharedMemory->fromFile, strlen(sharedMemory->fromFile));
            printf("%s\n", sharedMemory->fromFile);
            //getchar();
            sleep(3);
            //printf("Progress: %d\n", i++);     
            strcpy(sharedMemory->messageEnd, "Now");
            strcpy(sharedMemory->backMgs, "Go");
         }
           
    }
    //strcpy(sharedMemory->message, "Gut gemacht!");
    sharedMemory->message[20] = "Gut gemacht!";
    printf("\n\nTask completed!\n");

    if (shmdt(sharedMemory) == -1) {  // Odlaczanie pamięci współdzielonej
        fprintf(stderr, "Error detaching shared memory segment!\n");
        printf("Error value : %d\n", errno);
        perror("Def ");
        printf("\n");
        return -1;
    }
    close(openFile2);

    return 0;
}
