#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"

char *noms[] = {
    "BEGIN", "END", "READ", "WRITE", "ID", "INTLITERAL", "REALLITERAL",
    "LPAREN", "RPAREN", "SEMICOLON", "COMMA", "ASSIGNOP", "PLUSOP", "MINUSOP", "SCANEOF"
};

FILE *f;               
int  ligne = 1;
int  nb_erreurs = 0;

int  erreur_inst = 0;

char token_buffer[100];
int  len = 0;

typedef struct {
    int   ligne;
    token t;
    char  texte[100];
} token_lu;

void clear_buffer(void)  { len = 0; token_buffer[0] = '\0'; }
void buffer_char(int c)  { if (len < 99) { token_buffer[len++] = c; token_buffer[len] = '\0'; } }

token check_reserved(void)
{
    if (strcmp(token_buffer, "begin") == 0) return BEGIN;
    if (strcmp(token_buffer, "end")   == 0) return END;
    if (strcmp(token_buffer, "read")  == 0) return READ;
    if (strcmp(token_buffer, "write") == 0) return WRITE;
    return ID;
}

void sauter_instruction(void)
{
    int c;
    erreur_inst = 1;
    do {
        c = getc(f);
        if (c == '\n') ligne++;
    } while (c != ';' && c != EOF);
}

void lexical_error(int c)
{
    nb_erreurs++;
    printf("Erreur lexicale    ligne %d : caractere inconnu '%c'\n", ligne, c);
    sauter_instruction();
}

token scanner(void)
{
    int in_char, c;
    clear_buffer();

    while ((in_char = getc(f)) != EOF) {
        if (isspace(in_char)) {
            if (in_char == '\n') ligne++;
            continue;
        }
        else if (isalpha(in_char)) {
            buffer_char(in_char);
            for (c = getc(f); isalnum(c) || c == '_'; c = getc(f))
                buffer_char(c);
            ungetc(c, f);
            return check_reserved();
        }
        else if (isdigit(in_char)) {
            token t = INTLITERAL;
            buffer_char(in_char);
            for (c = getc(f); isdigit(c); c = getc(f))
                buffer_char(c);
            if (c == '.') {
                t = REALLITERAL;
                buffer_char(c);
                for (c = getc(f); isdigit(c); c = getc(f))
                    buffer_char(c);
            }
            if (c == 'e' || c == 'E') {
                t = REALLITERAL;
                buffer_char(c);
                c = getc(f);
                if (c == '+' || c == '-') {
                    buffer_char(c);
                    c = getc(f);
                }
                if (!isdigit(c)) {
                    nb_erreurs++;
                    printf("Erreur lexicale    ligne %d : exposant incomplet \"%s\"\n", ligne, token_buffer);
                    ungetc(c, f);
                    sauter_instruction();
                    clear_buffer();
                    continue;
                }
                for ( ; isdigit(c); c = getc(f))
                    buffer_char(c);
            }
            ungetc(c, f);
            return t;
        }
        else if (in_char == '(') { buffer_char(in_char); return LPAREN; }
        else if (in_char == ')') { buffer_char(in_char); return RPAREN; }
        else if (in_char == ';') { buffer_char(in_char); return SEMICOLON; }
        else if (in_char == ',') { buffer_char(in_char); return COMMA; }
        else if (in_char == '+') { buffer_char(in_char); return PLUSOP; }
        else if (in_char == ':') {
            c = getc(f);
            if (c == '=') { strcpy(token_buffer, ":="); return ASSIGNOP; }
            ungetc(c, f);
            lexical_error(in_char);
        }
        else if (in_char == '-') {
            c = getc(f);
            if (c == '-') {
                do in_char = getc(f);
                while (in_char != '\n' && in_char != EOF);
                if (in_char == '\n') ligne++;
            } else {
                ungetc(c, f);
                buffer_char('-');
                return MINUSOP;
            }
        }
        else
            lexical_error(in_char);
    }
    return SCANEOF;
}

int analyse_lexicale(char *source, char *sortie)
{
    FILE *fs;
    token t;
    token_lu inst[200];
    int n = 0, i;

    f = fopen(source, "r");
    if (f == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", source);
        return -1;
    }
    fs = fopen(sortie, "w");
    if (fs == NULL) {
        printf("Impossible de creer le fichier %s\n", sortie);
        fclose(f);
        return -1;
    }

    ligne = 1;
    nb_erreurs = 0;
    erreur_inst = 0;
    do {
        t = scanner();
        //printf("ligne %-3d %-12s %s\n", ligne, noms[t], token_buffer);
        if (erreur_inst) {
            n = 0;
            erreur_inst = 0;
        }
        inst[n].ligne = ligne;
        inst[n].t = t;
        strcpy(inst[n].texte, token_buffer);
        n++;
        if (t == SEMICOLON || t == BEGIN || t == END || t == SCANEOF || n == 200) {
            for (i = 0; i < n; i++)
                fprintf(fs, "%d %s %s\n", inst[i].ligne, noms[inst[i].t], inst[i].texte);
            n = 0;
        }
    } while (t != SCANEOF);

    fclose(fs);
    fclose(f);
    return nb_erreurs;
}
