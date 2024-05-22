#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "driver_table_reader.h"

// returns a pointer to a table in the collection
// j is index in R (log_R)
// k is index in temp (log_T K)
SizedTable *get_table(TableCollection *collection, int j, int k) {
  return &(collection->table[DIM_FAST * k + j]);
}

// returns a pointer to the row in a table collection
// i is index in energy (log_E eV)
// j is index in R (log_R)
// k is index in temp (log_T K)
TableRow *get_table_row(TableCollection *collection, int i, int j, int k) {
  SizedTable *table = get_table(collection, j, k);
  return &(table->row[i]);
}

// loads a 2D table into the memory specified by index j and k
// from a file specified 
void load_table(TableCollection *collection, FILE *file, int j, int k) {
  char buff[1024];

  int i = 0;
  // condition to capture end of file and end of table
  while (fgets(buff, sizeof(buff), file)&&(i<DIM_ROWS)) {
    // check if the line starts with the comment line char
    if (buff[0]=='#') {
      // some comment lines contain information about the table
      // this updates the array of log_Ts and log_Rs
      sscanf(buff, "%*s %lf %lf", &(collection->log_Ts[k]), &(collection->log_Rs[j]));
      continue;
    };

    TableRow *row = get_table_row(collection, i, j, k);
    sscanf(buff, "%lf %lf %lf %lf %lf %lf %lf", 
      &(collection->row_labels[i]),  // row label is the first entry
      &row->entry[0], 
      &row->entry[1], 
      &row->entry[2], 
      &row->entry[3], 
      &row->entry[4], 
      &row->entry[5]);
    i++;
  };
  // printf("finished loading table %d\n", DIM_F * k + j + 1);
}

// requires manually calling free when done with the table
void initialise_table(TableCollection *collection, FILE *file) {
  // allocate the space required for the data
  int number_of_tables = DIM_FAST * DIM_SLOW;
  collection->table = malloc(number_of_tables * sizeof(SizedTable));
  if (collection->table == NULL) {
    printf("Error allocating for table data");
  };

  // read in all the 2D subtables
  for (int k=0; (k < DIM_SLOW); k++) {
    for (int j=0; (j < DIM_FAST); j++) {
      load_table(collection, file, j, k);
    };
  };
}

void initialise_tables_from_file(TableCollection *collection, char *filename) {
  FILE *file = fopen(filename, "r");
  initialise_table(collection, file);
  fclose(file);
}

void drop_tables(TableCollection *collection) {
  free(collection->table);
}
