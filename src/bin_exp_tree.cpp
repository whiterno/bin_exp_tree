#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/bin_exp_tree.h"

static int nodesDtor(Node* node);

int binExpTreeCtor(BinExpTree** tree INIT_ARGS_BET){
    assert(tree);

    (*tree)                 = (BinExpTree*)calloc(1, sizeof(BinExpTree));
    (*tree)->root           = NULL;
    (*tree)->nodes_amount   = 0;

    INIT_DEBUG_VARS_BET(tree)

    return NO_ERROR;
}

Node* createNode(int data_type, int data_value, Node* left, Node* right){
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    new_node->left      = left;
    new_node->right     = right;
    new_node->parent    = NULL;

    new_node->type      = data_type;
    new_node->value     = data_value;

    return new_node;
}

int countNodes(Node* node, int* amount){
    assert(node);

    if (node->left)     countNodes(node->left, amount);
    if (node->right)    countNodes(node->right, amount);

    (*amount)++;

    return NO_ERROR;
}

int binExpTreeDtor(BinExpTree** tree){
    assert(*tree);

    nodesDtor((*tree)->root);
    free(*tree);
    *tree = NULL;

    return NO_ERROR;
}

static int nodesDtor(Node* node){
    if (node == NULL) return NO_ERROR;

    if (node->left ==  NULL){
        free(node);

        return NO_ERROR;
    }

    nodesDtor(node->left);
    nodesDtor(node->right);

    free(node);

    return NO_ERROR;
}
