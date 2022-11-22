#include "parser.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Parser {
  FILE *f;
  char current_line[MAX_PARSER_LINE_LENGTH + 1];
  uint32_t line_offset;
};

Parser *parser_create(FILE *f) {
  // allocating memory
  // null check
  // setting struct variables to 0, and new line
  Parser *p = (Parser *)calloc(1, sizeof(Parser));
  if (p) {
    p->f = f;
    p->current_line[0] = '\n';
    p->line_offset = 0;
  }
  return p;
}

void parser_delete(Parser **p) {
  // deleting parser
  // null check
  if (*p) {
    // set file to null
    (*p)->f = NULL;
  }
  // free the parser
  free(*p);
  // set to NULL
  *p = NULL;
}

bool valid_word(char word1) {
  if (isalnum(word1) != 0) {
    return true;
  }
  if ((word1) == '\'') {
    return true;
  }
  if ((word1) == '-') {
    return true;
  } else {
    return false;
  }
}

bool next_word(Parser *p, char *word) {
  // logic from oh
  uint32_t iter;
  char j;
  char word_p;
  char *results = "";
  int start = 0;
  // checking if it's a not valid word
  while (!valid_word(p->current_line[p->line_offset])) {
    // checks if we are at the end of a file
    if (p->current_line[p->line_offset] == '\0') {
      // getting the line
      results = fgets(p->current_line, MAX_PARSER_LINE_LENGTH + 1, p->f);
      start = 0;
      // starting at -1
      p->line_offset = -1;
    }
    // add one to go to next character
    p->line_offset += 1;
    // null check
    if (results == NULL) {
      return false;
    }
  }
  // create variable for the loop to go to the length of the line
  iter = MAX_PARSER_LINE_LENGTH - p->line_offset + 1;
  for (uint32_t i = p->line_offset; i < iter; i++) {
    // getting the word from current line
    word_p = p->current_line[i];
    // checking if its a character
    j = isalnum(word_p);
    if (j != 0) {
      // if its a character add to word, lowercased
      word[start] = tolower(word_p);
      // moving to the next character in the line
      start++;
    }
    if (j == 0) {
      // check if it's - or ' if it is then add to word
      if (word_p == '\'' || word_p == '-') {
        word[start] = word_p;
        // moving to the next character in the line
        start++;
      } else {
        // move to the next line if at end of line
        word[start] = '\0';
        // add one to line_offset
        p->line_offset += 1;
        // moving to the next character in the line
        start++;
        return true;
      }
    }
    p->line_offset += 1;
  }
  return true;
}
