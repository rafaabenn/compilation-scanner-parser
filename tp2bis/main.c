#include <stdio.h>
#include "scanner.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    char *nom_fichier = "correct.txt";
    char *fichier_tokens = "tokens.txt";
    int nb_lex, nb_syn;

    if (argc > 1)
        nom_fichier = argv[1];

    /* analyse lexicale */
    printf("Analyse lexicale de %s : \n\n", nom_fichier);
    nb_lex = analyse_lexicale(nom_fichier, fichier_tokens);
    if (nb_lex < 0)
        return 1;
    printf("\n%d erreur(s) lexicale(s)\n", nb_lex);
    printf("Tokens ecrits dans %s\n\n", fichier_tokens);

    /* analyse syntaxique */
    printf("Analyse syntaxique de %s : \n\n", fichier_tokens);
    nb_syn = analyse_syntaxique(fichier_tokens);
    if (nb_syn < 0)
        return 1;
    printf("\n%d erreur(s) syntaxique(s)\n\n", nb_syn);

    if (nb_lex + nb_syn == 0)
        printf("Programme correct !\n");
    else
        printf("Total : %d erreur(s) trouvee(s)\n", nb_lex + nb_syn);

    return 0;
}
