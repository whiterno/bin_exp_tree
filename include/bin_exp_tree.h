#ifndef BIN_EXP_TREE_H
#define BIN_EXP_TREE_H

#define INIT_BET(binExpTree) , #binExpTree, __FILE__, __func__, __LINE__
#define INIT_ARGS_BET , const char* name, const char* filename, const char* funcname, size_t line
#define INIT_DEBUG_VARS_BET(binExpTree) (*binExpTree)->name = name; (*binExpTree)->filename = filename; (*binExpTree)->funcname = funcname; (*binExpTree)->line = line;
#define DUMP_BET(binExpTree) binExpTree, __FILE__, __func__, __LINE__

enum DataType{
    OPER,
    VAR,
    NUM,
};

enum Operations{
    ADD,
    SUB,
    MULT,
    DIV,
    POW,
    LN,
    EXP,
    SIN,
    COS,
    TAN,
    COT,
    UNKNOWN,
};

union NodeValue{
    char variable;
    double number;
    Operations operation_type;
};

struct Node{
    DataType type;
    NodeValue value;

    Node* left;
    Node* right;
    Node* parent;
};

struct BinExpTree{
    const char* name;           ///< tree's name
    const char* filename;       ///< filename where tree was initialized
    const char* funcname;       ///< funcname where tree was initialized
    size_t line;                ///< serial number of line where tree was initialized

    Node* root;                 ///< pointer on tree's root
    int nodes_amount;           ///< amount of nodes in tree
};

enum ErrorsBinExpTree{
    NO_ERROR,
    NO_SUCH_DATA_TYPE,
    NULL_VALUE_INSERTED,
    FILE_NOT_OPEN,
    CYCLE_IN_TREE,
};

int binExpTreeCtor(BinExpTree** tree INIT_ARGS_BET);
Node* createNode(Node* left, Node* right, DataType data_type, NodeValue value);
int countNodes(Node* node, int* amount);
int binExpTreeDtor(BinExpTree** tree);
int nodesDtor(Node* node);

#endif
