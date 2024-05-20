#include <stdio.h>
#include <stdlib.h>
#include "driver_table_reader.h"

int main() {
  int i, j, k;
  Table boxed_table;

  initialise_table_from_file(&boxed_table, "op10_5.dat");

  // test getting some of the table values after initialisation:
  i = 4;
  j = 6;
  k = 5;
  double *row = get_table_row(&boxed_table, i, j, k);
  printf("For energy index = %d in table %d\n", i, DIM_F*k + j + 1);
  printf("log_T: %f and log_R: %f\n", boxed_table.log_Ts[k], boxed_table.log_Rs[j]);
  printf("Energy[k] = %f, parameters: %f %f %f %f %f %f \n",
         boxed_table.energies[i], row[0], row[1], row[2], row[3], row[4], row[5]);
  i = 6;
  j = 0;
  k = 0;
  row = get_table_row(&boxed_table, i, j, k);
  printf("For energy index = %d in table %d\n", i, DIM_F*k + j + 1);
  printf("log_T: %f and log_R: %f\n", boxed_table.log_Ts[k], boxed_table.log_Rs[j]);
  printf("Energy[k] = %f, parameters: %4.3e %4.3e %4.3e %4.3e %4.3e %4.3e \n",
         boxed_table.energies[i], row[0], row[1], row[2], row[3], row[4], row[5]);

  free_table(&boxed_table);
  return 0;

}
