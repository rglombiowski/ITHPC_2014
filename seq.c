#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

//#define MILIARD 1000000000.0

// Alokacja pamięci
int** allocation(int row, int col){
  int i;
  int **array;
  // Alokacja tablicy
  array = (int**)calloc(row, sizeof(int*));

  // Sprawdzenie
  assert(array != NULL);
  /*if(array == NULL){
    printf("BLAD ALOKACJI: 1D");
  }*/

  // Alokacja drugiego wymiaru
  for(i=0; i<row; i++){
    (array)[i] = (int*)calloc(col, sizeof(int));

    // Sprawdzenie
    assert((array)[i] != NULL);
    /*if((array)[i] == NULL){
      printf("BLAD ALOKACJI: 2D");
    }*/
  }

  return array;
}

// Czyszczenie pamięci
void clear(int row, int** array){
  int i;

  for(i=0; i<row; i++){
    free(array[i]);
  }

  free(array);
}

// Zerowanie tablicy
void array_zero(int row, int col, int*** array){
  int i,j;
  for(i=0; i<row; i++){
    for(j=0; j<col; j++){
      (*array)[i][j] = 0;
    }
  }
}

// Sprawdzanie sąsiadów
int area_check(int row, int col, int max_row, int max_col, int** array){
  int amount = 0;

  // Lewo góra
  if(row == 0 && col == 0){
    if(array[row][col+1] == 1) amount++;
    if(array[row+1][col] == 1) amount++;
    if(array[row+1][col+1] == 1) amount++;
  }else{
    // Lewo
    if(col == 0 && row < max_row-1){
      if(array[row-1][col] == 1) amount++;
      if(array[row-1][col+1] == 1) amount++;
      if(array[row][col+1] == 1) amount++;
      if(array[row+1][col] == 1) amount++;
      if(array[row+1][col+1] == 1) amount++;
    }else{
      // Góra
      if(row == 0 && col < max_col-1){
        if(array[row][col-1]) amount++;
        if(array[row][col+1]) amount++;
        if(array[row+1][col-1]) amount++;
        if(array[row+1][col]) amount++;
        if(array[row+1][col+1]) amount++;
      }else{
        // Prawo góra
        if(row == 0 && col == max_col-1){
          if(array[row][col-1]) amount++;
          if(array[row+1][col-1]) amount++;
          if(array[row+1][col]) amount++;
        }else{
          // Lewo dół
          if(row == max_row-1 && col == 0){
            if(array[row-1][col]) amount++;
            if(array[row-1][col+1]) amount++;
            if(array[row][col+1]) amount++;
          }else{
            // Prawo
            if(col == max_col-1 && row < max_row-1){
              if(array[row-1][col-1]) amount++;
              if(array[row-1][col]) amount++;
              if(array[row][col-1]) amount++;
              if(array[row+1][col-1]) amount++;
              if(array[row+1][col]) amount++;
            }else{
              // Dół
              if(row == max_row-1 && col < max_col-1){
                if(array[row-1][col-1]) amount++;
                if(array[row-1][col]) amount++;
                if(array[row-1][col+1]) amount++;
                if(array[row][col-1]) amount++;
                if(array[row][col+1]) amount++;
              }else{
                // Prawo dół
                if(row == max_row-1 && col == max_col-1){
                  if(array[row-1][col-1]) amount++;
                  if(array[row-1][col]) amount++;
                  if(array[row][col-1]) amount++;
                }else{
                  //General
                  if(array[row-1][col-1] == 1) amount++;
                  if(array[row-1][col] == 1) amount++;
                  if(array[row-1][col+1] == 1) amount++;
                  if(array[row][col-1] == 1) amount++;
                  if(array[row][col+1] == 1) amount++;
                  if(array[row+1][col-1] == 1) amount++;
                  if(array[row+1][col] == 1) amount++;
                  if(array[row+1][col+1] == 1) amount++;
                }
              }
            }
          }
        }
      }
    }
  }

  return amount;
}

double timeDiff(struct timespec *timeA_p, struct timespec *timeB_p){
  double diff = (((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
    ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec));
  return diff / 1000000000;
}

int main(int argc, char *argv[]){

  // Sprawdzenie poprawności uruchomienia
  // Wymagane dane: wielkość macierzy, ilość kroków, plik danych, info o rysowaniu, info o zapisywaniu do pliku
  if(argc != 6){
    printf("BLAD: ZA MALO DANYCH\n");
    printf("Konstrukcja: program wielkosc_macierzy ilosc_krokow plik_danych(sama nazwa) animacja(0-nie, 1-tak) zapisywanie_do_pliku(0-nie, 1-tak)\n");
    exit(1);
  }

  // Zmienne
  int i,j;
  int counter, last_used = 1, area_amount = 0;
  int N = atoi(argv[1]); //Liczba wierszy
  int M = atoi(argv[1]); //Liczba kolumn
  int a_steps = atoi(argv[2]); // Ilość kroków
  int i_draw = atoi(argv[4]); // Czy rysować
  int i_write = atoi(argv[5]); // Czy zapisywać wyniki do pliku
  int **arr1;
  int **arr2;

  // Zmienne plikowe
  char cwd[100]; //Aktualnie działający katalog
  char file_name[150]; // Pełna ścieżka do pliku

  FILE *data;
  FILE *result;
  int data_row, data_col, a_cells;

  //zmienne czasowe
  struct timespec start1, stop1, start2, stop2, start3, stop3;
  double time, sum = 0;

  // Konstruowanie ścieżki do pliku
  if(getcwd(cwd, sizeof(cwd)) != NULL){ // getcwd pobiera info o aktualnie działającym katalogu
    strcpy(file_name,cwd);
  }else{
    printf("BLAD: BLAD PRZY POBIERANIU INFORMACJI O AKTUALNYM KATALOGU\n");
    exit(1);
  }
  strcat(file_name, "/");
  strcat(file_name, argv[3]);
  // Sprawdzenie poprawności ścieżki
//  printf("Sciezka: %s\n", file_name);

//  clock_gettime( CLOCK_REALTIME, &start1);
  clock_gettime(CLOCK_MONOTONIC, &start1);

  // Tworzenie tablic
  arr1 = allocation(N, M);
  arr2 = allocation(N, M);

  // Zerowanie tablic
  array_zero(N, M, &arr1);
  array_zero(N, M, &arr2);

  // Wczytywanie pozycji startowych
  if((data = fopen(file_name, "r")) == NULL){
    printf("BLAD OTWIERANA PLIKU: DATA\n");
    exit(1);
  }
  fscanf(data, "%d", &a_cells);

  for(i=0; i<a_cells; i++){
    fscanf(data, "%d %d", &data_row, &data_col);
    arr1[data_row][data_col] = 1;
  }

  fclose(data);

//  clock_gettime( CLOCK_REALTIME, &stop1);
  clock_gettime(CLOCK_MONOTONIC, &stop1);

//  time = (( stop1.tv_sec - start1.tv_sec ) + ( stop1.tv_nsec - start1.tv_nsec )) / MILIARD;
  time = timeDiff(&stop1, &start1);

  printf("Czas (tworzenie danych): %lf\n", time);
  sum += time;
  time = 0;

//  clock_gettime( CLOCK_REALTIME, &start2);
  clock_gettime(CLOCK_MONOTONIC, &start2);

  // Mechanika
  for(counter = 1; counter <= a_steps; counter++){
    if(last_used == 1){
      for(i=0; i<N; i++){
        for(j=0; j<M; j++){
          area_amount = area_check(i, j, N, M, arr1);
          if(arr1[i][j] == 0 && area_amount == 3){
            arr2[i][j] = 1;
          }else{
            if(arr1[i][j] == 1 && (area_amount == 3 || area_amount == 2)){
              arr2[i][j] = 1;
            }else{
              arr2[i][j] = 0;
            }
          }
        }
      }
      last_used = 2;
      // Rysowanie
      if(i_draw == 1){
        system("clear");
        for(i=0; i<N; i++){
          for(j=0; j<M; j++){
            printf("%d", arr2[i][j]);
          }
          printf("\n");
        }
        sleep(1);
      }
    }else{
      for(i=0; i<N; i++){
        for(j=0; j<M; j++){
          area_amount = area_check(i, j, N, M, arr2);
          if(arr2[i][j] == 0 && area_amount == 3){
            arr1[i][j] = 1;
          }else{
            if(arr2[i][j] == 1 && (area_amount == 3 || area_amount == 2)){
              arr1[i][j] = 1;
            }else{
              arr1[i][j] = 0;
            }
          }
        }
      }
      last_used = 1;
      // Rysowanie
      if(i_draw == 1){
        system("clear");
        for(i=0; i<N; i++){
          for(j=0; j<M; j++){
            printf("%d", arr1[i][j]);
          }
          printf("\n");
        }
        sleep(1);
      }
    }
  }

//  clock_gettime( CLOCK_REALTIME, &stop2);
  clock_gettime(CLOCK_MONOTONIC, &stop2);

//  time = (( stop2.tv_sec - start2.tv_sec ) + ( stop2.tv_nsec - start2.tv_nsec )) / MILIARD;
  time = timeDiff(&stop2, &start2);

  printf("Czas (mechanika): %lf\n", time);
  sum += time;
  time = 0;

  // Wypisywanie wyniku do pliku
  if(i_write == 1){
//    clock_gettime( CLOCK_REALTIME, &start3);
    clock_gettime(CLOCK_MONOTONIC, &start3);

    if((result = fopen("result.txt", "w")) == NULL){
      printf("BLAD OTWIERANA PLIKU: RESULT\n");
      exit(1);
    }

    if(last_used == 1){
      for(i=0; i<N; i++){
        for(j=0; j<M; j++){
          fprintf(result, "%d", arr1[i][j]);
        }
        fprintf(result, "\n");
      }
    }else{
      for(i=0; i<N; i++){
        for(j=0; j<M; j++){
          fprintf(result, "%d", arr2[i][j]);
        }
        fprintf(result, "\n");
      }
    }

    fclose(result);

//    clock_gettime( CLOCK_REALTIME, &stop3);
    clock_gettime(CLOCK_MONOTONIC, &stop3);

//    time = (( stop3.tv_sec - start3.tv_sec ) + ( stop3.tv_nsec - start3.tv_nsec )) / MILIARD;
    time = timeDiff(&stop3, &start3);

    printf("Czas (zapisywanie wyniku): %lf\n", time);
    sum += time;
  }

  //Czyszcenie pamięci
  clear(N, arr1);
  clear(N, arr2);

  printf("Czas (calosc): %lf\n", sum);

  return 0;
}
