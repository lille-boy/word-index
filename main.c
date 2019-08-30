#include <stdio.h>
#include "word_index.h"

int main(void)
{
    const char * filePath = "text.txt";

	FILE *input_file = fopen(filePath, "r");

	if (input_file != NULL)
	{
		generate_tree(input_file);
	}

	return 0;
}
