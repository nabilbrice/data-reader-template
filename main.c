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

    int index = closest_indices(boxed_table.log_Ts, DIM_S, 4.910);
    printf("Closest index temp: %d \n", index);
    
    double omega_plus = interpolated_value(&boxed_table, 5.39, -8, 1.04, 0);
    double omega_minus = interpolated_value(&boxed_table, 5.39, -8, 1.04, 1);
    double omega_zero = interpolated_value(&boxed_table, 5.39, -8, 1.04, 2);
    double sigma_plus = interpolated_value(&boxed_table, 5.39, -8, 1.04, 3);
    double sigma_minus = interpolated_value(&boxed_table, 5.39, -8, 1.04, 4);
    double sigma_zero = interpolated_value(&boxed_table, 5.39, -8, 1.04, 5);
    printf("Interpolated values = %lf, %lf, %lf, %4.3e, %4.3e, %4.3e \n", omega_plus, omega_minus, omega_zero, sigma_plus, sigma_minus, sigma_zero);
    
  free_table(&boxed_table);
  return 0;

}
