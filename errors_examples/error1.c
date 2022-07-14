#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../inOrder.h"



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