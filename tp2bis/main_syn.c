#include <stdio.h>
#include "parser.h"

int main(int argc, char *argv[])
{
    char *fichier_tokens = "tokens.txt";
    int nb_syn;

    if (argc > 1)
        fichier_tokens = argv[1];

    printf("Analyse syntaxique de %s\n\n", fichier_tokens);
    nb_syn = analyse_syntaxique(fichier_tokens);
    if (nb_syn < 0)
        return 1;

    if (nb_syn == 0)
        printf("Syntaxe correcte !\n");
    else
        printf("\n%d erreur(s) syntaxique(s)\n", nb_syn);
    return 0;
}
