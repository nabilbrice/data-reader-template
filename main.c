#include <stdio.h>
#include <stdlib.h>
#include "driver_table_reader.h"

const int MAIN_DIM_F = 2;

int main() {
  FILE *file;
  int i, j, k;

  file = fopen("op10_5.dat", "r");
  float *boxed_table = initialise_table(file);
  fclose(file);

  // test getting some of the table values after initialisation:
  i = 4;
  j = 6;
  k = 5;
    float *row = get_table_row(boxed_table, i, j, k);
  printf("For energy index = %d in table %d\n", i, MAIN_DIM_F*k + j + 1);
  printf("Energy[k] = %f, parameters: %f %f %f %f %f %f \n",
         row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
  i = 6;
  j = 0;
  k = 0;
  row = get_table_row(boxed_table, i, j, k);
  printf("For energy index = %d in table %d\n", i, MAIN_DIM_F*k + j + 1);
  printf("Energy[k] = %f, parameters: %f %f %f %f %f %f \n",
         row[0], row[1], row[2], row[3], row[4], row[5], row[6]);

  free(boxed_table);
  return 0;

}
