
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

typedef deff_tree_element * elem_ptr;



void tree_visualize(deff_tree * tree);

void print_tree_inorder(deff_tree_element * root);

enum operations {
    OP_ADD = 1,
    OP_SUB = 2,
    OP_MUL = 3,
    OP_DIV = 4
};

enum types_of_node {
    value_t = 1,
    operator_t = 2,
    variable_t = 3
};