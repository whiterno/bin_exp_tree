#include <stdio.h>

#include "../include/bin_exp_tree.h"
#include "../include/dump_bet.h"

#define _ADD(left, right)   createNode(left, right, OPER, ADD)
#define _SUB(left, right)   createNode(left, right, OPER, SUB)
#define _DIV(left, right)   createNode(left, right, OPER, DIV)
#define _MULT(left, right)  createNode(left, right, OPER, MULT)

#define _NUM(value)         createNode(NULL, NULL, NUM, value)

#define _X                  createNode(NULL, NULL, VAR, 0)

int main(){
    BinExpTree* tree = NULL;
    binExpTreeCtor(&tree INIT_BET(tree));

    tree->root = _DIV(_ADD(_X, _NUM(3.0)), _SUB(_NUM(1000.0), _NUM(7.0)));
    int nodes_amount = 0;
    countNodes(tree->root, &nodes_amount);
    tree->nodes_amount = nodes_amount;

    binExpTreeDump(DUMP_BET(tree), 0);

    binExpTreeDtor(&tree);
}
