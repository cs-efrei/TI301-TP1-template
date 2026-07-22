## Chaînage en tête

### 👉 Objectifs :

> Écrire une fonction qui ajoute une certaine valeur à une liste chaînée en tête de liste.
>
>Utiliser des fonctions déjà disponibles pour initialiser et visualiser les listes
>
>Vérifier visuellement le résultat avec les fichiers `.svg` du dossier `output`

### 👀 Consignes
Complétez le code de la fonction **addHeadStd**.

Cette fonction se trouve dans le fichier `common/list.c` et est déclarée dans le fichier `common/list.h`.

### 📝 Indications

- Pour utiliser les fonctions disponibles : consultez les fichiers `cell.h`, `list.h` et `getsamples.h`


### Sorties attendues

Pour une liste vide, avec ajout de la valeur 42, le résultat attendu est :

```
list [head @-]-->NULL
list [head @-]-->[ 42 | @-]-->NULL
```

Pour une liste initialisée par `SampleStdList_1()`, avec ajout successif des valeurs 42, 66 et -99, le résultat attendu est :

```
list [head @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->[ 2 | @-]-->NULL
list [head @-]-->[ 42 | @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->[ 2 | @-]-->NULL
list [head @-]-->[ 66 | @-]-->[ 42 | @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->[ 2 | @-]-->NULL
list [head @-]-->[ -99 | @-]-->[ 66 | @-]-->[ 42 | @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->[ 2 | @-]-->NULL
