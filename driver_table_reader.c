#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "driver_table_reader.h"

const int TABLE_SIZE = DIM_R * DIM_C * DIM_F * DIM_S;
// p is index for parameter (i.e. 0 = energy, 1 = sigma +, 2 = sigma -, etc
// i is index in energy (log_E eV)
// j is index in R (log_R)
// k is index in temp (log_T K)
// Together these pointers will get to the correct index for energy, density and temp provided
double *get_mut_table_value(Table *table, int p, int i, int j, int k) {
  // returns a pointer to a value in the table
  // this can be used to write (mutate) the value stored in the table
  return &table->data[table->stride_energy * i 
                    + table->stride_log_Rs * j 
                    + table->stride_log_Ts * k 
                    + p];
}

double *get_table_row(Table *table, int i, int j, int k) {
  // returns a pointer to the first value of the 7 numbers/parameters
  // to access each of the values, index in to this: row[p]
  return &(table->data[ table->stride_log_Ts * k 
                      + table->stride_log_Rs * j 
                      + table->stride_energy * i ]);
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
      sscanf(buff, "%*s %lf %lf", &(table->log_Ts[k]), &(table->log_Rs[j]));
      continue;
    };

    double *row = get_table_row(table, i, j, k);
    sscanf(buff, "%lf %lf %lf %lf %lf %lf %lf", 
      &(table->energies[i]),  // energies are always the first entry
      &row[0], &row[1], &row[2], &row[3], &row[4], &row[5]);
    i++;
  };
  // printf("finished loading table %d\n", DIM_F * k + j + 1);
}

// requires manually calling free when done with the table
void initialise_table(Table *table, FILE *file) {
  // allocate the space required for the data
  table->data = malloc(TABLE_SIZE * sizeof(table->data));
  if (table->data == NULL) {
    printf("Error allocating for table data");
  };

  // set stride lengths for the data array
  table->stride_energy = DIM_C;
  table->stride_log_Rs = DIM_R*DIM_C;
  table->stride_log_Ts = DIM_F*DIM_R*DIM_C;

  // read in all the 2D subtables
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

// Convert from density to R
// Value for input needs to be log10
double log_density_to_R(double logdensity, double logtemp) {
    return logdensity - 3 * (logtemp - 6);
}

// Function to return the index for the larger element of an ordered array above the input value
// Inputs: array (ordered), length, value; Return: larger element index
int closest_indices(double *array, int length, double value) {
    for (int idx = 0; idx < length; idx++) {
        if (array[idx] >= value){
            return idx;
        };
    };
    return -1;
}
// If return is 0 or -1, value is out of bounds


// Function to interpolate
// Inputs: data table, value for temp, density and energy, index of variable
// Outputs: One interpolated value for one variable
// How do I return these values
double interpolated_value(Table *boxed_table, double temp_value, double density_value, double energy_value, int variable_index) {
    
    double R_value = log_density_to_R(density_value, temp_value);
    
    int energy_index = closest_indices(boxed_table->energies, DIM_R, energy_value);
    
    int closest_index_temp = closest_indices(boxed_table->log_Ts, DIM_S, temp_value);
    int second_closest_temp = closest_index_temp - 1;
    int closest_index_R = closest_indices(boxed_table->log_Rs, DIM_F, R_value);
    int second_closest_R = closest_index_R - 1;
    
    if ((closest_index_temp >= 1) && (closest_index_R >= 1)){
        if (energy_value == boxed_table->energies[energy_index]){
            double temp_weight = fabs(temp_value - boxed_table->log_Ts[closest_index_temp])/(0.1);
            double R_weight = fabs(R_value - boxed_table->log_Rs[closest_index_R])/(0.2);
            
            double value1 = get_table_row(boxed_table, energy_index, closest_index_R, closest_index_temp)[variable_index];
            double value2 = get_table_row(boxed_table, energy_index, second_closest_R, closest_index_temp)[variable_index];
            double value3 = get_table_row(boxed_table, energy_index, closest_index_R, second_closest_temp)[variable_index];
            double value4 = get_table_row(boxed_table, energy_index, second_closest_R, second_closest_temp)[variable_index];
            //printf("temp_weight = %lf", temp_weight);
            
            double interpolated_value = (value1 * (1-temp_weight) * (1-R_weight)) + (value2 * (1-temp_weight) * (R_weight)) + (value3 * (temp_weight) * (1-R_weight)) + (value4 * (temp_weight) * (R_weight));
            
            
            return interpolated_value;
        }
        else{
            printf("Error: Incoreect energy value");
        }
    }
    else {
        printf("Error: Out of bounds value");
    }
    return 0;
}

