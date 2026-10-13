#include <stdio.h>
#include "scanner.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    char *nom_fichier = "correct.txt";
    token t;
    int nb_erreurs_lex;

    if (argc > 1)
        nom_fichier = argv[1];

    f = fopen(nom_fichier, "r");
    if (f == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return 1;
    }

    /* analyse lexicale */
    printf("Analyse lexicale de %s : \n\n", nom_fichier);
    do {
        t = scanner();
        printf("ligne %-3d %-12s %s\n", ligne, noms[t], token_buffer);
    } while (t != SCANEOF);
    nb_erreurs_lex = nb_erreurs;
    printf("\n%d erreur(s) lexicale(s)\n\n", nb_erreurs_lex);

    /* analyse syntaxique */
    printf("Analyse syntaxique de %s : \n\n", nom_fichier);
    rewind(f);
    ligne = 1;
    afficher_lex = 0;
    system_goal();
    printf("\n%d erreur(s) syntaxique(s)\n\n", nb_erreurs - nb_erreurs_lex);

    if (nb_erreurs == 0)
        printf("Programme correct !\n");
    else
        printf("Total : %d erreur(s) trouvee(s)\n", nb_erreurs);

    fclose(f);
    return 0;
}
