#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "driver_table_reader.h"

const int TABLE_SIZE = DIM_R * DIM_C * DIM_F * DIM_S;
// p is index for parameter (i.e. 0 = energy, 1 = sigma +, 2 = sigma -, etc
// i is index in energy (log_E eV)
// j is index in R (log_R)
// k is index in temp (log_T K)
// Together these pointers will get to the correct index for energy, density and temp provided
float *get_mut_table_value(Table *table, int p, int i, int j, int k) {
  // returns a pointer to a value in the table
  // this can be used to write (mutate) the value stored in the table
  return &table->data[table->stride_energy * i 
                    + table->stride_log_Rs * j 
                    + table->stride_log_Ts * k 
                    + p];
}

float *get_table_row(Table *table, int i, int j, int k) {
  // returns a pointer to the first value of the 7 numbers/parameters
  // to access each of the values, index in to this: row[p]
  return &(table->data[ table->stride_log_Ts * k 
                      + table->stride_log_Rs * j 
                      + table->stride_energy * i ]);
}

void save_into_array(float *array, float value, int index) {
  array[index] = value;
}

void fload_subtable_header(char buff[], Table *table, int j, int k) {
  float log_T, log_R;
  // check the formatting first
  if (sscanf(buff, "%*s %f %f", &log_T, &log_R)) {
    table->log_Ts[k] = log_T;
    table->log_Rs[j] = log_R;
  };
}

// loads a 2D table into the memory specified by index j and k
// from a file specified 
void load_subtable(Table *table, FILE *file, int j, int k) {
  char buff[1024];

  int i = 0;
  // condition to capture end of file and end of table
  while (fgets(buff, sizeof(buff), file)&&(i<DIM_R)) {
    // check if the line starts with the comment line char
    if (buff[0]=='#') {
      // some comment lines contain information about the table
      // this updates the array of log_Ts and log_Rs
      fload_subtable_header(buff, table, j, k);
      continue;
    };

    float *row = get_table_row(table, i, j, k);
    sscanf(buff, "%f %f %f %f %f %f %f", 
      &(table->energies[i]),  // energies are always the first entry
      &row[0], &row[1], &row[2], &row[3], &row[4], &row[5]);
    i++;
  };
  // printf("finished loading table %d\n", DIM_F * k + j + 1);
}

// requires manually calling free when done with the table
void initialise_table(Table *table, FILE *file) {
  // allocate the space required for the data
  table->data = malloc(TABLE_SIZE * sizeof(float));
  if (table->data == NULL) {
    printf("Error allocating for table data");
  };

  table->stride_energy = DIM_C;
  table->stride_log_Rs = DIM_R*DIM_C;
  table->stride_log_Ts = DIM_F*DIM_R*DIM_C;

  for (int k=0; (k < DIM_S); k++) {
    for (int j=0; (j < DIM_F); j++) {
      load_subtable(table, file, j, k);
    };
  };
}

void initialise_table_from_file(Table *table, char *filename) {
  FILE *file = fopen(filename, "r");
  initialise_table(table, file);
  fclose(file);
}

void free_table(Table *table) {
  free(table->data);
}
