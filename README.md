# Word Index
## Purpose
Creates an index of all the words used in an input text.
Sorts them in alphabetical order.
Next to each word, the lines where it appears are added, separated by commas.

Example:
- apple 1, 5,
- banana 3, 5, 12,
- cherry 4,

Generating the index of words makes use of a binary tree.
Adding line numbers makes use of linked lists.

To avoid doubles, for example "The" and "the", the program converts the input text to lowercase.
If a word appears several times on the same line, it records the line number once.

## How to use
The program requires a text file as an input. It only accepts ASCII. Each line should not exceed 512 characters.
It writes the index to an output file, which is an optional parameter (by default "output.txt").
