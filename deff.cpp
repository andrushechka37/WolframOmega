#include <stdlib.h>
#include <stdio.h>
#include "deff.h"
#include "deff_dump.h"

// reader------------hard
// verificator
// tree calc
// variables
// no brackets

deff_tree_element * node_ctor(double value, int type) {
    deff_tree_element * element = (deff_tree_element *) calloc(1, sizeof(deff_tree_element));
    element->type = type;
    element->value = value;
    element->left = NULL;
    element->right = NULL;
    return element;
}

void tie_child_node(elem_ptr * parent, double value, int type) {        // it like "tie" to the given link
    *parent = node_ctor(value, type);
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
    tie_child_node(&(tree.root->left), 12, 1);
    tie_child_node(&(tree.root->right), 21, 1);
    tie_child_node(&(tree.root->left->left), 43, 2);
    tie_child_node(&(tree.root->left->left->left), 41, 1);
    tie_child_node(&(tree.root->left->right), 363, 1);
    //read_data(&tree);
    print_tree_inorder(tree.root);

    tree_visualize(&tree);
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















static bool check_symbol(char symbol, FILE * pfile) {
    char check_char = '0';
    fscanf(pfile, "%c", &check_char);
    if (check_char != symbol) {                           
        printf("syntax error %c\n", check_char);          
        return 0;                                         
    } 
    fscanf(pfile, "%c", &check_char);                     
    if (check_char != '\n') {                            
        ungetc(check_char, pfile);          
    }
    return 1;
}



int read_node(elem_ptr * node, FILE * pfile, double value, int type) {
    inscect_symbol('(', pfile);
    if (fscanf(pfile, "(%lf", &value) == 1) {
        read_node(&((*node)->left), pfile, value, 1);
    } else if (fscanf(pfile, "(%c", (char *) &value) == 1) {
        read_node(&((*node)->left), pfile, value, 2);
    } else {
        inscect_symbol('_', pfile);
    }

    tie_child_node(node, value, type);

    if (fscanf(pfile, "(%lf", &value) == 1) {
        read_node(&((*node)->right), pfile, value, 1);
    } else if (fscanf(pfile, "(%c", (char *)&value) == 1) {
        read_node(&((*node)->right), pfile, value, 2);
    } else {
        inscect_symbol('_', pfile);
    }

    inscect_symbol(')', pfile);
    return 0;
}

int read_data(deff_tree * tree, char * filename) {
    FILE * pfile = fopen(filename, "r");
    char op = '0';
    fscanf(pfile, "(%c", &op);
    read_node(&(tree->root), pfile, (double)op, 2);
    fclose(pfile);
    return 0;
}
