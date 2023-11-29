#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "deff_dump.h"
#include <string.h>  // delete
#include <math.h>
// tree calc

// verificator upgrade    
// add check for single ops

// deff itself
// dsl
int error_status = 0;
op_names_numbers_t op_names_numbers[op_count] = {
        {OP_ADD, "+"},
        {OP_SUB, "-"},
        {OP_MUL, "*"},
        {OP_DIV, "/"},
        {OP_SQRT, "\\sqrt"},
        {OP_SIN, "\\sin"},
        {OP_COS, "\\cos"},
        {OP_POW, "^"}
};
static bool check_symbol(char symbol, FILE * pfile);

int main(void) {
    diff_tree tree = {};
    tree_ctor(&tree);
    read_data(&tree);

    //print_tree_inorder(tree.root);
    // print_in_pretty_way(tree.root);

    // tree_visualize(tree.root);
    // html_dump();
    //verify(tree.root);
    print_tex(tree.root);
    //tree_dtor(&(tree.root));
    tree_visualize(tree.root);
    printf("%lf",tree_eval(tree.root, 0));
}

diff_tree_element * node_ctor(double value, types_of_node type) {
    diff_tree_element * element = (diff_tree_element *) calloc(1, sizeof(diff_tree_element));
    element->type = type;
    element->value = value;
    element->left = NULL;
    element->right = NULL;
    element->parent = NULL;
    return element;
}

void tie_child_node(elem_ptr * link, double value, types_of_node type, elem_ptr parent) {        // it like "tie" to the given link
    *link = node_ctor(value, type);
    (*link)->parent = parent;
    return;
}

int tree_ctor(diff_tree * tree) {
    tree->root = node_ctor(0, value_t);  // it is not null, because in reader must be not null ptr to write there
    tree->size = 0;
    return 0;
}

const char * get_op_symbol(double op_num) {
    int i = 0;
    while (op_num != op_names_numbers[i].number) i++;
    return op_names_numbers[i].name;
}
double get_op_number(char * name) {
    int i = 0;
    while (strcmp(op_names_numbers[i].name, name)) i++;
    return op_names_numbers[i].number;
}


int read_node_data(elem_ptr * link, FILE * pfile, elem_ptr * parent) {  //more clear
    if (check_symbol('(', pfile) == 1) {
        tie_child_node(link, 0, value_t, *parent);
        read_node_data(&((*link)->left), pfile, link);

        double value = 0;
        char op[op_name_len] = {};

        if (fscanf(pfile, "%lf", &value) == 1) {
            (*link)->type = 1;
            (*link)->value = value;
        } else if (fscanf(pfile, "%[^(]s", &op) == 1) {
 
                (*link)->type = 2;// enum
                (*link)->value = get_op_number(op); 
            //}    
        }

        read_node_data(&((*link)->right), pfile, link);
        inscect_symbol(')');  // require

    } else if (check_symbol(nil, pfile) == 1) {
        return 0;
    }
}


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
    
    if (is_found == 1) {
        return 1;
    }
    return 0;
}


int read_data(diff_tree * tree, char * filename) {
    FILE * pfile = fopen(filename, "r");
    null_ptr_file;
    read_node_data(&(tree->root), pfile, &(tree->root));
    fclose(pfile);
    return 0;
}


int tree_verify(diff_tree_element * element) {
    if (element == NULL) {
        return 1;
    }
    tree_verify(element->left);
    if (element->type == value_t) {
        if (element->left != NULL || element->right != NULL) {
            printf("%p number does not have all nulls", element);
            error_status = 1;
        }
    } else if ((int)element->type == operator_t) {
        if (element->left == NULL || element->right == NULL) {
            printf("%p op does not have all numbers", element);
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

void print_in_pretty_way(diff_tree_element * root) {
    if (root == NULL) {     
        return;
    }
    if (root->type != value_t && (op_priority(root->value, root->parent->value) == 1)) {// caps + variable
        printf("(");
    }
    print_in_pretty_way(root->left);
    if (root->type == value_t) {
        printf("%.2lf", root->value);
    } else if ((int)root->type == operator_t) {
        printf("%s", get_op_symbol(root->value));
    } else if ((int)root->type == variable_t) {
        printf("x");
    }
    
    print_in_pretty_way(root->right);
    if (root->type != value_t && (op_priority(root->value, root->parent->value) == 1)) {
        printf(")");
    }
    return;
}

void print_tex_single_equation(diff_tree_element * root, FILE * pfile) {
    if (root == NULL) {     
        return;
    }

    if (root->value == OP_DIV) {
        fprintf(pfile, "\\frac{");
    }
    if (is_bracket) {
        fprintf(pfile,"(");
    }

    print_tex_single_equation(root->left, pfile);
    int bracket = 0;

    if(root->type == value_t) {
         fprintf(pfile,"%.2lf", root->value);
    } else if (root->type == operator_t) {
        if (((int)root->value < OP_DIV)) {
            fprintf(pfile,"%s", get_op_symbol(root->value));
        } else {
            bracket = 1;
            switch ((int)root->value) {
                case OP_POW:
                    fprintf(pfile,"^{");
                    break;
                case OP_DIV:
                    fprintf(pfile,"}{");
                    break;
                default:
                    fprintf(pfile,"%s{", get_op_symbol(root->value));
            }
        }
    } else if ((int)root->type == variable_t) {
        fprintf(pfile, "x");
    }

    print_tex_single_equation(root->right, pfile);

    if (is_bracket) {
        fprintf(pfile,")");
    }

    if (bracket == 1) {
       fprintf(pfile,"}");
    }
    return;
}

int print_tex(diff_tree_element * root, char * file_name) {
    FILE * pfile = fopen(file_name, "w");
    null_ptr_file;
    fprintf(pfile, "$$");
    print_tex_single_equation(root, pfile);
    fprintf(pfile, "$$");
    fclose(pfile);
}

void tree_dtor(elem_ptr * root) {
    if (*root == NULL) {     
        return;
    }
    tree_dtor(&(*root)->left);
    (*root)->parent = NULL;
    (*root)->type = 0;
    (*root)->value= 0;
    tree_dtor(&(*root)->right);
    (*root)->right = NULL;
    (*root)->left = NULL;
    free(*root);
    return;
}

double tree_eval(diff_tree_element * element, double x_value) {
    if (element->type == value_t) {
        return element->value;
    }
    if (element->type == variable_t) {
        return x_value;
    }
    double right_value = 0;
    double  left_value = 0;
    if (OP_SQRT >= (int)element->value || (int)element->value >= OP_COS) {
        left_value = tree_eval(element->left, x_value);
    }
    right_value = tree_eval(element->right, x_value);
    switch ((int)element->value) {
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




