
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

typedef deff_tree_element * elem_ptr;



void tree_visualize(deff_tree * tree);