/**************************************************************************************************
 * Create an index of all the words used in an input file
 * Sort them in alphabetical order
 * Next to each word, the lines where it appears are added, separated by commas
 *
 * Example:
 *         apple 1, 5,
 *         banana 3, 5, 12,
 *         cherry 4,
 *
 *	Resources:
 *		https://www.cprogramming.com/tutorial/c/lesson18.html
 *		https://www.thegeekstuff.com/2013/02/c-binary-tree
 *************************************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "word_index.h"

#define LINE_LENGTH_MAX 512u

typedef struct words words_t;
typedef struct lines lines_t;

struct words {
	char *word;
	lines_t *lines;
	words_t *next_left;
	words_t *next_right;
};

struct lines {
	int line_number;
	lines_t *next_line;
};

words_t *tree_root = 0;

/******************************************************************************
 * Adds a line number to the linked list
 *****************************************************************************/
static void add_line_number(lines_t *head, int line_number)
{
	lines_t *current = head;

	while (current->line_number != line_number) { /* Don't add the same line number twice */
		if (current->next_line == NULL) {
			current->next_line = (lines_t *)malloc(sizeof(lines_t));
			current->next_line->line_number = line_number;
			current->next_line->next_line = NULL;
			return;
		}
		else {
			current = current->next_line;
		}
	}
}

/******************************************************************************
 * Inserts a word into the binary tree, together with the line where it appears
 *****************************************************************************/
static void insert_word(words_t **leaf, char *new_word, int new_line)
{
	if (*leaf == 0) {
		*leaf = (words_t *)malloc(sizeof(words_t));
		(*leaf)->word = new_word;
		(*leaf)->next_left = 0;
		(*leaf)->next_right = 0;
		(*leaf)->lines = (lines_t *)malloc(sizeof(lines_t));
		(*leaf)->lines->line_number = new_line;
	}
	else {
		int result = strcmp(new_word, (*leaf)->word);

		if (result > 0) {
			insert_word(&(*leaf)->next_right, new_word, new_line);
		}
		else if (result < 0) {
			insert_word(&(*leaf)->next_left, new_word, new_line);
		}
		else {
			/* word is already in the index, add line number only */
			add_line_number((*leaf)->lines, new_line);
		}
	}
}

/******************************************************************************
 * Checks if the input character is alphabetical
 *****************************************************************************/
static bool is_alphabetical(char letter)
{
	if ( (letter >= 'a') && (letter <= 'z') ) {
		return true;
	}

	return false;
}

/******************************************************************************
 * Checks if the input character is a separator or punctuation
 *****************************************************************************/
static bool is_separator(char letter)
{
	if ( (letter == ' ') || (letter == '.') ||
         (letter == ',') || (letter == ';') ||
		 (letter == '-') || (letter == ':') ||
		 (letter == '(') || (letter == ')') ||
	 	(letter == '\n') ) {
		return true;
	}

	return false;
}

/******************************************************************************
 * Gets one line as an input
 * Every word found is inserted into the binary tree
 *****************************************************************************/
static void line_handler(const char *line, const unsigned int line_number)
{
	unsigned int word_length = 0;
	unsigned int word_start_index = 0;
	char * p_word;
	char word[LINE_LENGTH_MAX]; /* plan the worst case: word's length is the line's length */

	for (int i = 0; i < LINE_LENGTH_MAX; i++) {
		if ( is_alphabetical(line[i]) ) {
			if (word_length == 0) {
				word_start_index = i;
			}
			word_length++;
		}
		else if ( is_separator(line[i]) ) {
			if (word_length > 0) {
				p_word = (char *)malloc(word_length + 1);

				int j;
				for (j = 0; j < word_length; j++) {
					word[j] = line[word_start_index + j];
				}
				word[j] = '\0';

				strncpy(p_word, word, word_length+1);
				insert_word(&tree_root, p_word, line_number);
				word_length = 0;

				if (DEBUG) {
					printf("inserted %s\n", word);
				}
			}
		}

		if (line[i] == '\n') {
			/* exit on end of line */
			break;
		}
	}
}

/******************************************************************************
 * Takes a line and converts all letters to lower lowercase
 * This is to avoid doubles
 *****************************************************************************/
static void lower_string(char *line)
{
	unsigned int i = 0;
	unsigned int offset = 'a' - 'A';

	while (line[i] != '\0') {
	  if (line[i] >= 'A' && line[i] <= 'Z') {
	     line[i] = line[i] + offset;
	  }
	  i++;
	}
}

/******************************************************************************
 * Parses the input file line by line
 *****************************************************************************/
static void parse_file(FILE *input)
{
	unsigned int line_number = 1;
	char one_line[LINE_LENGTH_MAX];

	while (fgets(one_line, LINE_LENGTH_MAX, input) != NULL)
	{
		if (DEBUG) {
			printf("line %d: %s", line_number, one_line);
		}

		lower_string(one_line);	/* convert to lowercase to avoid doubles */
		line_handler(one_line, line_number);
		line_number++;
	}
}

/******************************************************************************
 * Writes the lines where a word appears to the output file
 *****************************************************************************/
static void write_lines_to_file(lines_t *line, FILE *output_file)
{
	char number[4]; /* up to 9999 lines */

	while (line != NULL) {
		sprintf(number, "%d, ", line->line_number);
		fputs(number, output_file);
		line = line->next_line;
	}
}

/******************************************************************************
 * Write the sorted binary tree to a file
 * The output is the list of words in alphabetical order
 *****************************************************************************/
static void write_tree_to_file(words_t *tree, FILE *output_file)
{
	if (tree) {
		write_tree_to_file(tree->next_left, output_file);
		fputs(tree->word, output_file);
		fputs(" ", output_file);
		write_lines_to_file(tree->lines, output_file);
		fputs("\n", output_file);
		write_tree_to_file(tree->next_right, output_file);
	}
}

/******************************************************************************
 * Destroys the binary tree (frees memory)
 *****************************************************************************/
static void destroy_tree(words_t *leaf)
{
	if (leaf != 0) {
		destroy_tree(leaf->next_left);
		destroy_tree(leaf->next_right);
		free(leaf);
	}
}

/******************************************************************************
 * Parses the input file and generates the index
 *****************************************************************************/
void generate_index(FILE *input_file, FILE * output_file)
{
    destroy_tree(tree_root);

    parse_file(input_file);

	if (input_file != NULL) {
		write_tree_to_file(tree_root, output_file);
	}

    destroy_tree(tree_root);
}
