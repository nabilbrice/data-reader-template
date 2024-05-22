#include <stdio.h>
#include <stdlib.h>
#include "driver_table_reader.h"

int main() {
  int i, j, k;
  TableCollection collection;

  initialise_tables_from_file(&collection, "op10_5.dat");

  // test getting some of the table values after initialisation:
  i = 4;
  j = 6;
  k = 5;
  TableRow *row = get_table_row(&collection, i, j, k);
  printf("For energy index = %d in table %d\n", i, DIM_FAST*k + j + 1);
  printf("log_T: %f and log_R: %f\n", collection.log_Ts[k], collection.log_Rs[j]);
  printf("Energy[k] = %f, parameters: %f %f %f %f %f %f \n",
        collection.row_labels[i],
        row->entry[0], 
        row->entry[1], 
        row->entry[2],
        row->entry[3],
        row->entry[4],
        row->entry[5]);
  i = 6;
  j = 0;
  k = 0;
  row = get_table_row_from_key(&collection, i, 4.91, -7.48);
  printf("For energy index = %d in table %d\n", i, DIM_FAST*k + j + 1);
  printf("log_T: %f and log_R: %f\n", collection.log_Ts[k], collection.log_Rs[j]);
  printf("Energy[k] = %f, parameters: %4.3e %4.3e %4.3e %4.3e %4.3e %4.3e \n",
         collection.row_labels[i], 
          row->entry[0], 
          row->entry[1], 
          row->entry[2], 
          row->entry[3], 
          row->entry[4], 
          row->entry[5]);

  drop_tables(&collection);

  return 0;

}
