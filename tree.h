#pragma once



const int op_name_len = 10;
const int op_count = 8;
const int funcs_count = 8;

struct op_names_numbers_t {
    const double number;
    const char name[op_name_len];
    const int arg_quantity;
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
struct op_info {
    operations op_number;
    int arg_quantity;
};
union node_value {
    op_info operetor;
    double number;
};

struct diff_tree_element {
    node_value value;
    int type;
    diff_tree_element * left;
    diff_tree_element * right;
    diff_tree_element * parent;
};

struct diff_tree {
    diff_tree_element * root;
    int size;
};

enum types_of_node {
    zero_t     = 0,
    value_t    = 1,
    operator_t = 2,
    variable_t = 3
};


#define REQUIRE_SYMBOL(symbol)                   \
    if (check_symbol(symbol, pfile) == 0) {      \
        return 1;                                \
    }                                            \

#define verify(element)                  \
    error_status = 0;                    \
    if (tree_verify(element) == 1) {     \
        tree_visualize(element);       \
        return 0;                        \
    }

#define IS_NULL_PTR(element)                    \
    if (element == NULL) {                      \
        printf("place holder of "               \
               "reporting null ptr element");   \
        return 0;                               \
    }

#define READ_DATA(element)                                      \
    if (read_data(element) == 1) {                              \
        printf("\x1b[31m" "segmentationnnn faulttttt\n");       \
        return 0;                                               \
    }   

const int op_priority_mask = 240;


const char nil = '_';

typedef diff_tree_element* elem_ptr;

#define IS_ROUND_BRACKET                                       \
    (element->type == operator_t) &&                              \
    (element->parent->type == operator_t) &&                      \
    (op_priority(element->value.operetor.op_number, element->parent->value.operetor.op_number) == 1)       \



diff_tree_element * node_ctor(double value, types_of_node type, diff_tree_element * left, 
                          diff_tree_element * right, diff_tree_element * parent);

int tree_ctor(diff_tree * tree);
void tree_dtor(elem_ptr * root);

const char * get_op_symbol(int op_num);          // maybe static
double get_op_number(char op_symbol);

int read_node_data(elem_ptr * link, FILE * pfile, elem_ptr * parent);
int read_data(diff_tree * tree, char * filename = "data.txt");

int tree_verify(diff_tree_element * element);
void print_in_pretty_way(diff_tree_element * root);
void print_tex_single_equation(diff_tree_element * root, FILE * pfile);

int print_tex(diff_tree_element * root, char * file_name = "tex.md");
double tree_eval(diff_tree_element * element, double x_value);

inline int error_status = 0;


#define ELEM_OP_NUM element->value.operetor.op_number
#define ELEM_DOUBLE element->value.number
#define ELEM_OP_ARG element->value.operetor.arg_quantity