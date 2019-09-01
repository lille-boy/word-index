#include <stdio.h>
#include "word_index.h"

int main(int argc, char *argv[])
{
    if (argc > 1) {
        char * filePath = argv[1];

        FILE *input_file = fopen(filePath, "r");

    	if (input_file != NULL)
    	{
    		generate_tree(input_file);
    	}
        
        fclose(input_file);
    }
    else {
        printf("Missing argument: text file\n");
    }

	return 0;
}
