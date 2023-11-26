#include <stdlib.h>
#include <stdio.h>
#include "deff.h"
#include "deff_dump.h"
#include <string.h>  // delete

// dtor

// rewrite getting argunent dtom data scanf[^(]  // array of structures
// tree calc

// verificator upgrade           // add check for single ops
// variables
// deff itself
                

// dsl
int error_status = 0;

static bool check_symbol(char symbol, FILE * pfile);

deff_tree_element * node_ctor(double value, int type) {
    deff_tree_element * element = (deff_tree_element *) calloc(1, sizeof(deff_tree_element));
    element->type = type;
    element->value = value;
    element->left = NULL;
    element->right = NULL;
    element->parent = NULL;
    return element;
}

void tie_child_node(elem_ptr * link, double value, int type, elem_ptr * parent) {        // it like "tie" to the given link
    *link = node_ctor(value, type);
    (*link)->parent = *parent;
    return;
}

int tree_ctor(deff_tree * tree) {
    tree->root = node_ctor(47, 2);
    tree->size = 0;
    return 0;
}

int main(void) {
    deff_tree tree = {};
    tree_ctor(&tree);
    read_data(&tree);

    //print_tree_inorder(tree.root);
    //print_in_pretty_way(tree.root);

    tree_visualize(tree.root);
    html_dump();
    //verify(tree.root);
    print_tex(tree.root);
}


char get_op_symbol(double op_num) {
    char op_sign  = '0';
    switch ((int)op_num)
    {
    case OP_ADD:
        op_sign = '+';
        break;
    
    case OP_SUB:
        op_sign = '-';
        break;

    case OP_MUL:
        op_sign = '*';
        break;

    case OP_DIV:
        op_sign = '/';
        break;
    
    case OP_SQRT:
        op_sign = 'v';
        break;

    case OP_SIN:
        op_sign = 's';
        break;
    
    case OP_COS:
        op_sign = 'c';
        break;

    case OP_POW:
        op_sign = '^';
        break;


    default:
        op_sign = '0';
        break;
    }

    return op_sign;
}
double get_op_number(char op_symbol) {
    double number = 0;
    switch (op_symbol)
    {
    case '+':
        number = OP_ADD;
        break;
    case '-':
        number = OP_SUB;
        break;
    case '*':
        number = OP_MUL;
        break;
    case '/':
        number = OP_DIV;
        break;
    
    case 'v':
        number = OP_SQRT;
        break;
    case 's':
        number = OP_SIN;
        break;
    case 'c':
        number = OP_COS;
        break;
    case '^':
        number = OP_POW;
        break;
    default:
        number = 0;
        break;
    }
    return number;
}


int read_node_data(elem_ptr * link, FILE * pfile, elem_ptr * parent) {
    if (check_symbol('(', pfile) == 1) {
        tie_child_node(link, 0, 0, parent);
        read_node_data(&((*link)->left), pfile, link);

        double value = 0;
        char op = '0';

        if (fscanf(pfile, "%lf", &value) == 1) {
            (*link)->type = 1;
            (*link)->value = value;
        } else if (fscanf(pfile, "%c", &op) == 1) {
            (*link)->type = 2;
            (*link)->value = get_op_number(op);        // do new func for set type and value
        }

        read_node_data(&((*link)->right), pfile, link);
        inscect_symbol(')');

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


int read_data(deff_tree * tree, char * filename) {
    FILE * pfile = fopen(filename, "r");
    null_ptr_file;
    read_node_data(&(tree->root), pfile, &(tree->root));
    fclose(pfile);
    return 0;
}


int tree_verify(deff_tree_element * element) {
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
void print_in_pretty_way(deff_tree_element * root) {
    if (root == NULL) {     
        return;
    }
    if (root->type != value_t && (op_priority(root->value, root->parent->value) == 1)) {
        printf("(");
    }
    print_in_pretty_way(root->left);
    if (root->type == value_t) {
        printf("%.2lf", root->value);
    } else if ((int)root->type == operator_t) {
        printf("%c", get_op_symbol(root->value));
    }
    
    print_in_pretty_way(root->right);
    if (root->type != value_t && (op_priority(root->value, root->parent->value) == 1)) {
        printf(")");
    }
    return;
}

void print_tex_single_equation(deff_tree_element * root, FILE * pfile) {
    if (root == NULL) {     
        return;
    }

    if (root->value == OP_DIV) {
        fprintf(pfile, "\\frac{");
    }

    if (root->type != value_t && (op_priority(root->value, root->parent->value) == 1)) { // maybe define
        fprintf(pfile,"(");
    }
    print_tex_single_equation(root->left, pfile);
    if (root->type == value_t) {
        if ((int)root->parent->value == OP_POW) {
            fprintf(pfile,"{%.2lf}", root->value);
        } else {
            fprintf(pfile,"%.2lf", root->value);
        }
    } else if ((int)root->type == operator_t && (int)root->value != OP_DIV) {
        if ((int)root->value == OP_POW || (int)root->value == OP_SQRT) {}
        fprintf(pfile,"%c", get_op_symbol(root->value));
    }

    if (root->value == OP_DIV) {
        fprintf(pfile,"}{");
    }
    
    print_tex_single_equation(root->right, pfile);
    if (root->type != value_t && (op_priority(root->value, root->parent->value) == 1)) {
        fprintf(pfile,")");
    }

    if (root->value == OP_DIV) {
       fprintf(pfile,"}");
    }
    return;
}
int print_tex(deff_tree_element * root, char * file_name) {
    FILE * pfile = fopen(file_name, "w");
    null_ptr_file;
    fprintf(pfile, "$$");
    print_tex_single_equation(root, pfile);
    fprintf(pfile, "$$");
    fclose(pfile);

}