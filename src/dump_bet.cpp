#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/bin_exp_tree.h"
#include "../include/dump_bet.h"

static int dump_img_counter = 0;
static FILE* dump_html = fopen(DUMP_HTML_FILENAME, "w");

static const char* enumToString(int error);
static int createDotFile(Node* root);
static int createNodesAndEdges(FILE* dump_dot, Node* node, TreeDumpAttributes* attrs);
static int writeNode(FILE* dump_dot, Node* node, TreeDumpAttributes* attrs);
static int writeEdge(FILE* dump_dot, TreeDumpAttributes* attrs, long long node, Node* parent, const char label[]);

static int printLabel(FILE* dump_dot, Node* node);
static int printOperLabel(FILE* dump_dot, Node* node);

int binExpTreeDump(BinExpTree* tree, const char filename[], const char funcname[], const size_t line, int error){
    if (tree == NULL || error == NULL_VALUE_INSERTED){
        return NULL_VALUE_INSERTED;
    }

    createDotFile(tree->root);

    fprintf(dump_html, "\n<pre>\n");
    fprintf(dump_html, "\tERROR: %s", enumToString(error));

    fprintf(dump_html, "\n\tDump called from %s:%s:%lu", filename, funcname, line);
    fprintf(dump_html, "\n\tTree %s born in %s:%s:%lu", tree->name, tree->filename, tree->funcname, tree->line);

    fprintf(dump_html, "\n\tBinary Expression Tree[%p]{", tree);
    fprintf(dump_html, "\n\t\troot = %p", tree->root);
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

    writeNode(dump_dot, node, attrs);
    if (node->left){
        fprintf(dump_dot, "\tnode_%lld: <fl%lld> -> node_%lld [", (long long)node, (long long)node, (long long)node->left);
        writeEdge(dump_dot, attrs, (long long)node->left, node, "");
        createNodesAndEdges(dump_dot, node->left, attrs);
    }
    if (node->right){
        fprintf(dump_dot, "\tnode_%lld: <fr%lld> -> node_%lld [", (long long)node, (long long)node, (long long)node->right);
        writeEdge(dump_dot, attrs, (long long)node->right, node, "");
        createNodesAndEdges(dump_dot, node->right, attrs);
    }

    return NO_ERROR;
}

static int writeNode(FILE* dump_dot, Node* node, TreeDumpAttributes* attrs){
    assert(node);
    assert(attrs);
    assert(dump_dot);

    fprintf(dump_dot, "\tnode_%lld [", (long long)node);
    fprintf(dump_dot,  "color = \"%s\", ", attrs->node_color);
    fprintf(dump_dot,  "style = \"filled\", ");
    fprintf(dump_dot,  "shape = \"%s\", ", attrs->node_shape);
    fprintf(dump_dot,  "fillcolor = \"%s\", ", attrs->node_fillcolor);
    printLabel(dump_dot, node);
    fprintf(dump_dot,   " | {<fl%lld> %p | <fr%lld> %p}}\"]\n", (long long)node, node->left, (long long)node, node->right);

    return NO_ERROR;
}

static int printLabel(FILE* dump_dot, Node* node){
    switch(node->type){
        case(OPER):{
            printOperLabel(dump_dot, node);
            return NO_ERROR;
        }
        case(VAR):{
            fprintf(dump_dot, "label = \"{%c ", node->value.variable);
            return NO_ERROR;
        }
        case(NUM):{
            fprintf(dump_dot, "label = \"{%lg ", node->value.number);
            return NO_ERROR;
        }
    }

    return UNKNOWN;
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
        case(UNKNOWN):{
            break;
        }
    }

    return NO_ERROR;
}

static int writeEdge(FILE* dump_dot, TreeDumpAttributes* attrs, long long node, Node* parent, const char label[]){
    assert(dump_dot);
    assert(attrs);
    assert(label);

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
