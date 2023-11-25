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
    OP_ADD  = 17,   // 0001|0001     |
    OP_SUB  = 18,   // 0001|0010     |
    OP_MUL  = 33,   // 0010|0001     |    =>    elder 4 bits are for operation priority
    OP_DIV  = 34,   // 0010|0010     |          young 4 bits are for op number
    OP_SQRT = 49,   // 0011|0001     |
    OP_SIN  = 50,   // 0011|0010     |
    OP_COS  = 51,   // 0011|0011     |
    OP_POW  = 52    // 0011|0100     |
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
    error_status = 0;                    \
    if (tree_verify(element) == 1) {     \
        tree_visualize(element);         \
        return 0;                        \
    }


const int op_priority_mask = 240;

const int op_name_len = 10;
const char nil = '_';

typedef deff_tree_element* elem_ptr;


void tie_child_node(elem_ptr * link, double value, int type, elem_ptr * parent);
deff_tree_element * node_ctor(double value, int type);

int tree_ctor(deff_tree * tree);


char get_op_symbol(double op_num);          // maybe static
double get_op_number(char op_symbol);

int read_node(elem_ptr * link, FILE * pfile, elem_ptr * parent);
int read_data(deff_tree * tree, char * filename = "data.txt");

int tree_verify(deff_tree_element * element);
void print_in_pretty_way(deff_tree_element * root);
void print_tex_single_equation(deff_tree_element * root);
void print_tex_single_equation(deff_tree_element * root);