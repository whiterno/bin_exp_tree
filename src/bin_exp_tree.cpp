#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "../include/bin_exp_tree.h"

static Node* createNodeIn(Node* left, Node* right, DataType data_type);
static Node* createVarNode(Node* left, Node* right, char var);
static Node* createNumNode(Node* left, Node* right, double num);
static Node* createOperNode(Node* left, Node* right, Operations operation_type);

static int nodesDtor(Node* node);

int binExpTreeCtor(BinExpTree** tree INIT_ARGS_BET){
    assert(tree);

    (*tree)                 = (BinExpTree*)calloc(1, sizeof(BinExpTree));
    (*tree)->root           = NULL;
    (*tree)->nodes_amount   = 0;

    INIT_DEBUG_VARS_BET(tree)

    return NO_ERROR;
}

Node* createNode(Node* left, Node* right, DataType data_type, NodeValue value) {
    if (data_type == OPER) {
        return createOperNode(left, right, value.operation_type);
    }
    if (data_type == VAR){
        return createVarNode(left, right, value.variable);
    }
    if (data_type == NUM){
        return createNumNode(left, right, value.number);
    }

    return NULL;
}

static Node* createVarNode(Node* left, Node* right, char var){
    Node* node = createNodeIn(left, right, VAR);
    node->value.variable = var;

    return node;
}

static Node* createNumNode(Node* left, Node* right, double num){
    Node* node = createNodeIn(left, right, NUM);
    node->value.number = num;

    return node;
}

static Node* createOperNode(Node* left, Node* right, Operations operation_type){
    Node* node = createNodeIn(left, right, OPER);
    node->value.operation_type = operation_type;

    return node;
}

static Node* createNodeIn(Node* left, Node* right, DataType data_type){
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    new_node->left      = left;
    new_node->right     = right;
    new_node->parent    = NULL;
    new_node->type      = data_type;

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
