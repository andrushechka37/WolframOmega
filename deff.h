#pragma once

struct deff_tree_element {
    double value;
    int type;
    deff_tree_element * left;
    deff_tree_element * right;
    deff_tree_element * parent;
};

struct deff_tree {
    deff_tree_element * root;
    int size;
};

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


#define inscect_symbol(symbol)                   \
    if (check_symbol(symbol, pfile) == 0) {      \
        return 0;                                \
    }                                            \

#define verify(element)                  \
    error = 0;                           \
    if (tree_verify(element) == 1) {     \
        tree_visualize(element);         \
        return 0;                        \
    }



const int op_name_len = 10;
const char nil = '_';

typedef deff_tree_element* elem_ptr;



char get_op_sign(double op_num);
deff_tree_element * node_ctor(double value, int type);

void tie_child_node(elem_ptr * link, double value, int type, elem_ptr * parent);
int tree_ctor(deff_tree * tree);
char get_op_sign(double op_num);



int read_node(elem_ptr * link, FILE * pfile, elem_ptr * parent);
int read_data(deff_tree * tree, char * filename = "data.txt");
int tree_verify(deff_tree_element * element);
