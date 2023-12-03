#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include <string.h>
#include <math.h>


static bool check_symbol(char symbol, FILE * pfile);
static int set_type_and_value(double value, types_of_node type, diff_tree_element * element);
static int get_op_arg_number(operations op);

op_names_numbers_t op_names_numbers[op_count] = {
        {OP_ADD, "+",       2},
        {OP_SUB, "-",       2},
        {OP_MUL, "*",       2},
        {OP_DIV, "/",       2},
        {OP_SQRT, "\\sqrt", 1},
        {OP_SIN, "\\sin",   1},
        {OP_COS, "\\cos",   1},
        {OP_POW, "^",       2}
};

diff_tree_element * node_ctor(double value, types_of_node type, diff_tree_element * left,
                              diff_tree_element * right, diff_tree_element * parent) {
    diff_tree_element * element = (diff_tree_element *) calloc(1, sizeof(diff_tree_element));
    element->type = type;
    switch (type)
    {
    case value_t:
        ELEM_DOUBLE = value;
        break;
    case operator_t:
        ELEM_OP_NUM = (operations) value;
        element->value.operetor.arg_quantity = get_op_arg_number((operations) value);
        break;
    }
    element->left = left;
    element->right = right;
    element->parent = parent;
    return element;
    
}

int tree_ctor(diff_tree * tree) {
    tree->root = node_ctor(0,zero_t, NULL, NULL, NULL);  // it is not null, because
    tree->size = 0;                                                    // in reader must be not null ptr to write there
    return 0;              
}

const char * get_op_symbol(int op_num) { 
    int i = 0;
    while (op_num != op_names_numbers[i].number) i++;      // switch case
    return op_names_numbers[i].name;
}

int get_op_number(char * name) { // do it faster with hashes instead of strcmp
    int i = 0;
    while (strcmp(op_names_numbers[i].name, name)) {
        i++;
        if (i > funcs_count) {
            return -1;
        }
    }
    return op_names_numbers[i].number;
}

#define LEFT &((*link)->left)
#define RIGHT &((*link)->right)

int read_node_data(elem_ptr * link, FILE * pfile, elem_ptr * parent) {  //more clear      // skip probels
    if (check_symbol('(', pfile) == 1) { // if it is new node or not
        *link = node_ctor(0, value_t, NULL, NULL, *parent);
        read_node_data(LEFT, pfile, link);

        double value = 0;                  // error obrabotka
        char op[op_name_len] = {};
        char x = '0';

        if (fscanf(pfile, "%lf", &value) == 1) {
            set_type_and_value(value, value_t, *link);
        } else if(check_symbol('x', pfile) == 1) {
            set_type_and_value(1, variable_t, *link);
        } else if (fscanf(pfile, "%[^(]s", &op) == 1) {         // not safe
            set_type_and_value(get_op_number(op), operator_t, *link);
        }

        read_node_data(RIGHT, pfile, link);
        REQUIRE_SYMBOL(')');

    } else if (check_symbol(nil, pfile) == 1) {
        return 0;
    }
}

#undef LEFT
#undef RIGHT

static bool check_symbol(char symbol, FILE * pfile) {
    bool is_found = 1;
    char check_char = getc(pfile);
    if (check_char != symbol) {                           
        ungetc(check_char, pfile);        
        is_found = 0;                                         
    } 
    check_char = getc(pfile);                  
    if (check_char != '\n') {                            
        ungetc(check_char, pfile);          
    }
    
    return is_found;
}

int read_data(diff_tree * tree, char * filename) {
    FILE * pfile = fopen(filename, "r");
    IS_NULL_PTR(pfile);
    if (read_node_data(&(tree->root), pfile, &(tree->root)) == 1) {
        fclose(pfile);
        return 1;
    }
    fclose(pfile);
    return 0;
}

int tree_verify(diff_tree_element * element) {
    if (element == NULL) {
        return 1;
    }
    tree_verify(element->left);

    switch (element->type) {
    case value_t:
        if (element->left != NULL || element->right != NULL) {   // if number node has left and right NULL clilds
            printf("%p number does not have all nulls", element);
            error_status = 1;
        }
        break;
    case operator_t:
        if (element->value.operetor.arg_quantity == 1) { // if operator has one argument
            if (element->left != NULL || element->right == NULL) {
                printf("%p op does not have all numbers 1", element);
                error_status = 1;
            }
        } else {
            if (element->left == NULL || element->right == NULL) { // if operator has two arguments
                printf("%p op does not have all numbers", element);
                error_status = 1;
            }
        }
    
    default:
        break;
    }

    if (element->left != NULL && element->right != NULL) {   // if parents clilds and childs parents are the same
        if(element->left->parent != element || element->right->parent != element) {
        printf("%p - left parent %p - right parent %p - elemen", element->left->parent, element->right->parent, element);
        error_status = 1;
        }
    }
    tree_verify(element->right);
    if (error_status == 1) {
        return 1;
    }
    return 0;
}

bool op_priority(double op1, double op2) {
    if (((int)op1 & op_priority_mask) < ((int)op2 & op_priority_mask)) {
        return 1;
    }
    return 0;
}


void print_tex_single_equation(diff_tree_element * element, FILE * pfile) {
    if (element == NULL) {     
        return;
    }
    if (element->value.operetor.op_number == OP_DIV) {
        fprintf(pfile, "\\frac{");
    }
    if (IS_ROUND_BRACKET) {
        fprintf(pfile,"(");
    }

    print_tex_single_equation(element->left, pfile);

    bool is_figure_bracket = 0;
    if(element->type == value_t) {
        if (element->value.number < 0) {
             fprintf(pfile,"(%.2lf)", element->value);
        } else {
            fprintf(pfile,"%.2lf", element->value);
        }
    } else if (element->type == operator_t) {
        if ((ELEM_OP_NUM < OP_DIV)) {
            fprintf(pfile,"%s", get_op_symbol(element->value.operetor.op_number));
        } else {
            is_figure_bracket = 1;
            switch (element->value.operetor.op_number) {
                case OP_POW:
                    fprintf(pfile,"^{");
                    break;
                case OP_DIV:
                    fprintf(pfile,"}{");
                    break;
                default:
                    fprintf(pfile,"%s{", get_op_symbol(element->value.operetor.op_number));
            }
        }
    } else if ((int)element->type == variable_t) {
        fprintf(pfile, "x");
    }

    print_tex_single_equation(element->right, pfile);

    if (IS_ROUND_BRACKET) {
        fprintf(pfile,")");
    }

    if (is_figure_bracket == 1) {  // maybe caps for style
       fprintf(pfile,"}");
    }
    return;
}

int print_tex(diff_tree_element * root, char * file_name) {
    FILE * pfile = fopen(file_name, "w");
    IS_NULL_PTR(pfile);
    fprintf(pfile, "$$");
    print_tex_single_equation(root, pfile);
    fprintf(pfile, "$$");
    fclose(pfile);
    return 0;
}

void tree_dtor(elem_ptr * root) {
    if (*root == NULL) {     
        return;
    }

    tree_dtor(&(*root)->left);

    (*root)->parent = NULL;
    set_type_and_value(0, (types_of_node)0, *root);

    tree_dtor(&(*root)->right);

    (*root)->right = NULL;
    (*root)->left = NULL;
    free(*root);
    (*root) = NULL;
    return;
}

double tree_eval(diff_tree_element * element, double x_value) {
    if (element->type == value_t) {
        return element->value.number;
    }
    if (element->type == variable_t) {
        return x_value;
    }

    double right_value = 0;
    double  left_value = 0;

    if (ELEM_OP_ARG == 2) {
        left_value = tree_eval(element->left, x_value);
    }
    right_value = tree_eval(element->right, x_value);

    switch (ELEM_OP_NUM) {
        case OP_ADD:
            return left_value + right_value;
        case OP_SUB:
            return left_value - right_value;
        case OP_MUL:
            return left_value * right_value;
        case OP_DIV:
            if (right_value != 0) {
                return left_value / right_value;
            } else {
                printf("divide on zero, error\n");
                return 0;
            }
        case OP_SIN:
            return sin(right_value);
        case OP_COS:
            return cos(right_value);
        case OP_POW:
            return pow(left_value, right_value);
        case OP_SQRT:
            return sqrt(right_value);
        default:
            printf("there is no that operation, error %lf\n", element->value);
            return 0;
    }
}

static int get_op_arg_number(operations op) {
    int i = 0;
    while (op != op_names_numbers[i].number) i++;
    return op_names_numbers[i].arg_quantity;
}

static int set_type_and_value(double value, types_of_node type, diff_tree_element * element) { // null ptr check
    IS_NULL_PTR(element);
    if (type == value_t) {
        element->value.number = value;
    } else if (type == operator_t) {
        ELEM_OP_NUM = (operations)value;
        ELEM_OP_ARG = get_op_arg_number((operations)value);

    }
    element->type = (int) type;
    return 0;
}


