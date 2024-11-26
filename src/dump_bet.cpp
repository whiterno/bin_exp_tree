#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/bin_exp_tree.h"
#include "../include/dump_bet.h"

static int dump_img_counter = 0;
static int node_counter = 0;
static FILE* dump_html = fopen(DUMP_HTML_FILENAME, "w");

static const char* enumToString(int error);
static int createDotFile(Node* root);
static int createNodesAndEdges(FILE* dump_dot, Node* node, TreeDumpAttributes* attrs);
static int writeNode(FILE* dump_dot, Node* node, TreeDumpAttributes* attrs, int node_serial_number);
static int writeEdge(FILE* dump_dot, TreeDumpAttributes* attrs, int node_serial_number, const char label[]);

static int printLabel(FILE* dump_dot, Node* node);
static int printOperLabel(FILE* dump_dot, Node* node);
static int printVarLabel(FILE* dump_dot, Node* node);


int binExpTreeDump(BinExpTree* tree, const char filename[], const char funcname[], const size_t line, int error){
    if (tree == NULL || error == NULL_VALUE_INSERTED){
        return NULL_VALUE_INSERTED;
    }
    node_counter = tree->nodes_amount;

    createDotFile(tree->root);

    fprintf(dump_html, "\n<pre>\n");
    fprintf(dump_html, "\tERROR: %s", enumToString(error));

    fprintf(dump_html, "\n\tDump called from %s:%s:%lu", filename, funcname, line);
    fprintf(dump_html, "\n\tTree %s born in %s:%s:%lu", tree->name, tree->filename, tree->funcname, tree->line);

    fprintf(dump_html, "\n\tBinary Expression Tree[%p]{", tree);
    fprintf(dump_html, "\n\t\troot = %p", tree->root);
    fprintf(dump_html, "\n\t\tnodes_amount = %d", tree->nodes_amount);
    fprintf(dump_html, "\n\t\t <img src = %d.png width = 50%%>", dump_img_counter - 1);
    fprintf(dump_html, "\n\n</pre>\n");

    return error;
}

static int createDotFile(Node* root){
    FILE* dump_dot = fopen(DUMP_DOT_FILENAME, "w");
    if (dump_dot == NULL){
        return FILE_NOT_OPEN;
    }

    TreeDumpAttributes attrs = {.node_color = "darkgreen",
                                .node_fillcolor = "white",
                                .node_shape = "record",
                                .edge_color = "black",
                                .edge_arrowhead = "normal"};

    fprintf(dump_dot, "digraph D{\n");
    fprintf(dump_dot, "\trankdir = TB\n\n");

    createNodesAndEdges(dump_dot, root, &attrs);

    fprintf(dump_dot, "}");
    fclose(dump_dot);

    char cmd[MAX_CMD_SIZE] = {};

    snprintf(cmd, MAX_CMD_SIZE, "dot ./log/dump.dot -Tpng -o ./log/%d.png", dump_img_counter++);
    system(cmd);

    return NO_ERROR;
}

static int createNodesAndEdges(FILE* dump_dot, Node* node, TreeDumpAttributes* attrs){
    if (node == NULL){
        return NO_ERROR;
    }
    if (node_counter < 0){
        return CYCLE_IN_TREE;
    }

    int node_serial_number = node_counter;
    writeNode(dump_dot, node, attrs, node_serial_number);
    if (node->left){
        writeEdge(dump_dot, attrs, node_serial_number, "");
        createNodesAndEdges(dump_dot, node->left, attrs);
    }
    if (node->right){
        writeEdge(dump_dot, attrs, node_serial_number, "");
        createNodesAndEdges(dump_dot, node->right, attrs);
    }

    return NO_ERROR;
}

static int writeNode(FILE* dump_dot, Node* node, TreeDumpAttributes* attrs, int node_serial_number){
    assert(node);
    assert(attrs);
    assert(dump_dot);

    fprintf(dump_dot, "\tnode_%d [", node_serial_number);
    fprintf(dump_dot,  "color = \"%s\", ", attrs->node_color);
    fprintf(dump_dot,  "style = \"filled\", ");
    fprintf(dump_dot,  "shape = \"%s\", ", attrs->node_shape);
    fprintf(dump_dot,  "fillcolor = \"%s\", ", attrs->node_fillcolor);
    printLabel(dump_dot, node);
    fprintf(dump_dot,   " | {<fl%d> %p | <fr%d> %p}}\"]\n", node_serial_number, node->left, node_serial_number, node->right);

    return NO_ERROR;
}

static int printLabel(FILE* dump_dot, Node* node){
    switch(node->type){
        case(OPER):{
            printOperLabel(dump_dot, node); break;
            break;
        }
        case(VAR):{
            printVarLabel(dump_dot, node); break;
            break;
        }
        case(NUM):{
            fprintf(dump_dot, "label = \"{%lg ", node->value.number); break;
        }
    }

    return NO_ERROR;
}

static int printOperLabel(FILE* dump_dot, Node* node){
    switch(node->value.operation_type){
        case(ADD):{
            fprintf(dump_dot, "label = \"{ + "); break;
        }
        case(SUB):{
            fprintf(dump_dot, "label = \"{ - "); break;
        }
        case(DIV):{
            fprintf(dump_dot, "label = \"{ / "); break;
        }
        case(MULT):{
            fprintf(dump_dot, "label = \"{ * "); break;
        }
        case(POW):{
            fprintf(dump_dot, "label = \"{ ^ "); break;
        }
        case(SIN):{
            fprintf(dump_dot, "label = \"{ sin "); break;
        }
        case(COS):{
            fprintf(dump_dot, "label = \"{ cos "); break;
        }
        case(TAN):{
            fprintf(dump_dot, "label = \"{ tan "); break;
        }
        case(COT):{
            fprintf(dump_dot, "label = \"{ cot "); break;
        }
    }

    return NO_ERROR;
}

static int printVarLabel(FILE* dump_dot, Node* node){
    if (node->value.number == 0){
        fprintf(dump_dot, "label = \"{ x ");
    }

    return NO_ERROR;
}

static int writeEdge(FILE* dump_dot, TreeDumpAttributes* attrs, int node_serial_number, const char label[]){
    assert(dump_dot);
    assert(attrs);
    assert(label);

    fprintf(dump_dot, "\tnode_%d: <fl%d> -> node_%d [", node_serial_number, node_serial_number, --node_counter);
    fprintf(dump_dot,  "color = \"%s\", ", attrs->edge_color);
    fprintf(dump_dot,  "arrowhead = \"%s\", ", attrs->edge_arrowhead);
    fprintf(dump_dot,  "label = \"%s\"]\n", label);

    return NO_ERROR;
}

#define _DESCR(error) case(error): return #error
static const char* enumToString(int enumerator){
    switch (enumerator)
    {
    _DESCR(NO_ERROR);
    _DESCR(NULL_VALUE_INSERTED);
    _DESCR(CYCLE_IN_TREE);
    _DESCR(FILE_NOT_OPEN);
    }

    return "IDK_YOU_IDIOT";
}
#undef _DESCR
