# Exemple d'exercice en C avec correction automatique

## Enoncé


Vous devez écrire une fonction réalisant le parcours infixe d'un arbre binaire donné par la structure suivante:

```c
typedef struct tree{
    int value;              // the value at root of this tree
    int number_elements;    // number of elements in the tree bellow this root (including self)  
    struct tree *left;      // left sub-tree
    struct tree *right;     // right sub-tree
}tree_t;

/**
 *              1
 *             / \
 *            0   2
 *           /
 *         -1
 * 
 * => tree(1).number_elements = 4 because it has 3 childs + himself
 */
```

```c
/**
 * @brief Travel in a in-order tour the binary tree
 * 
 * @pre tree : the binary tree to travel
 * @post int* : an array containing the list of integers according to the in-order tour;
 *                NULL if the tree is NULL, or if an error occurs
 */
int *in_order(tree_t *tree);
```

Exemples d'utilisation

```
/**
*   t = 
*              1
*             / \
*            0   2
*/
- in_order(t) retourne [0, 1, 2]

/**
*   t = 
*              1
*             / \
*            0   2
*           /   / 
*/         15  -1   
- in_order(t) retourne [15, 0, 1, -1, 2]

/**
*   t = 
*               15
*             /    \
*            0      22
*           / \    /
*         -3   1  20
*                  \
*                   21
*/
- in_order(t) retourne [-3, 0, 1, 15, 20, 21, 22]
```


## Solution correcte
-----------------

```c
int *in_order(tree_t *tree){
    if (tree == NULL) return NULL;
    int *ret = malloc(tree->number_elements * sizeof(int));
    if (ret == NULL) return NULL;
    int index = 0;                          // number of elements already copied to the response array
    if (tree->left != NULL){
        int *temp = in_order(tree->left);  // result for left sub-tree
        if (temp == NULL) return NULL;      // error in malloc
        memcpy(ret, temp, tree->left->number_elements * sizeof(int));
        free(temp);
        index += tree->left->number_elements;
    }
    *(ret+index) = tree->value;
    index++;
    if (tree->right != NULL){
        int *temp = in_order(tree->right); // result for right sub-tree
        if (temp == NULL) return NULL;      // error in malloc
        memcpy(ret+index, temp, tree->right->number_elements * sizeof(int));
        free(temp);
    }
    return ret;
}
```

## Description de la suite de tests
--------------------------------

Pour la suite de test, j'utilise les propriétés des arbres binaires de recherche afin de créer des arbres facilement (toutes les clés < valeur se trouvent à gauche de la racine et les clés >= valeur se trouvent à droite de celle-ci)

Je pense à vérifier également les cas limites (arbres qui ne sont qu'une seule branche ou ayant qu'un seul enfant).

J'utilise également un test aléatoire afin de mieux garantir l'exactitude de la fonction. Je stocke dans un tableau le nombre de fois que la valeur est censée apparaître et je décrémente celle-ci afin de vérifier que chaque valeur apparait le bon nombre de fois. Je vérifie ensuite qu'elles sortent dans le bon ordre grâce à la propriété de parcours in-order des arbres binaires de recherche, retournant un tableau trié en ordre croissant.

Par la suite, j'utilise les fonction malloc_hook et free_hook pour compter le nombre de malloc et de free appelé par le code de l'étudiant lors de son exécution. Je vérifie ensuite grâce à un assert que le nombre de malloc est bien égal au nombre de free + 1 (il faut un malloc non free pour retourner le tableau d'int tel que demandé dans la spécification). Nous vérifions ainsi qu'il n'y a pas de memory leak causé par le code de l'étudiant. Ceci génère malheureusement des alertes de compilation de par le fait que malloc_hook et free_hook sont obsolètes.

Enfin, j'utilise à nouveau la fonction malloc_hook afin de forcer un malloc à retourner NULL, pour vérifier que l'étudiant traite également le cas limite lorsque le malloc échoue, et qu'il doit donc renvoyer NULL.

## Exécution de la suite de tests avec la solution correcte
--------------------------------------------------------

```


     CUnit - A unit testing framework for C - Version 2.1-3
     http://cunit.sourceforge.net/


Suite: Suite_1
  Test: test in-order simple balanced binary tree ...passed
  Test: test in-order linear binary tree (only right child) ...passed
  Test: test in-order linear binary tree (only left child) ...passed
  Test: test in-order binary tree (with missing right/left children) ...passed
  Test: test in-order binary tree (random tree) ...passed
  Test: test in-order binary tree with NULL tree ...passed
  Test: test in-order binary tree with failed malloc ...passed

Run Summary:    Type  Total    Ran Passed Failed Inactive
              suites      1      1    n/a      0        0
               tests      7      7      7      0        0
             asserts   2024   2024   2024      0      n/a

Elapsed time =    0.016 seconds
```


## Erreurs détectées par la suite de test
--------------------------------------

Pour les exemples, j'ai désactivé le test sur un arbre aléatoire car celui-ci génère beaucoup de messages d'erreurs, troublant la visibilité des autres tests. Néanmoins, il est tout à fait possible de faire tourner ce tests pour les codes erronés, et il renverra un message d'erreur signalant que le code est incorrect.

### Exemple 1:

Code qui ne traite pas le fait qu'un malloc puisse échouer ou ne traitant pas le fait que l'arbre soit NULL

```c
int *in_order(tree_t *tree){
    int *ret = malloc(tree->number_elements * sizeof(int));
    int index = 0;                          // number of elements already copied to the response array
    if (tree->left != NULL){
        int *temp = in_order(tree->left);  // result for left sub-tree
        memcpy(ret, temp, tree->left->number_elements * sizeof(int));
        free(temp);
        index += tree->left->number_elements;
    }
    *(ret+index) = tree->value;
    index++;
    if (tree->right != NULL){
        int *temp = in_order(tree->right); // result for right sub-tree
        memcpy(ret+index, temp, tree->right->number_elements * sizeof(int));
        free(temp);
    }
    return ret;
}
```


```
     CUnit - A unit testing framework for C - Version 2.1-3
     http://cunit.sourceforge.net/


Suite: Suite_1
  Test: test in-order simple balanced binary tree ...passed
  Test: test in-order linear binary tree (only right child) ...passed
  Test: test in-order linear binary tree (only left child) ...passed
  Test: test in-order binary tree (with missing right/left children) ...passed
  Test: test in-order binary tree with failed malloc ...Segmentation fault (core dumped)
```


### Exemple 2:

Code qui oublie de libérer la mémoire temporaire:


```c
int *in_order(tree_t *tree){
    if (tree == NULL) return NULL;
    int *ret = malloc(tree->number_elements * sizeof(int));
    if (ret == NULL) return NULL;
    int index = 0;                          // number of elements already copied to the response array
    if (tree->left != NULL){
        int *temp = in_order(tree->left);  // result for left sub-tree
        if (temp == NULL) return NULL;      // error in malloc
        memcpy(ret, temp, tree->left->number_elements * sizeof(int));
        index += tree->left->number_elements;
    }
    *(ret+index) = tree->value;
    index++;
    if (tree->right != NULL){
        int *temp = in_order(tree->right); // result for right sub-tree
        if (temp == NULL) return NULL;      // error in malloc
        memcpy(ret+index, temp, tree->right->number_elements * sizeof(int));
    }
    return ret;
}
```
```

     CUnit - A unit testing framework for C - Version 2.1-3
     http://cunit.sourceforge.net/


Suite: Suite_1
  Test: test in-order simple balanced binary tree ...passed
  Test: test in-order linear binary tree (only right child) ...passed
  Test: test in-order linear binary tree (only left child) ...passed
  Test: test in-order binary tree (with missing right/left children) ...passed
  Test: test in-order binary tree (random tree) ...passed
  Test: test in-order binary tree with failed malloc ...Segmentation fault (core dumped)
```

### Exemple 3:

Code qui écrase les modifications du parcours du sous-arbre de gauche

```c
int *in_order(tree_t *tree){
    if (tree == NULL) return NULL;
    int *ret = malloc(tree->number_elements * sizeof(int));
    if (ret == NULL) return NULL;
    int index = 0;                          // number of elements already copied to the response array
    if (tree->left != NULL){
        int *temp = in_order(tree->left);  // result for left sub-tree
        if (temp == NULL) return NULL;      // error in malloc
        memcpy(ret, temp, tree->left->number_elements * sizeof(int));
        free(temp);
    }
    *(ret+index) = tree->value;
    index++;
    if (tree->right != NULL){
        int *temp = in_order(tree->right); // result for right sub-tree
        if (temp == NULL) return NULL;      // error in malloc
        memcpy(ret+index, temp, tree->right->number_elements * sizeof(int));
        free(temp);
    }
    return ret;
}
```

```

     CUnit - A unit testing framework for C - Version 2.1-3
     http://cunit.sourceforge.net/


Suite: Suite_1
  Test: test in-order simple balanced binary tree ...FAILED
    1. tests.c:157  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
    2. tests.c:157  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
    3. tests.c:157  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
  Test: test in-order linear binary tree (only right child) ...passed
  Test: test in-order linear binary tree (only left child) ...FAILED
    1. tests.c:222  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
    2. tests.c:222  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
    3. tests.c:222  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
  Test: test in-order binary tree (with missing right/left children) ...FAILED
    1. tests.c:257  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
    2. tests.c:257  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
    3. tests.c:257  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
    4. tests.c:257  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
    5. tests.c:257  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
    6. tests.c:257  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
    7. tests.c:257  - CU_ASSERT_EQUAL(*(ret+i),expected[i])
  Test: test in-order binary tree with NULL tree ...passed
  Test: test in-order binary tree with failed malloc ...passed

Run Summary:    Type  Total    Ran Passed Failed Inactive
              suites      1      1    n/a      0        0
               tests      6      6      3      3        0
             asserts     24     24     11     13      n/a

Elapsed time =    0.016 seconds
```
