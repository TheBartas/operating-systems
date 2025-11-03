// SO IS1 211A LAB11
// Bartłomiej Szewczyk
// sb53955@zut.edu.pl


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<errno.h>
#include<windows.h>
#include<sys/time.h>
// gcc -pthread prog.c 

// ------ Zmienne Globalne ------ //
float AllSum = 0;
HANDLE mutex;
#define BILLION  1000000000.0L;
// --------- Struktura --------- //
typedef struct {
    int index;
    int end;
    int size;
    float* array;
} ImCrazy;


double DieZeit(clock_t t1, clock_t t2){
    double diff = 0;
    diff = ((double)(t2 - t1))/ CLOCKS_PER_SEC;
    return diff;
}


bool CheckIfGood(int size, int threads) { // size to "n"
    if (size < 0 || size > 1000000) {
        return false;
    }
    if (threads < 0 || threads > size) return false;
    return true;
}

void CreateAnArray(int n, float * array, float * arrayMech) {
    srand(0);
    double var = 0;
    for (int i = 0; i < n;i++){
        var = 1000.*rand() / RAND_MAX;
        array[i] = var;
        arrayMech[i] = var;
    }
    /*
    for (int i = 0; i < n;i++){
        printf("Element: %f\n", array[i]);
    }
    */
}

float CalculateArray(int n, int index, float* array){
    float result = 0;
    for (int i = index; i < index + n;i++){
        result+=array[i];
    }
    return result;
}

DWORD WINAPI thread(LPVOID data){
    DWORD self = GetCurrentThreadId();
    ImCrazy* rangeData = (ImCrazy*)data;
    int size = rangeData->size;
    int index = rangeData->index;
    float* array = rangeData->array;
    float threadResult = CalculateArray(size, index, array);
    printf("Thread: %xd | Size: %d\n", self, size);
    srand(self);
    DWORD delay = rand() % 5 + 1;
    Sleep(delay);
    printf("Thread: %xd | Sum: %f\n", self, threadResult);

    //---- Sekcja z Mutexem ----//
    WaitForSingleObject(mutex, INFINITE);
    AllSum+=threadResult;
    ReleaseMutex(mutex);
    return 0;
}

// -------- MAIN --------- //

int main(int argc, char* argv[]) {
    //----------- Sekcja warningów -----------//
    if (argc != 3) {
        fprintf(stderr, "%s", "Error: Invalid number of arguments!\n");
        return 0;
    }
    char* endarg1;
    char* endarg2;
    int check1 = strtol(argv[1], &endarg1, 10); // check1 to liczba wyrazów
    int check2 = strtol(argv[2], &endarg2, 10); // check2 to ilość wątków
    errno = 0;
    if (!CheckIfGood(check1, check2)) {
        fprintf(stderr, "%s", "Error: Invalid data format!\n");
        return 0;
    }
    if (argc == 3) {
        if (*endarg1 != '\0' || *endarg2 != '\0' || errno != 0) {
            fprintf(stderr, "%s", "Invalid data format!\n");
            return 1;
        }
    }
    
    //----- Wypis wprowadzonych danych -----//
    printf("Good data format: [%s] | [%s]\n", argv[1], argv[2]);

    //--------- Tworzenie tablicy ---------//
    float* array = (float*)malloc(check1 * sizeof(float));
    float* arrayMech = (float*)malloc(check1 * sizeof(float));
    CreateAnArray(check1, array, arrayMech);

    //------ Sekcja dzieląca z resztą ------//
    int remainder = check1 % check2;
    printf("The remainder of dividing %d by %d is: %d\n", check1, check2, remainder);
    int realSizeNminusOne = (check1 - remainder) / check2;
    int theLastOneSize = realSizeNminusOne + remainder;
    printf("Threads n - 1 got size of: %d | Thread n got size of: %d\n", realSizeNminusOne, theLastOneSize);

    //----- Tworzenie Mutexa -----//
    mutex = CreateMutex(NULL, FALSE, NULL);

    //---------- Sekcja wątków ------------//

    struct timeval begin, end, begin2, end2;
    gettimeofday(&begin, 0); // drugi argument to okreslenie strefy czasowej; nie ma to większego znaczenia

    double myTime = 0;
    HANDLE threads[check2];
    ImCrazy ranges[check2];  
    for (int i = 0; i < check2; i++) {
        ranges[i].index = i * realSizeNminusOne;
        ranges[i].size = (i == check2 - 1) ? theLastOneSize : realSizeNminusOne;
        ranges[i].end = (i == check2 - 1) ? check1 : (i + 1) * realSizeNminusOne;
        ranges[i].array = array;
        threads[i] = CreateThread(NULL, 0, thread, &ranges[i], 0, NULL);
        if (threads[i] == NULL) {
            fprintf(stderr, "Invalid Thread Create Proccess.\n");
            return 1;
        }
    }
    WaitForMultipleObjects(check2, threads, TRUE, INFINITE);
    for (int i = 0; i < check2; ++i) {
        CloseHandle(threads[i]);
    }
    CloseHandle(mutex);

    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double myTimeOne = seconds + microseconds*1e-6;

    //---- Liczenie Mechaniczne ----//

    gettimeofday(&begin2, 0);
    //Sleep(500);
    float result = CalculateArray(check1, 0,arrayMech);

    gettimeofday(&end2, 0);
    long seconds2 = end2.tv_sec - begin2.tv_sec;
    long microseconds2 = end2.tv_usec - begin2.tv_usec;
    double myTimeTwo = seconds2 + microseconds2*1e-6;

    //------- Wyniki --------//
    printf("w/Threads: sum = %f | Time: %lf\n", AllSum, myTimeOne);
    printf("wo/Threads: sum = %f | Time: %lf\n", result, myTimeTwo);
    free(array);
    free(arrayMech);
    return 0;
}
