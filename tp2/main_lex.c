#include <stdio.h>
#include "scanner.h"

int main(int argc, char *argv[])
{
    char *nom_fichier = "test.txt";
    token t;

    if (argc > 1)
        nom_fichier = argv[1];

    f = fopen(nom_fichier, "r");
    if (f == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return 1;
    }

    printf("Analyse lexicale de %s\n\n", nom_fichier);
    do {
        t = scanner();
        printf("ligne %-3d %-12s %s\n", ligne, noms[t], token_buffer);
    } while (t != SCANEOF);

    printf("\n%d erreur(s) lexicale(s)\n", nb_erreurs);
    fclose(f);
    return 0;
}
