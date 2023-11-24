#include <stdlib.h>
#include <stdio.h>
#include "deff_dump.h"
static void print_graph_arrows(deff_tree_element * element, FILE * pfile);
static void print_graph_node(deff_tree_element * element, FILE * pfile, int rank);


void print_tree_inorder(deff_tree_element * root) {
    if (root == NULL) {     
        printf("_");
        return;
    }
    printf("(");
    print_tree_inorder(root->left);
    if (root->type == value_t) {
        printf("%.2lf", root->value);
    } else if ((int)root->type == operator_t) {
        printf("%c", get_op_sign(root->value));
    }
    
    print_tree_inorder(root->right);
    printf(")");
    return;
}



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
    if (hard_visualize == 1) {                              // i know copypast, but i thuink it is not critical,
        fprintf(pfile, "\t%d[shape=Mrecord,style=filled, fillcolor=\"#7293ba\", rank = %d,"  // it is for better understanding
                   "label=\"{name: %p | {value: %.2lf | type: %d} | {left: %p | right: %p}}\"];\n", 
                   element, rank, element, element->value, 
                 element->type, element->left, element->right);
    } else {
        if (element->type == 1) {
            fprintf(pfile, "\t%d[shape=Mrecord,style=filled, fillcolor=\"#7293ba\", rank = %d," 
                   "label=\"%.2lf\"];\n", element, rank, element->value);
        } else if (element->type == 2) {
            fprintf(pfile, "\t%d[shape=circle,style=filled, fillcolor=\"#f77ca3\", rank = %d," 
                   "label=\"%c\"];\n", element, rank, get_op_sign(element->value));
        }

    }
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
    if (hard_visualize == 1) {
        fprintf(pfile, "\tnode[color=\"black\",fontsize=14];\n");
    } else {
        fprintf(pfile, "\tnode[color=\"black\",fontsize=18];\n");
    }

    fprintf(pfile, "\tedge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12,  width=0.4];\n\n\n");

    print_graph_node(tree->root, pfile, 1);

    fprintf(pfile, "\n\n\n\n");
    
    print_graph_arrows(tree->root, pfile);

    fprintf(pfile, "\n\n\n}");
    fclose(pfile);
    system("dot -Tpng ./graph.dot -o graph.png");
}