# Compilation : analyseur lexical et syntaxique de MICRO

## tp2
Analyse lexicale (scanner) + analyse syntaxique (parser par descente recursive).
Le fichier source est lu deux fois : une fois pour les erreurs lexicales, une fois par le parser.

```
gcc main.c scanner.c parser.c -o main      # les deux analyses
gcc main_lex.c scanner.c -o main_lex       # analyse lexicale seule
./main test.txt
```

## tp2bis
Meme logique, avec un fichier intermediaire : l'analyse lexicale ecrit les tokens dans `tokens.txt`,
que l'analyseur syntaxique lit ensuite. En cas d'erreur lexicale, l'instruction est sautee jusqu'au `;`.
Les reels avec exposant (`1.5E3`, `2e-4`) sont reconnus.

```
gcc main.c scanner.c parser.c -o main            # les deux analyses
gcc main_lex.c scanner.c -o main_lex             # lexicale seule  -> tokens.txt
gcc main_syn.c parser.c scanner.c -o main_syn    # syntaxique seule <- tokens.txt
./main erreurs.txt
```

Fichiers de test : `correct.txt`, `test.txt` (erreurs lexicales), `erreurs.txt` (erreurs syntaxiques).
