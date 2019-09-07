#include <stdio.h>
#include "word_index.h"

/******************************************************************************
 *
 * Main program.
 *
 *****************************************************************************/
int main(int argc, char *argv[])
{
    if (argc > 1) {
        char *in_file_path = argv[1];
        FILE *input_file = fopen(in_file_path, "r");
        FILE *output_file;

    	if (input_file != NULL) {
            if (argc > 2) {
                char *out_file_path = argv[2];
                output_file = fopen(out_file_path, "w");
            }
            else {
                output_file = fopen("output.txt", "w");
            }

            if(output_file != NULL) {
                generate_index(input_file, output_file);
            }
            fclose(output_file);
    	}
        fclose(input_file);
    }
    else {
        printf("Missing argument: text file\n");
    }

	return 0;
}
