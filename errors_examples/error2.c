#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../inOrder.h"



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