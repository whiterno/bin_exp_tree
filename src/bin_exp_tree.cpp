#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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

Node* createNode(Node* left, Node* right, int data_type, ...){
    va_list args;
    va_start(args, data_type);

    NodeValue data_value = {};
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    new_node->left      = left;
    new_node->right     = right;
    new_node->parent    = NULL;
    new_node->type      = (DataType)data_type;

    if (data_type == NUM){
        data_value.number       = va_arg(args, double);
        new_node->value.number  = data_value.number;
    }
    if (data_type == VAR){
        data_value.variable         = va_arg(args, int);
        new_node->value.variable    = data_value.variable;
    }
    if (data_type == OPER){
        data_value.operation_type       = (Operations)va_arg(args, int);
        new_node->value.operation_type  = data_value.operation_type;
    }
    va_end(args);

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
