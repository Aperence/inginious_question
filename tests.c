#include <CUnit/Basic.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <mcheck.h>
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>

#include "inOrder.h"


// initialize malloc_hook to count the number of malloc called (see https://man7.org/linux/man-pages/man3/malloc_hook.3.html)
int nb_malloc = 0;
// initialize free_hook to count the number of free called (see https://www.gnu.org/software/libc/manual/html_node/Hooks-for-Malloc.html)
int nb_free = 0;

int enabled = 1;

/* Variables to save original hooks */
static void *(*old_malloc_hook)(size_t, const void *);
void (*volatile old_free_hook)(void *__ptr, const void *);

static void *my_malloc_hook(size_t size, const void *caller)
{
    void *result;

    /* Restore all old hooks */
    __malloc_hook = old_malloc_hook;

    /* Call recursively */
    result = malloc(size);

    /* Save underlying hooks */
    old_malloc_hook = __malloc_hook;

    nb_malloc++;

    /* Restore our own hooks */
    __malloc_hook = my_malloc_hook;

    return result;
}

static void *my_malloc_hook_failed(size_t size, const void *caller){
    return NULL;
}

static void
my_free_hook (void *ptr, const void *caller)
{
  /* Restore all old hooks */
  __malloc_hook = old_malloc_hook;
  __free_hook = old_free_hook;
  /* Call recursively */
  free (ptr);
  /* Save underlying hooks */
  old_malloc_hook = __malloc_hook;
  old_free_hook = __free_hook;

  nb_free++;

  /* Restore our own hooks */
  __malloc_hook = my_malloc_hook;
  __free_hook = my_free_hook;
}

static void
my_init (void)
{
  old_malloc_hook = __malloc_hook;
  old_free_hook = __free_hook;
  __malloc_hook = my_malloc_hook;
  __free_hook = my_free_hook;
}

static void
malloc_fail_activation(void)
{
    if (!enabled){
        enabled = 1;
        __malloc_hook = my_malloc_hook_failed;
    }
}

static void
malloc_fail_deactivation (void)
{
    if (enabled){
        enabled = 0;
        __malloc_hook = my_malloc_hook;
    }
}

void (*__malloc_initialize_hook)(void) = my_init;


tree_t *insertNode(tree_t *tree, int value){
    if (tree == NULL){
        tree_t *new = malloc(sizeof(tree_t));
        new->number_elements = 1;
        new->left = NULL;
        new->right = NULL;
        new->value = value;
        return new;
    }
    tree->number_elements++;
    if (value >= tree->value && tree->right != NULL){
        insertNode(tree->right, value);
        return tree;
    }
    if (value < tree->value && tree->left != NULL){
        insertNode(tree->left, value);
        return tree;
    }
    tree_t *new = malloc(sizeof(tree_t));
    new->number_elements = 1;
    new->left = NULL;
    new->right = NULL;
    new->value = value;
    if (value >= tree->value && tree->right == NULL){
        tree->right = new;
        return tree;
    }else{
        tree->left = new;
        return tree;
    }
}

void freeTree(tree_t *tree){
    if (tree == NULL) return;
    freeTree(tree->left);
    freeTree(tree->right);
    free(tree);
}

tree_t *createTree(int *values, int number){
    tree_t *tree = NULL;
    for (int i = 0; i < number; i++)
    {
        tree = insertNode(tree, *(values+i));
    }
    return tree;
    
}

void test1(){

    int values[] = {1,0,2};  
    /**
     *        balanced tree 
     *              1
     *             / \
     *            0   2
     */
    int number = 3;

    tree_t *t = createTree(values, number);
    int expected[] = {0,1,2};

    nb_malloc = 0;
    nb_free = 0;

    int *ret = in_order(t);

    CU_ASSERT(nb_malloc - 1 == nb_free);    // check if all memory was freed (should only remain the malloc of the return value)
    for (int i = 0; i < t->number_elements; i++)
    {
        CU_ASSERT_EQUAL(*(ret+i), expected[i]);
    }

    freeTree(t);
    free(ret);

}

void test2(){

    int values[] = {0,5,10};
    /**
     *        right tree 
     *              0
     *               \
     *                5
     *                 \
     *                  10
     */
    int number = 3;

    tree_t *t = createTree(values, number);
    int expected[] = {0,5,10};

    nb_malloc = 0;
    nb_free = 0;

    int *ret = in_order(t);

    CU_ASSERT(nb_malloc - 1 == nb_free);   // check if all memory was freed (should only remain the malloc of the return value)
    for (int i = 0; i < t->number_elements; i++)
    {
        CU_ASSERT_EQUAL(*(ret+i), expected[i]);
    }

    freeTree(t);
    free(ret);
    
}

void test3(){

    int values[] = {15,0,-3}; 
    int number = 3;

    /**
     *        left tree 
     *              15
     *             /
     *            0
     *           /
     *         -3
     */

    tree_t *t = createTree(values, number);
    int expected[] = {-3,0,15};

    nb_malloc = 0;
    nb_free = 0;

    int *ret = in_order(t);

    CU_ASSERT(nb_malloc - 1 == nb_free);     // check if all memory was freed (should only remain the malloc of the return value)
    for (int i = 0; i < t->number_elements; i++)
    {
        CU_ASSERT_EQUAL(*(ret+i), expected[i]);
    }

    freeTree(t);
    free(ret);
    
}

void test4(){

    int values[] = {15, 0, 22, -3, 1, 20, 21}; 
    int number = 7;

    /**
     *   
     *               15
     *             /    \
     *            0      22
     *           / \    /
     *         -3   1  20
     *                  \
     *                   21
     */

    tree_t *t = createTree(values, number);
    int expected[] = {-3,0,1, 15, 20, 21, 22};

    nb_malloc = 0;
    nb_free = 0;

    int *ret = in_order(t);

    CU_ASSERT(nb_malloc - 1 == nb_free);     // check if all memory was freed (should only remain the malloc of the return value)
    for (int i = 0; i < t->number_elements; i++)
    {
        CU_ASSERT_EQUAL(*(ret+i), expected[i]);
    }

    freeTree(t);
    free(ret);
    
}

void test5(){
    srand( time( NULL ) );
    

    int values[1000];
    int expected[1000];
    int number = 1000;

    for (int i = 0; i < number; i++)
    {
        expected[i] = 0;
    }

    for (int i = 0; i < number; i++)
    {
        int random = rand() % number;
        values[i] = random;
        expected[random]++;
    }
    

    tree_t *t = createTree(values, number);
    nb_malloc = 0;
    nb_free = 0;

    int *ret = in_order(t);

    CU_ASSERT(nb_malloc - 1 == nb_free);     // check if all memory was freed (should only remain the malloc of the return value)

    for (int i = 0; i < t->number_elements; i++)   // check if all elements appeared the right number of times
    {
        CU_ASSERT(expected[*(ret+i)] > 0);
        expected[*(ret+i)]--;
    }

    for (int i = 0; i < t->number_elements - 1; i++)   // check if all elements appeared in the expected order
    {
        CU_ASSERT(*(ret+i) <= *(ret+i+1));
    }

    freeTree(t);

    free(ret);
    
}

sig_t def;

void fail(int sig){
    malloc_fail_deactivation();
    signal(SIGSEGV, def);
    CU_FAIL_FATAL();
}

void test6(){

    def = signal(SIGSEGV, (sig_t) fail);
    signal(SIGABRT, fail);

    int a[] = {0,1,2};
    int number = 3;

    tree_t *t = createTree(a, number);

    malloc_fail_activation();              // make malloc fail

    int *res = in_order(t);

    malloc_fail_deactivation();     // reactivate malloc normal behaviour

    signal(SIGSEGV, def);

    CU_ASSERT(res == NULL);

    free(res);
    freeTree(t);
}


void test7(){

    tree_t *t = NULL;
    nb_malloc = 0;
    nb_free = 0;

    def = signal(SIGSEGV, (sig_t) fail);

    CU_ASSERT(in_order(t) == NULL);
    CU_ASSERT(nb_malloc == 0);
    CU_ASSERT(nb_free == 0);

    signal(SIGSEGV, def);
}
 

int main(int argc,char** argv)
{

    CU_pSuite pSuite = NULL;
    my_init();
    /* ensemble de tests CUnit */
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();
    pSuite = CU_add_suite("Suite_1", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    

    mallopt(M_CHECK_ACTION, 3);

    /* Ajout des tests */
    if ((NULL == CU_add_test(pSuite, "test in-order simple balanced binary tree", test1)
    || (NULL == CU_add_test(pSuite, "test in-order linear binary tree (only right child)", test2))
    || (NULL == CU_add_test(pSuite, "test in-order linear binary tree (only left child)", test3))
    || (NULL == CU_add_test(pSuite, "test in-order binary tree (with missing right/left children)", test4))
    || (NULL == CU_add_test(pSuite, "test in-order binary tree (random tree)", test5))
    || (NULL == CU_add_test(pSuite, "test in-order binary tree with failed malloc", test6))
    || (NULL == CU_add_test(pSuite, "test in-order binary tree with NULL tree", test7))
        ))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
  

}