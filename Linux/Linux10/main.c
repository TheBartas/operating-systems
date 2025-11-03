#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include<unistd.h> 
#include<sys/wait.h>
#include<stdbool.h>
#include<pthread.h>
#include<time.h>
// gcc -pthread prog.c 

// ------ Zmienne Globalne ------ //
double AllSum = 3; 
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
#define BILLION  1000000000.0L;

// --------- Struktura --------- //
typedef struct {
    int index;
    int end;
    int size;
} ImCrazy;

// --------- Funkcje --------- //
int Min(int n, int size){
if (n > size) {
    return size;
} else {
    return n;
}
}

bool CheckIfGood(int size, int threads){
    if (size < 1 || size > 1000000){
        return false;
    }
    if (threads < 1 || threads > Min(100, size)) return false;
return true;
}

double CalculatePI(const ImCrazy* yes) {
double result = 0;
int i = yes->index;
for (i+1; i <= yes->size; i++){
    double first = 2*i;
    double second =2*i+1;
    double third = 2*i+2;
    if (i % 2 == 0) {
       result = result + (4.0/((2*(double)i+2)*(2*(double)i+3)*(2*(double)i+4)));
       //result+= (4.0/(first * second * third));
    } else {
       result = result - (4.0/((2*(double)i+2)*(2*(double)i+3)*(2*(double)i+4)));
        //result-= (4.0/(first * second * third));
    }
}
return result;
}



void* CalculatePIThread(void* data){
    pthread_t self = pthread_self();
    srand(self);
    //int* range = (int*)data;
    ImCrazy* yes = (ImCrazy*)data;
    //printf("Thread: %lxd | Rozmiar do przetworzenia: %d | Index początkowy: %d\n", self, range[1], range[0]);
    //double result = CalculatePI(range[0], range[1]);
    printf("Thread: %lxd | Rozmiar do przetworzenia: %d | Index początkowy: %d\n", self, yes->size, yes->index);
    long delay = rand() % 5 + 1;
    //sleep(delay);
    double result = CalculatePI(yes);
    printf("Thread: %lxd | Wynik wątku: %.20lf\n", self,result);
    //sleep(delay);
    //-------- SEKCJA KRTYTCZNA --------//
    pthread_mutex_lock(&mymutex);
    AllSum+=result;
    pthread_mutex_unlock(&mymutex);
    return NULL;
}

// -------- MAIN --------- //

int main(int argc, char*argv[]){
//----------- Sekcja warningów -----------//
setlocale(LC_ALL, "Polish");
if (argc != 3) {
fprintf(stderr, "%s", "Error: Nieprawidłowa liczba argumentów!\n");
return 0;
}
int check1 = atoi(argv[1]); // check1 to liczba wyrazów
int check2 = atoi(argv[2]); // check2 to ilość wątków
if (!CheckIfGood(check1, check2)){
fprintf(stderr, "%s", "Error: Niepoprawny format danych!\n");
return 0;
}


//----- Wypis wprowadzonych danych -----//
printf("Podane dane są zgodne ze standardami: [%s] | [%s]\n", argv[1], argv[2]);

//------ Sekcja dzieląca z resztą ------//

int remainder = check1 % check2;
printf("Reszta z dzielenia %d przez %d wynosi: %d\n", check1, check2, remainder);
int realSizeNminusOne = (check1 - remainder)/ check2;
int theLastOneSize = realSizeNminusOne + remainder;
printf("Wątki n-1 otrzymały rozmiar danych do przetworzenia: %d | Wątek n dostał rozmiar równy: %d\n", realSizeNminusOne, theLastOneSize);


//-------- Tworzenie wątków -----------//
struct timespec start, stop;
double myTime;
if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
    perror("Invalid Start Timer!");
    exit(EXIT_FAILURE);
}


pthread_t threads[check2]; 
ImCrazy ranges[check2];
for (int i = 0; i < check2; i++) {
    ranges[i].index = i * realSizeNminusOne;
    ranges[i].size = (i == check2 - 1) ? theLastOneSize : realSizeNminusOne;
    ranges[i].end = (i == check2 - 1) ? check1 : (i + 1) * realSizeNminusOne;
    pthread_create(threads+i, NULL, CalculatePIThread, &ranges[i]);
}

for (int i = 0;i<check2;i++){
    pthread_join(threads[i], NULL);
}
pthread_mutex_destroy(&mymutex);

if (clock_gettime(CLOCK_REALTIME, &stop) == -1) {
    perror("Invalid Stop Timer!");
    exit(EXIT_FAILURE);
}

    myTime = ( stop.tv_sec - start.tv_sec )
          + ( stop.tv_nsec - start.tv_nsec )
            / BILLION;

// ------ Liczenie mechaniczne --------- //
double MechanicSum;
ImCrazy tooMuch;
tooMuch.index = 0;
tooMuch.size = check1; // można z, można bez
tooMuch.end = check1;
struct timespec startM, stopM;
double myTimeM;
if (clock_gettime(CLOCK_REALTIME, &startM) == -1) {
    perror("Invalid Start \"M\" Timer!");
    exit(EXIT_FAILURE);
}
MechanicSum = CalculatePI(&tooMuch) + 3;

if (clock_gettime(CLOCK_REALTIME, &stopM) == -1) {
    perror("Invalid Stop \"M\" Timer!");
    exit(EXIT_FAILURE);
}

    myTimeM = ( stopM.tv_sec - startM.tv_sec )
          + ( stopM.tv_nsec - startM.tv_nsec )
            / BILLION;

// ------ Wyniki: z wątków (1), mechanicznie (2) ------- //
printf("(W) Thread result = %.20lf | Czas: %lf\n", AllSum, myTime); // (1)
printf("(WO) Thread result = %.20lf | Czas: %lf\n", MechanicSum, myTimeM); // (2)
return 0;
}
