#include <stdio.h>
#include "scanner.h"

int main(int argc, char *argv[])
{
    char *nom_fichier = "test.txt";
    int nb_lex;

    if (argc > 1)
        nom_fichier = argv[1];

    printf("Analyse lexicale de %s\n\n", nom_fichier);
    nb_lex = analyse_lexicale(nom_fichier, "tokens.txt");
    if (nb_lex < 0)
        return 1;

    printf("\n%d erreur(s) lexicale(s)\n", nb_lex);
    printf("Tokens ecrits dans tokens.txt\n");
    return 0;
}
