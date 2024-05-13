#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// currently the information about dimensions of the data
// have to be given here:
// giving the incorrect dimensions results in a garbled table
const int TABLE_DIM_E = 9;
const int TABLE_DIM_P = 2;
const int NUMBER_OF_TABLES = 2;
const int TABLE_DIMS = TABLE_DIM_E * TABLE_DIM_P * NUMBER_OF_TABLES;

// T_ijk with j being adjacent in memory
float *get_table_value(float *table, int i, int j, int k) {
  int stride_k = TABLE_DIM_E * TABLE_DIM_P;
  int stride_i = TABLE_DIM_P;
  return &table[stride_k * k + stride_i * i + j];
}

void load_subtable(float *table, FILE *file, int k) {
  char buff[1024];

  int i = 0;
  // condition to capture end of file and end of table
  // while loop is needed to bypass the comments
  while (fgets(buff, sizeof(buff), file)&&(i<TABLE_DIM_E)) {
    if (buff[0]=='#') {
      continue;
    };
    sscanf(buff, "%f %f", get_table_value(table, i, 0, k), get_table_value(table, i, 1, k));
    printf("subtable: %f %f \n", *get_table_value(table, i, 0, k), *get_table_value(table, i, 1, k));
    i++;
  };
  printf("finished loading table %d\n", k);
}


int main() {
  FILE *file;
  int i, k;

  // the table stored in memory:
  float *boxed_table = malloc(TABLE_DIMS * sizeof(boxed_table));

  file = fopen("example.dat", "r");
  for (int k=0; (k < NUMBER_OF_TABLES); k++) {
    load_subtable(boxed_table, file, k);
  };

  i = 0;
  k = 0;
  printf("For i = %d in table %d\n", i, k+1);
  printf("Energy[k] = %f, Scattering cross-section: %f\n", *get_table_value(boxed_table, i, 0, k), 
    *get_table_value(boxed_table, i, 1, k));
  i = 1;
  k = 1;
  printf("For i = %d in table %d\n", i, k+1);
  printf("Energy[k] = %f, Scattering cross-section: %f\n", *get_table_value(boxed_table, i, 0, k), 
    *get_table_value(boxed_table, i, 1, k));

  free(boxed_table);
  return 0;

}
