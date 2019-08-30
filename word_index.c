/*
 * Create an index of all the words used in an input file
 * Sort them in alphabetical order
 * Next to each word, the lines where it appears are added, separated by commas
 *
 * Example:
 *         apple	1, 5
 *         banana	3, 5, 12
 *
 *	Resources:
 *		https://www.cprogramming.com/tutorial/c/lesson18.html
 *		https://www.thegeekstuff.com/2013/02/c-binary-tree
 */

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
	char * word;
	lines_t *lines;
	words_t *next_left;
	words_t *next_right;
};

struct lines {
	int line_number;
	lines_t *next_line;
};

words_t *tree_root = 0;

static void insert_word(words_t **leaf, char *new_word, int new_line)
{
	int result;

	if (*leaf == 0) {
		*leaf = (words_t *)malloc(sizeof(words_t));
		(*leaf)->word = new_word;
		(*leaf)->next_left = 0;
		(*leaf)->next_right = 0;
		(*leaf)->lines = (lines_t *)malloc(sizeof(lines_t));
		(*leaf)->lines->line_number = new_line;
	}
	else {
		result = strcmp(new_word, (*leaf)->word);

		if (result > 0) {
			insert_word(&(*leaf)->next_right, new_word, new_line);
		}
		else if (result < 0) {
			insert_word(&(*leaf)->next_left, new_word, new_line);
		}
		else {
			/* add line number only */
		}
	}
}

/*
 * Checks if the input character is alphabetical
 */
static bool isAlphabethical(char letter)
{
	if ((letter >= 'a') && (letter >= 'z') || (letter >= 'A') && (letter >= 'Z')) {
		return true;
	}

	return false;
}

/*
 * Inserts all the words found on one line
 */
static void line_handler(const char *line, const unsigned int line_number)
{
	unsigned int word_length = 0;
	unsigned int word_start_index = 0;
	char * p_word;
	char word[LINE_LENGTH_MAX]; /* plan the worst case: word's length is the line's length */

	for (int i = 0; i < LINE_LENGTH_MAX; i++) {
		if(line[i] == '\n') {
			/* exit on end of line */
			break;
		}
		else if( isAlphabethical(line[i]) ) {
			if (word_length == 0) {
				word_start_index = i;
			}
			word_length++;
		}
		else {
			p_word = (char *)malloc(word_length + 1);

			int j;
			for (j = 0; j < word_length; j++) {
				word[j] = line[word_start_index + j];
			}
			word[j] = '\0';

			strncpy(p_word, word, word_length+1);
			insert_word(&tree_root, p_word, line_number);
			word_length = 0;
		}
	}

	if (DEBUG) {
		printf("inserted %s\n", word);
	}
}

void parse_file(FILE *input)
{
	unsigned int line_number = 1;
	char one_line[LINE_LENGTH_MAX];

	while (fgets(one_line, LINE_LENGTH_MAX, input) != NULL)
	{
		if (DEBUG) {
			printf("line %d: %s\n", line_number, one_line);
		}

		line_handler(one_line, line_number);
		line_number++;
	}
}

/*
 * Displays the tree in alphabetical order
 */
void display_tree(words_t *tree)
{
	if (tree) {
		display_tree(tree->next_left);
		printf("%s ", tree->word);
		//printf("%d, ", tree->lines->line_number);
		printf("\n");
		display_tree(tree->next_right);
	}
}

void destroy_tree(words_t *leaf)
{
	if (leaf != 0) {
		destroy_tree(leaf->next_left);
		destroy_tree(leaf->next_right);
		free(leaf);
	}
}

void generate_tree(FILE *input_file)
{
    destroy_tree(tree_root);

    parse_file(input_file);

    display_tree(tree_root);

    // getchar();

    destroy_tree(tree_root);

    fclose(input_file);
}
