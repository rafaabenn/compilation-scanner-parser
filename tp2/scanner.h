#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

typedef enum {
    BEGIN, END, READ, WRITE, ID, INTLITERAL, REALLITERAL,
    LPAREN, RPAREN, SEMICOLON, COMMA, ASSIGNOP, PLUSOP, MINUSOP, SCANEOF
} token;

extern char *noms[];             

extern FILE *f;                 
extern int  ligne;              
extern int  nb_erreurs;          
extern int  afficher_lex;        
extern char token_buffer[100];   

token scanner(void);

#endif
