#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// currently the information about dimensions of the data
// have to be given here:
// giving the incorrect dimensions results in a garbled table
const int DIM_R = 9; // number of rows per subtable
const int DIM_C = 2; // number of columns per subtable
const int DIM_F = 2; // faster varying label
const int DIM_S = 3; // slower varying label
const int TABLE_SIZE = DIM_R * DIM_C * DIM_F * DIM_S;

// T_ijk with j being adjacent in memory
float *get_table_value(float *table, int p, int i, int j, int k) {
  int stride_k = DIM_F * DIM_R * DIM_C;
  int stride_j = DIM_R * DIM_C;
  int stride_i = DIM_C;
  // stride_p = 1;
  return &table[stride_k * k + stride_j * j + stride_i * i + p];
}

void load_subtable(float *table, FILE *file, int j, int k) {
  char buff[1024];

  int i = 0;
  float temperature, density;
  // condition to capture end of file and end of table
  // while loop is needed to bypass the comments
  while (fgets(buff, sizeof(buff), file)&&(i<DIM_R)) {
    if (buff[0]=='#') {
      if (sscanf(buff, "%*s %f %f", &temperature, &density)) {
        printf("logT = %f and logR = %f\n", temperature, density);
      } else {
        printf("skipped commented line\n");
      };
      continue;
    };
    sscanf(buff, "%f %f", get_table_value(table, 0, i, j, k), get_table_value(table, 1, i, j, k));
    printf("subtable: %f %f \n", *get_table_value(table, 0, i, j, k), *get_table_value(table, 1, i, j, k));
    i++;
  };
  printf("finished loading table %d\n", DIM_F * k + j + 1);
}

float *initialise_table(FILE *file) {
  float *table = malloc(TABLE_SIZE * sizeof(table));
  if (table == NULL) {
    printf("Error allocating memory for table");
    return table;
  };

  for (int k=0; (k < DIM_S); k++) {
    for (int j=0; (j < DIM_F); j++) {
      load_subtable(table, file, j, k);
    };
  };

  return table;
}


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
  printf("For i = %d in table %d\n", i, DIM_F*k + j + 1);
  printf("Energy[k] = %f, Scattering cross-section: %f\n", 
    *get_table_value(boxed_table, 0, i, j, k), 
    *get_table_value(boxed_table, 1, i, j, k));
  i = 6;
  j = 1;
  k = 2;
  printf("For i = %d in table %d\n", i, DIM_F*k + j + 1);
  printf("Energy[k] = %f, Scattering cross-section: %f\n", 
    *get_table_value(boxed_table, 0, i, j, k), 
    *get_table_value(boxed_table, 1, i, j, k));

  free(boxed_table);
  return 0;

}
