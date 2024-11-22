#include <stdio.h>

#include "../include/bin_exp_tree.h"
#include "../include/dump_bet.h"

#define _ADD(left, right)   createNode(OPER, ADD, left, right)
#define _SUB(left, right)   createNode(OPER, SUB, left, right)
#define _DIV(left, right)   createNode(OPER, DIV, left, right)
#define _MULT(left, right)  createNode(OPER, MULT, left, right)

#define _NUM(value)         createNode(NUM, value, NULL, NULL)

#define _X                  createNode(VAR, 0, NULL, NULL)

int main(){
    BinExpTree* tree = NULL;
    binExpTreeCtor(&tree INIT_BET(tree));

    tree->root = _DIV(_ADD(_X, _NUM(3)), _SUB(_NUM(1000), _NUM(7)));
    int nodes_amount = 0;
    countNodes(tree->root, &nodes_amount);
    tree->nodes_amount = nodes_amount;

    binExpTreeDump(DUMP_BET(tree), 0);

    binExpTreeDtor(&tree);
}
