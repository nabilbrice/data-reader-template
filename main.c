#include <stdio.h>
#include <stdlib.h>
#include "driver_table_reader.h"

const int MAIN_DIM_F = 2;

int main() {
  FILE *file;
  int i, j, k;

  file = fopen("example.dat", "r");
  float *boxed_table = initialise_table(file);
  fclose(file);

  // test getting some of the table values after initialisation:
  i = 0;
  j = 0;
  k = 0;
  printf("For i = %d in table %d\n", i, MAIN_DIM_F*k + j + 1);
  printf("Energy[k] = %f, Scattering cross-section: %f\n", 
    *get_table_value(boxed_table, 0, i, j, k), 
    *get_table_value(boxed_table, 1, i, j, k));
  i = 6;
  j = 1;
  k = 2;
  printf("For i = %d in table %d\n", i, MAIN_DIM_F*k + j + 1);
  printf("Energy[k] = %f, Scattering cross-section: %f\n", 
    *get_table_value(boxed_table, 0, i, j, k), 
    *get_table_value(boxed_table, 1, i, j, k));

  free(boxed_table);
  return 0;

}
