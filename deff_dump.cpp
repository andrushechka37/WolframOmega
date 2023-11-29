#include <stdlib.h>
#include <stdio.h>
#include "deff_dump.h"
#include <string.h>
static void print_graph_arrows(diff_tree_element * element, FILE * pfile);
static void print_graph_node(diff_tree_element * element, FILE * pfile, int rank);


void print_tree_inorder(diff_tree_element * root) {
    if (root == NULL) {     
        printf("_");
        return;
    }
    printf("(");
    print_tree_inorder(root->left);
    if (root->type == value_t) {
        printf("%.2lf", root->value);
    } else if ((int)root->type == operator_t) {
        printf("%c", get_op_symbol(root->value));
    }
    
    print_tree_inorder(root->right);
    printf(")");
    return;
}



static void print_graph_arrows(diff_tree_element * element, FILE * pfile) {
    if (element->left != NULL) {
        fprintf(pfile, "\t%d->%d [color = \"#0ae7ff\"];\n", element, element->left);
        print_graph_arrows(element->left, pfile);
    }

    if (element->right != NULL) {
        fprintf(pfile, "\t%d->%d [color = \"#f0ff4f\"];\n", element, element->right);
        print_graph_arrows(element->right, pfile);
    }
    return;
}

static void print_graph_node(diff_tree_element * element, FILE * pfile, int rank) {
    if (hard_visualize == 1) {                              // i know copypast, but i thuink it is not critical,
        fprintf(pfile, "\t%d[shape=Mrecord,style=filled, fillcolor=\"#7293ba\", rank = %d,"  // it is for better understanding
                   "label=\"{parent: %p | name: %p | {value: %.2lf | type: %d} | {left: %p | right: %p}}\"];\n", 
                   element, rank, element->parent, element, element->value, 
                 element->type, element->left, element->right);
    } else {                                         // ifndef
        if (element->type == value_t) {
            fprintf(pfile, "\t%d[shape=Mrecord,style=filled, fillcolor=\"#7293ba\", rank = %d," 
                   "label=\"%.2lf\"];\n", element, rank, element->value);  // obertka
        } else if (element->type == operator_t) {
            fprintf(pfile, "\t%d[shape=circle,style=filled, fillcolor=\"#b481f7\", rank = %d," 
                   "label=\"%s\"];\n", element, rank, get_op_symbol(element->value));
        } else if (element->type == variable_t) {
            fprintf(pfile, "\t%d[shape=circle,style=filled, fillcolor=\"#f0ab67\", rank = %d," 
                   "label=\"x\"];\n", element, rank);
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

void tree_visualize(diff_tree_element * element) {
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

    print_graph_node(element, pfile, 1);

    fprintf(pfile, "\n\n\n\n");
    
    print_graph_arrows(element, pfile);

    fprintf(pfile, "\n\n\n}");
    fclose(pfile);
    create_new_graph();
}


void create_new_graph(void) {  // not tmp files, but better than previous hardcode of path
    char command1[command_len] = "dot -Tpng ./graph.dot -o graphs/graph";
    char command2[] = ".png";
    char graph_number_str[2] = {};

    snprintf(graph_number_str, 2,  "%d", graph_number);
    strcat(command1, graph_number_str);
    strcat(command1, command2);
    system(command1);
    graph_number++;
}

void html_dump(void) {
    FILE * pfile = fopen("log.html", "w"); // 

    fprintf(pfile, "<hr/>\n");
    fprintf(pfile, "<head>\n");
    fprintf(pfile, "\t<title>megalogg</title>\n");
    fprintf(pfile, "</head>\n");
    fprintf(pfile, "<body bgcolor=\"#a2a8a8\">\n");
    for (int i = 1; i < graph_number; i++) {
        fprintf(pfile, "<img src = \"/Users/anzhiday/Documents/WolframOmega/graphs/graph");
        fprintf(pfile, "%d", i);
        fprintf(pfile, ".png\">\n");
        fprintf(pfile,"<br><br><br><br>\n");
    }
    fprintf(pfile, "</body>\n");
    fclose(pfile);
    
}