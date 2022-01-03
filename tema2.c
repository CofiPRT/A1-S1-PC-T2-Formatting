#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "accessories.h"
#include "support_functions.h"
#include "validation_functions.h"
#include "primary_functions.h"

#define MAXOPNUM 10
#define MAXOPCHAR 20

int find_operation(char operation[], char original[][MAXCHARS], int *lines)
{
    operation[0] = toupper(operation[0]); //make sure it's uppercase

	char operation_aux[20];
	//'operation' will used by strtok, we need a backup
	strcpy(operation_aux, operation);

	char *parameter; //divide operation into parameters
	parameter = strtok(operation, " ");

	switch (parameter[0])
	{
        //ONLY if something bad happens, exit with -1 (or -2 in wrap)
        int test;
		case 'W':
            test = wrap_text(operation_aux, original, lines);
			if (test == -1 || test == -2)
            {
                return test;
            }
            break;
		case 'C':
			if (center_text(operation_aux, original, lines) == -1)
            {
                return -1;
            }
            break;
		case 'L':
			if (align_left(operation_aux, original, lines) == -1)
            {
                return -1;
            }
            break;
		case 'R':
			if (align_right(operation_aux, original, lines) == -1)
            {
                return -1;
            }
            break;
		case 'J':
			if (justify(operation_aux, original, lines) == -1)
            {
                return -1;
            }
            break;
		case 'P':
			if (paragraph(operation_aux, original, lines) == -1)
            {
                return -1;
            }
            break;
		case 'I':
			if (list(operation_aux, original, lines) == -1)
            {
                return -1;
            }
            break;
		case 'O':
			if (ordered_list(operation_aux, original, lines) == -1)
            {
                return -1;
            }
            break;
		default:
            //if the operation is unrecognised, it's bad
			return -1;
	}
	return 0;
}

int apply(char command[], char original[][MAXCHARS], int *lines)
{
    int i;
    for (i = 0; i < *lines; i++)
    {
        trailing(original[i]); //removes trailing whitespaces
    }

	char operation_table[MAXOPNUM][MAXOPCHAR],	//saves all operations
		*operation_token;			//use with strtok

	//divide the command into operations
	operation_token = strtok(command, ",");

	int op_number; //keep track of the operations number
	for (op_number = 0; operation_token != NULL; op_number++)
	{
		if (strlen(operation_token) > MAXOPCHAR)
		{
            //operation is longer than 20 characters
			printf("Invalid operation!\n");
			return -1;
		}

		if (op_number == MAXOPNUM)
		{
            //more than 10 operations already read
			printf("Too many operations! "
				"Only the first 10 will be applied.\n");
			break;
		}
		//save each operation on a line
		strcpy(operation_table[op_number], operation_token);

		operation_token = strtok(NULL, ",");
	}

    int test, j;
	for (i = 0; i < op_number; i++)
	{
        test = find_operation(operation_table[i], original, lines);
		if (test == -1)
		{
			//not a valid operation
			printf("Invalid operation!\n");
			return -1;
		}
        if (test == -2)
        {
            return -2;
        }

        for (j = 0; j < *lines; j++)
        {
            trailing(original[j]); //removes trailing whitespaces
        }
	}

	return 0;
}

int main(int argc, char *argv[]) {
  	char buf[MAXCHARS],              // buffer used for reading from the file
      	original[MAXCHARS][MAXCHARS],// original text, line by line
    	result[MAXCHARS][MAXCHARS];  // text obtained after applying operations
  	int original_line_count = 0,     // number of lines in the input file
      	result_line_count = 0,       // number of lines in the output file
    	i;

  	if (argc != 4) {  // invalid number of arguments
    	fprintf(stderr,
            "Usage: %s operations_string input_file output_file\n",
            argv[0]);
    	return -1;
  	}

  	// Open input file for reading
  	FILE *input_file = fopen(argv[2], "r");

  	if (input_file == NULL) {
    	fprintf(stderr, "File \"%s\" not found\n", argv[2]);
  	  return -2;
  	}

  	// Read data in file line by line
  	while (fgets(buf, MAXCHARS, input_file)) {
    	strcpy(original[original_line_count], buf);
    	original_line_count++;
  	}

  	fclose(input_file);

  	char backup[MAXCHARS][MAXCHARS]; //in case of invalid operations
  	cp_text(backup, original, original_line_count);

    int lines = original_line_count; //save the 2nd variable as backup

    int test = apply(argv[1], original, &lines);
 	if (test == -1 || test == -2)
 	{
 		//if apply exits with -1 or -2, something bad happened, use backup
 		cp_text(result, backup, original_line_count);
 		result_line_count = original_line_count;
 	}
 	else
 	{
 		cp_text(result, original, lines);
 		result_line_count = lines;
 	}

  	// Open output file for writing
  	FILE *output_file = fopen(argv[3], "w");

  	if (output_file == NULL) {
    	fprintf(stderr, "Could not open or create file \"%s\"\n", argv[3]);
    	return -2;
  	}

  	// Write result in output file
  	for (i = 0; i < result_line_count; i++) {
    	fputs(result[i], output_file);
  	}

  	fclose(output_file);

    return 0;
}