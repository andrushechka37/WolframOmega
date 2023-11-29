#pragma once
#include "tree.h"

#define hard_visualize 0

void print_tree_inorder(diff_tree_element * root);
void tree_visualize(diff_tree_element * element);
void create_new_graph(void);
void html_dump(void);

inline int graph_number = 1;
const int command_len = 1000;

