#pragma once
struct deff_tree_element {
    double value;
    int type;
    deff_tree_element * left;
    deff_tree_element * right;
};

struct deff_tree {
    deff_tree_element * root;
    int size;
};

const int op_name_len = 10;

typedef deff_tree_element* elem_ptr;


char get_op_sign(double op_num);
deff_tree_element * node_ctor(double value, int type);

void tie_child_node(elem_ptr * parent, double value, int type);
int tree_ctor(deff_tree * tree);
char get_op_sign(double op_num);

enum operations {
    OP_ADD = 43,
    OP_SUB = 45,
    OP_MUL = 42,
    OP_DIV = 47
};

enum types_of_node {
    value_t = 1,
    operator_t = 2,
    variable_t = 3
};


#define inscect_symbol(symbol, pfile)         \
    if (check_symbol(symbol, pfile) == 0) {   \
        return 0;                             \
    }

int read_node(elem_ptr * node, FILE * pfile, double value, int type);
int read_data(deff_tree * tree, char * filename = "data.txt");