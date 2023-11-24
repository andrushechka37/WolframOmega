#include <stdlib.h>
#include <stdio.h>
#include "deff.h"
#include "deff_dump.h"
#include <string.h>

// reader------------hard
// verificator
// tree calc
// variables
// no brackets
static bool check_symbol(char symbol, FILE * pfile);

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
    // tie_child_node(&(tree.root->left), 12, 1);
    // tie_child_node(&(tree.root->right), 21, 1);
    // tie_child_node(&(tree.root->left->left), 43, 2);
    // tie_child_node(&(tree.root->left->left->left), 41, 1);
    // tie_child_node(&(tree.root->left->right), 363, 1);
    read_data(&tree);
    print_tree_inorder(tree.root);

    //tree_visualize(tree.root);
    html_dump();
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






int read_node(elem_ptr * node, FILE * pfile) {
    if (check_symbol('(', pfile) == 1) {
        tie_child_node(node, 0, 0);
        read_node(&((*node)->left), pfile);

        double value = 0;

        if (fscanf(pfile, "%lf", &value) == 1) {
            (*node)->type = 1;
            (*node)->value = value;
        } else if (fscanf(pfile, "%c", (char *)&value) == 1) {
            (*node)->type = 2;
            (*node)->value = value;        // do new func for set type and value
        }

        read_node(&((*node)->right), pfile);
        inscect_symbol(')');
        tree_visualize(*node);

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
    read_node(&(tree->root), pfile);
    fclose(pfile);
    return 0;
}


    // char symbol = getc(pfile);

    // if (symbol != nil) {
    //     ungetc(symbol, pfile);
    // } else {
    //     return 0;
    // }

    // inscect_symbol('(');
    
    // read_node(&((*node)->left), pfile);

    // double value = 0;
    // if (fscanf(pfile, "%lf", &value) == 1) {
    //     tie_child_node(node, value, 1);
    // } else if (fscanf(pfile, "%c", &symbol) == 1) {
    //     tie_child_node(node, symbol, 2);
    // }

    // read_node(&((*node)->right), pfile);

    // inscect_symbol(')');
    // return 0;






















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

void tree_visualize(deff_tree_element * element) {
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


void create_new_graph(void) {  // TODO: temporary files, hardcode of path
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
    for (int i = 1; i < graph_number; i++) {
        fprintf(pfile, "<img src = \"/Users/anzhiday/Documents/WolframOmega/graphs/graph");
        fprintf(pfile, "%d", i);
        fprintf(pfile, ".png\">\n");
        fprintf(pfile,"<br><br><br><br>\n");
    }
    fclose(pfile);
    
}