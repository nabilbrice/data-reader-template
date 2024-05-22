#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "driver_table_reader.h"

// dimensions (number of) labels in logR
#define DIM_FAST 56
// dimensions (number of) labels in logT
#define DIM_SLOW 22
// returns the floor index of the table in the collection
// assumes a particular distribution of labels in logT and logR
int hash_label(double logT, double logR) {
  int index_j, index_k;
  index_j = (logR + 7.4) / 0.2;  
  index_k = (logT - 4.9) / 0.1;
  return index_k * DIM_FAST + index_j;
}

// returns a pointer to a table in the collection
// j is index in R (log_R)
// k is index in temp (log_T K)
SizedTable *get_table(TableCollection *collection, int j, int k) {
  return &(collection->table[DIM_FAST * k + j]);
}

SizedTable *get_table_from_key(TableCollection *collection, double logT, double logR) {
  int table_index = hash_label(logT, logR);
  return &(collection->table[table_index]);
}

// returns a pointer to the row in a table collection
// i is index in energy (log_E eV)
// j is index in R (log_R)
// k is index in temp (log_T K)
TableRow *get_table_row(TableCollection *collection, int i, int j, int k) {
  SizedTable *table = get_table(collection, j, k);
  return &(table->row[i]);
}

TableRow *get_table_row_from_key(TableCollection *collection, int i, 
                                 double logT, double logR) {
  SizedTable *table = get_table_from_key(collection, logT, logR);
  return &(table->row[i]);
}

// loads a 2D table into the memory specified by index j and k
// from a file specified 
void load_tables(TableCollection *collection, FILE *file) {
  char buff[256];

  int i = 0;
  double logT, logR;
  // keeps reading rows in the file until reaching end of file (EOF)
  while (fgets(buff, sizeof(buff), file)) {
    // check if the line starts with the comment line char
    if (buff[0]=='#') {
      i = 0; // reset the row_label index since new table
      // some comment lines contain information about the table
      // this updates the array of log_Ts and log_Rs
      sscanf(buff, "%*s %lf %lf", &(logT), &(logR));
      continue;
    };

    TableRow *row = get_table_row_from_key(collection, i, logT, logR);
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
  load_tables(collection, file);
}

void initialise_tables_from_file(TableCollection *collection, char *filename) {
  FILE *file = fopen(filename, "r");
  initialise_table(collection, file);
  fclose(file);
}

void drop_tables(TableCollection *collection) {
  free(collection->table);
}

