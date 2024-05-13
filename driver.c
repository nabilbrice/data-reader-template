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
float *get_table_value(float *table, int p, int i, int k) {
  int stride_k = TABLE_DIM_E * TABLE_DIM_P;
  int stride_i = TABLE_DIM_P;
  return &table[stride_k * k + stride_i * i + p];
}

void load_subtable(float *table, FILE *file, int k) {
  char buff[1024];

  int i = 0;
  float temperature, density;
  // condition to capture end of file and end of table
  // while loop is needed to bypass the comments
  while (fgets(buff, sizeof(buff), file)&&(i<TABLE_DIM_E)) {
    if (buff[0]=='#') {
      if (sscanf(buff, "%*s %f %f", &temperature, &density)) {
        printf("logT = %f and logR = %f\n", temperature, density);
      } else {
        printf("skipped commented line\n");
      };
      continue;
    };
    sscanf(buff, "%f %f", get_table_value(table, 0, i, k), get_table_value(table, 1, i, k));
    printf("subtable: %f %f \n", *get_table_value(table, 0, i, k), *get_table_value(table, 1, i, k));
    i++;
  };
  printf("finished loading table %d\n", k);
}

float *initialise_table(FILE *file) {
  float *table = malloc(TABLE_DIMS * sizeof(table));
  if (table == NULL) {
    printf("Error allocating memory for table");
    return table;
  };

  for (int k=0; (k < NUMBER_OF_TABLES); k++) {
    load_subtable(table, file, k);
  };

  return table;
}


int main() {
  FILE *file;
  int i, k;

  file = fopen("example.dat", "r");
  float *boxed_table = initialise_table(file);
  fclose(file);

  // test getting some of the table values after initialisation:
  i = 0;
  k = 0;
  printf("For i = %d in table %d\n", i, k+1);
  printf("Energy[k] = %f, Scattering cross-section: %f\n", 
    *get_table_value(boxed_table, 0, i, k), 
    *get_table_value(boxed_table, 1, i, k));
  i = 1;
  k = 1;
  printf("For i = %d in table %d\n", i, k+1);
  printf("Energy[k] = %f, Scattering cross-section: %f\n", 
    *get_table_value(boxed_table, 0, i, k), 
    *get_table_value(boxed_table, 1, i, k));

  free(boxed_table);
  return 0;

}
