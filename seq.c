#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define MILIARD 1000000000.0

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
  for(i=0; i<col; i++){
    for(j=0; j<row; j++){
      (*array)[j][i] = 0;
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

int main(){

  // Zmienne
  int i,j;
  int counter, a_steps = 50, last_used = 1, area_amount = 0;
  int N = 20; //Liczba wierszy
  int M = 20; //Liczba kolumn
  int **arr1;
  int **arr2;

  FILE *data;
  FILE *result;
  int data_row, data_col, a_cells;

  //zmienne czasowe
  struct timespec start, stop;
  double time, sum = 0;

  clock_gettime( CLOCK_REALTIME, &start);

  // Tworzenie tablic
  arr1 = allocation(N, M);
  arr2 = allocation(N, M);

  // Zerowanie tablic
  array_zero(N, M, &arr1);
  array_zero(N, M, &arr2);

  // Wczytywanie pozycji startowych
  if((data = fopen("data.txt", "r")) == NULL){
    printf("BLAD OTWIERANA PLIKU: DATA\n");
    exit(1);
  }
  fscanf(data, "%d", &a_cells);

  for(i=0; i<a_cells; i++){
    fscanf(data, "%d %d", &data_row, &data_col);
    arr1[data_row][data_col] = 1;
  }

  fclose(data);

  clock_gettime( CLOCK_REALTIME, &stop);
  time = (( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec )) / MILIARD;
  printf("Czas (tworzenie danych): %lf\n", time);
  sum += time;

  clock_gettime( CLOCK_REALTIME, &start);

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
/*      // Tmp: Rysowanie
      system("clear");
      for(i=0; i<N; i++){
        for(j=0; j<M; j++){
          printf("%d", arr2[i][j]);
        }
        printf("\n");
      }*/
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
/*      // Tmp: Rysowanie
      system("clear");
      for(i=0; i<N; i++){
        for(j=0; j<M; j++){
          printf("%d", arr2[i][j]);
        }
        printf("\n");
      }*/
    }
  }

  clock_gettime( CLOCK_REALTIME, &stop);
  time = (( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec )) / MILIARD;
  printf("Czas (mechanika): %lf\n", time);
  sum += time;

  clock_gettime( CLOCK_REALTIME, &start);

  // Tmp: Wywalanie wyniku do pliku
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

  //Czyszcenie pamięci
  clear(N, arr1);
  clear(N, arr2);

  clock_gettime( CLOCK_REALTIME, &stop);
  time = (( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec )) / MILIARD;
  printf("Czas (zapisywanie wyniku): %lf\n", time);
  sum += time;

  printf("Czas (calosc): %lf\n", sum);

  return 0;
}
