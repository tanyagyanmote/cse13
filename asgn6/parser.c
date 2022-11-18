#include <stdio.h>
#include "parser.h"
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

struct Parser {
    FILE *f;
    char current_line[MAX_PARSER_LINE_LENGTH + 1];
    uint32_t line_offset;
};

Parser *parser_create(FILE *f){
    Parser *p = (Parser*)calloc(1,sizeof(Parser));
    if(p){
        p -> f = f;
        p -> current_line[0] = '\n';
        p -> line_offset = 0;
    }
    return p;
}

void parser_delete(Parser **p){
    if(*p){
        (*p) -> f = NULL;
    }
    free(*p);
    *p = NULL;
}


bool valid_word(char word1){
    if(isalnum(word1) != 0){
        return true;
    }
    if(isalnum(word1) == '\''){
        return true;
    }
    if(isalnum(word1) == '-'){
        return true;
    }
    else{
        return false;
    }
}


bool next_word(Parser *p, char *word){
    //logic from tutor office hours
    uint32_t iterations;
    char j;
    char word_p;
    char *results = "";
    int start = 0;

    while (!valid_word(p->current_line[p->line_offset])){
        if (p->current_line[p->line_offset] == '\0'){
            results = fgets(p->current_line,MAX_PARSER_LINE_LENGTH+1,p->f); 
            start = 0;
            p->line_offset = -1;
        }
        p->line_offset += 1;
        if (results == NULL){
            word[0] = '\0';
            word = NULL;
            return false;
        }
    }

    iterations = MAX_PARSER_LINE_LENGTH - p->line_offset + 1;
    for(uint32_t i =p->line_offset; i < iterations; i++){
        word_p = p->current_line[i];
        j = isalnum(word_p);
        if(j!=0){
            word[start] = tolower(word_p);
            start ++;
        }
        if(j==0){
            if(word_p == '\''){
                word[start] = word_p;
                start ++;
            }
            if(word_p == '-'){
                word[start] = word_p;
                start ++;
            }
            else{
                word[start] = '\0';
                p->line_offset += 1;
                start ++;
                return true;
            }
        }
        p->line_offset += 1;
    }
    return true;

}
