#include <stdlib.h>
#include <stdio.h>
#include "deff.h"
#include "deff_dump.h"
#include <string.h>  // delete

// verificator upgrade

// tree calc
// variables
// no brackets
// tex outoup
// single ops
                           // deff itself
// dtor


int error = 0;

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
    // tie_child_node(&(tree.root->left), 12, 1);
    // tie_child_node(&(tree.root->right), 21, 1);
    // tie_child_node(&(tree.root->left->left), 43, 2);
    // tie_child_node(&(tree.root->left->left->left), 41, 1);
    // tie_child_node(&(tree.root->left->right), 363, 1);
    read_data(&tree);
    print_tree_inorder(tree.root);

    tree_visualize(tree.root);
    html_dump();
    verify(tree.root);
}


char get_op_sign(double op_num) {
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

    default:
        op_sign = '0';
        break;
    }

    return op_sign;
}






int read_node(elem_ptr * link, FILE * pfile, elem_ptr * parent) {
    if (check_symbol('(', pfile) == 1) {
        tie_child_node(link, 0, 0, parent);
        read_node(&((*link)->left), pfile, link);

        double value = 0;
        char op = '0';

        if (fscanf(pfile, "%lf", &value) == 1) {
            (*link)->type = 1;
            (*link)->value = value;
        } else if (fscanf(pfile, "%c", &op) == 1) {
            (*link)->type = 2;
            (*link)->value = (double)op;        // do new func for set type and value
        }

        read_node(&((*link)->right), pfile, link);
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
    read_node(&(tree->root), pfile, &(tree->root));
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
            error = 1;
        }
    } else if ((int)element->type == operator_t) {
        if (element->left == NULL || element->right == NULL) {
            printf("%p op does not have all numbers", element);
            error = 1;
        }
    }
    tree_verify(element->right);
    if (error == 1) {
        return 1;
    }
    return 0;
}





