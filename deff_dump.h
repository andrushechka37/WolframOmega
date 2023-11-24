#pragma once
#include "deff.h"
#define hard_visualize 0

void print_tree_inorder(deff_tree_element * root);
void tree_visualize(deff_tree_element * element);

inline int graph_number = 1;
const int command_len = 1000;
void create_new_graph(void);
void html_dump(void);