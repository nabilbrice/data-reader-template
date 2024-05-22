#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "driver_table_reader.h"

// size of char buffer for reading in lines
#define BUFFSIZE 256
//
// compile-time information to configure the hashing function
//
// logR labels look like:
// [-7.4, -7.2, -7.0, ...]
#define LOGR0 (-7.4)
#define LOGR_STEP 0.2
// dimensions (number of) labels in logR
#define DIM_FAST 56
// logT labels look like:
// [4.9, 5.0, 5.1, ...]
#define LOGT0 (4.9)
#define LOGT_STEP 0.1
// dimensions (number of) labels in logT
#define DIM_SLOW 22

// returns the floor index of the table in the collection
// assumes a particular distribution of labels in logT and logR
int hash_label(double logT, double logR) {
  int index_j = (logR - LOGR0) / LOGR_STEP;  
  int index_k = (logT - LOGT0) / LOGT_STEP;
  return index_k * DIM_FAST + index_j;
}

// returns a pointer to a table in the collection
// j is index in R (log_R)
// k is index in temp (log_T K)
SizedTable *get_table_from_index(TableCollection *collection, int j, int k) {
  return &(collection->table[DIM_FAST * k + j]);
}

SizedTable *get_table(TableCollection *collection, double logT, double logR) {
  int table_index = hash_label(logT, logR);
  return &(collection->table[table_index]);
}

// returns a pointer to the row in a table collection
// i is index in energy (log_E eV)
// j is index in R (log_R)
// k is index in temp (log_T K)
TableRow *get_table_row_from_index(TableCollection *collection, int i, int j, int k) {
  SizedTable *table = get_table_from_index(collection, j, k);
  return &(table->row[i]);
}

TableRow *get_table_row(TableCollection *collection, int i, 
                                 double logT, double logR) {
  SizedTable *table = get_table(collection, logT, logR);
  return &(table->row[i]);
}

// loads a 2D table into the memory specified by index j and k
// from a file specified 
void load_tables(TableCollection *collection, FILE *file) {
  char buff[BUFFSIZE];

  int i = 0;
  double logT, logR;
  // keeps reading rows in the file until reaching end of file (EOF)
  while (fgets(buff, sizeof(buff), file)) {
    // check if the line starts with the comment line char
    if (buff[0]=='#') {
      i = 0; // reset the row_label index since new table
      // some comment lines contain table labels
      sscanf(buff, "%*s %lf %lf", &(logT), &(logR));
      continue;
    };

    TableRow *row = get_table_row(collection, i, logT, logR);
    double *entry = row->entry;
    sscanf(buff, "%lf %lf %lf %lf %lf %lf %lf", 
      &(collection->row_labels[i]),  // row label is the first entry
      &entry[0], &entry[1], &entry[2], &entry[3], &entry[4], &entry[5]);
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

