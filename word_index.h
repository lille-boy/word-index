#define DEBUG   0

/******************************************************************************
 * Input: file to create an index from, in -.TXT format
 * Output: file where the index of words is written. The path to the output
 *         file is optional. If not provided, the index is written to
 *         "output.txt"
 *****************************************************************************/
void generate_index(FILE *input_file, FILE * output_file);
