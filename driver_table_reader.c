#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// currently the information about dimensions of the data
// have to be given here:
// giving the incorrect dimensions results in a garbled table
const int DIM_R = 301; // number of rows per subtable (no. energy values)
const int DIM_C = 7; // number of columns per subtable (no. variables)
const int DIM_F = 56; // faster varying label (no. log_R)
const int DIM_S = 22; // slower varying label (no. log_T)
const int TABLE_SIZE = DIM_R * DIM_C * DIM_F * DIM_S;

// p is index for parameter (i.e. 0 = energy, 1 = sigma +, 2 = sigma -, etc
// i is index in energy (log_E eV)
// j is index in R (log_R)
// k is index in temp (log_T K)
// Together these pointers will get to the correct index for energy, density and temp provided
float *get_table_value(float *table, int p, int i, int j, int k) {
  int stride_k = DIM_F * DIM_R * DIM_C;
  int stride_j = DIM_R * DIM_C;
  int stride_i = DIM_C;
  // stride_p = 1;
  // returns the address of the one value specified
  // will need to change this to return array of six values for each of the six variables
  return &table[stride_k * k + stride_j * j + stride_i * i + p];
}

float *get_table_row(float *table, int i, int j, int k) {
  int stride_k = DIM_F * DIM_R * DIM_C;
  int stride_j = DIM_R * DIM_C;
  int stride_i = DIM_C;
  // stride_p = 1;
  // returns the address of the first value of the 7 numbers/parameters
  // to access each of the values, index in to this
  return &table[stride_k * k + stride_j * j + stride_i * i];
}

// loads a particular 2D table with rows and columns
void load_subtable(float *table, FILE *file, int j, int k) {
  char buff[1024];

  int i = 0;
  float value_logT, value_logR;
  // condition to capture end of file and end of table
  // while loop is needed to bypass the comments
  while (fgets(buff, sizeof(buff), file)&&(i<DIM_R)) {
    if (buff[0]=='#') {
      if (sscanf(buff, "%*s %f %f", &value_logT, &value_logR)) {
       // printf("logT = %f and logR = %f\n", value_logT, value_logR);
      } else {
       // printf("skipped commented line\n");
      };
      continue;
    };
    float *row = get_table_row(table, i, j, k);
    sscanf(buff, "%f %f %f %f %f %f %f", &row[0], &row[1], &row[2], &row[3], &row[4], &row[5], &row[6]);
    // printf("subtable: %f %f %f %f %f %f %f\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
    i++;
  };
  // printf("finished loading table %d\n", DIM_F * k + j + 1);
}

// requires manually calling free when done with the table
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
