#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"

FILE *ft;           
token current_token; 
int  nb_erreurs_syn = 0;

token lire_token(void)
{
    char texte_ligne[200], nom[20];
    int i;

    if (fgets(texte_ligne, sizeof texte_ligne, ft) == NULL)
        return SCANEOF;                      

    token_buffer[0] = '\0';                   
    if (sscanf(texte_ligne, "%d %19s %99s", &ligne, nom, token_buffer) < 2)
        return SCANEOF;                       

    for (i = BEGIN; i <= SCANEOF; i++)       
        if (strcmp(nom, noms[i]) == 0)
            return (token) i;
    return SCANEOF;
}

token next_token(void) { return current_token; }

void syntax_error(token t)
{
    nb_erreurs_syn++;
    printf("Erreur syntaxique  ligne %d : token inattendu %s \"%s\"\n",
           ligne, noms[t], token_buffer);
}

void match(token t)
{
    if (current_token == t)
        current_token = lire_token();
    else {
        nb_erreurs_syn++;
        printf("Erreur syntaxique  ligne %d : on attendait %s mais on a trouve %s \"%s\"\n",
               ligne, noms[t], noms[current_token], token_buffer);
    }
}

void program(void);
void inst_list(void);
void inst(void);
void id_list(void);
void expr_list(void);
void expression(void);
void add_op(void);
void prim(void);

/* <system_goal> ::= <program> SCANEOF */
void system_goal(void)
{
    current_token = lire_token();   
    program();
    match(SCANEOF);
}

/* <program> ::= BEGIN <inst_list> END */
void program(void)
{
    match(BEGIN);
    inst_list();
    match(END);
}

/* <inst_list> ::= <inst> { <inst> } */
void inst_list(void)
{
    inst();
    while (true){
        switch (next_token())
        {
        case ID :
        case READ :
        case WRITE :
            inst();
            break;
        case END :
        case SCANEOF :
            return;
        default :
            current_token = lire_token();
        }
    }
}

/* <inst> ::= ID := <expr> ;  |  READ ( <id_list> ) ;  |  WRITE ( <expr_list> ) ; */
void inst(void)
{
    switch (next_token()) {
    case ID:
        match(ID); match(ASSIGNOP); expression(); match(SEMICOLON);
        break;
    case READ:
        match(READ); match(LPAREN); id_list(); match(RPAREN); match(SEMICOLON);
        break;
    case WRITE:
        match(WRITE); match(LPAREN); expr_list(); match(RPAREN); match(SEMICOLON);
        break;
    default:
        syntax_error(next_token());
    }
}

/* <id_list> ::= ID { , ID } */
void id_list(void)
{
    match(ID);
    while (next_token() == COMMA) {
        match(COMMA);
        match(ID);
    }
}

/* <expr_list> ::= <expr> { , <expr> } */
void expr_list(void)
{
    expression();
    while (next_token() == COMMA) {
        match(COMMA);
        expression();
    }
}

/* <expr> ::= <prim> { <add_op> <prim> } */
void expression(void)
{
    prim();
    while (next_token() == PLUSOP || next_token() == MINUSOP) {
        add_op();
        prim();
    }
}

/* <add_op> ::= PLUSOP | MINUSOP */
void add_op(void)
{
    if (next_token() == PLUSOP || next_token() == MINUSOP)
        match(next_token());
    else
        syntax_error(next_token());
}

/* <prim> ::= ( <expr> ) | ID | INTLITERAL | REALLITERAL */
void prim(void)
{
    switch (next_token()) {
    case LPAREN:
        match(LPAREN); expression(); match(RPAREN);
        break;
    case ID:
        match(ID);
        break;
    case INTLITERAL:
        match(INTLITERAL);
        break;
    case REALLITERAL:
        match(REALLITERAL);
        break;
    default:
        syntax_error(next_token());
    }
}

int analyse_syntaxique(char *fichier_tokens)
{
    ft = fopen(fichier_tokens, "r");
    if (ft == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", fichier_tokens);
        return -1;
    }

    nb_erreurs_syn = 0;
    system_goal();

    fclose(ft);
    return nb_erreurs_syn;
}
