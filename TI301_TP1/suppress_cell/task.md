## Chaînage en tête

### 👉 Objectifs :

> Écrire une fonction qui supprime une cellule comportant une vameur dans une liste chaînée.
>
>Utiliser des fonctions déjà disponibles pour initialiser et visualiser les listes
>
>Vérifier visuellement le résultat avec les fichiers `.svg` du dossier `output`

### 👀 Consignes
Écrivez le prototype et la définition de la fonction **removeFromStd**.

Cette fonction doit prendre en paramètre :
- un pointeur vers la liste chaînée
- une valeur entière à supprimer

Vous devrez éditer les fichiers `common/list.c` et `common/list.h`.

### 📝 Indications

- Pour utiliser les fonctions disponibles : consultez les fichiers `cell.h`, `list.h` et `getsamples.h`


### Sorties attendues

Pour une liste vide, supprimer une valeur laissera la liste vide, le résultat attendu est :

```
list [head @-]-->NULL
deleting value 15
list [head @-]-->NULL
```

Pour une liste initialisée par `SampleStdList_1()` :

- suppression de la valeur 2 (fin de liste)
- suppression de la valeur 33 (pas dans la liste - pas d'effet)
- suppression de la valeur 98 (milieu de liste)
- suppression de la valeur 12 (début de liste)

le résultat attendu est :
```
list [head @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->[ 2 | @-]-->NULL
deleting value 2
list [head @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->NULL
deleting value 33
list [head @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->NULL
deleting value 98
list [head @-]-->[ 12 | @-]-->[ -7 | @-]-->NULL
deleting value 12
list [head @-]-->[ -7 | @-]-->NULL