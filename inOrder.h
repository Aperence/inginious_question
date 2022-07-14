#ifndef PREORDER_H

#define PREORDER_H

#include <stdlib.h>
#include <string.h>

typedef struct tree{
    int value;              // the value at root of this tree
    int number_elements;    // number of elements in the tree bellow this root (including self)  (ex : tree(1, 
                            //                                                                              left: tree(2) , 
                            //                                                                              right : tree(3)).number_elements == 3   )
    struct tree *left;      // left sub-tree
    struct tree *right;     // right sub-tree
}tree_t;


/**
 * @brief Travel in a in-order tour the binary tree
 * 
 * @param tree : the binary tree to travel
 * @return int* : an array containing the list of integers according to the in-order tour;
 *                NULL if the tree is NULL
 */
int *in_order(tree_t *tree);

#endif