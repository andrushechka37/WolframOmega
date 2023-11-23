#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "deff.h"


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
    tree->root = node_ctor(0, 0);
    tree->size = 0;
    return 0;
}

int main(void) {
    deff_tree tree = {};
    tree_ctor(&tree);
    tie_child_node(&(tree.root->left), 12, 1);
    tie_child_node(&(tree.root->right), 21, 1);
    tie_child_node(&(tree.root->left->left), 1, 2);
    tie_child_node(&(tree.root->left->left->left), 41, 1);
    tie_child_node(&(tree.root->left->right), 363, 1);
    print_tree_inorder(tree.root);

    tree_visualize(&tree);
}


char get_op_sign(int op_num) {
    char op_sign  = '0';
    switch (op_num)
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

void print_tree_inorder(deff_tree_element * root) {
    if (root == NULL) {       //         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        printf("_");
        return;
    }
    printf("(");
    print_tree_inorder(root->left);
    if (root->type == value_t) {
        printf("%.2lf", root->value);
    } else if ((int)root->type == operator_t) {
        printf("%c", get_op_sign((int)root->value));
    }
    
    print_tree_inorder(root->right);
    printf(")");
    return;
}

// static bool check_symbol(char symbol, FILE * pfile) {
//     char check_char = '0';
//     fscanf(pfile, "%c", &check_char);
//     if (check_char != symbol) {                           
//         printf("syntax error %c\n", check_char);          
//         return 0;                                         
//     } 
//     fscanf(pfile, "%c", &check_char);                     
//     if (check_char != '\n') {                            
//         ungetc(check_char, pfile);          
//     }
// }



// int read_data(char * data = "data.txt") {
//     FILE * pfile = fopen(data, "r");
//     int read_node_akinator(akinator_element ** node, FILE * pfile, char * text) {
//     akinator_add_descendant(node, text);
//     char check_char = '0';
//     check_symbol('>');
//     if (fscanf(pfile, "(<%[^>]s", text) == 1) {
//         text[strcspn(text, "\n")] = '\0';
//         read_node_akinator(&((*node)->left), pfile, text);
//     } else {
//         check_symbol('*');
//     }

//     if (fscanf(pfile, "(<%[^>]s", text) == 1) {
//         text[strcspn(text, "\n")] = '\0';
//         read_node_akinator(&((*node)->right), pfile, text);
//     } else {
//         check_symbol('*');
//     }
//     check_symbol(')');
//     return 0; 
// }
// }































static void print_graph_arrows(deff_tree_element * element, FILE * pfile) {
    if (element->left != NULL) {
        fprintf(pfile, "\t%d->%d [color = \"#22f230\"];\n", element, element->left);
        print_graph_arrows(element->left, pfile);
    }

    if (element->right != NULL) {
        fprintf(pfile, "\t%d->%d [color = \"orange\"];\n", element, element->right);
        print_graph_arrows(element->right, pfile);
    }
    return;
}

static void print_graph_node(deff_tree_element * element, FILE * pfile, int rank) {
    fprintf(pfile, "\t%d[shape=Mrecord,style=filled, fillcolor=\"#7293ba\", rank = %d," 
                   "label=\"{name: %p | {value: %.2lf | type: %d} | {left: %p | right: %p}}\"];\n", element, rank, element, element->value, 
                                                                    element->type, element->left, element->right);
    if (element->left != NULL) {
        print_graph_node(element->left, pfile, ++rank);
    }

    if (element->right != NULL) {
        print_graph_node(element->right, pfile, ++rank);
    }
    return;
}

void tree_visualize(deff_tree * tree) {
    FILE * pfile = fopen("graph.dot", "w");
    fprintf(pfile, "digraph structs {\n");
    fprintf(pfile, "\trankdir=HR;\n");
    fprintf(pfile, "\tgraph [bgcolor=\"#31353b\"]\n");
    fprintf(pfile, "\tnode[color=\"black\",fontsize=14];\n");
    fprintf(pfile, "\tedge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12,  width=0.4];\n\n\n");

    print_graph_node(tree->root, pfile, 1);

    fprintf(pfile, "\n\n\n\n");
    
    print_graph_arrows(tree->root, pfile);

    fprintf(pfile, "\n\n\n}");
    fclose(pfile);
    system("dot -Tpng ./graph.dot -o graph.png");
}