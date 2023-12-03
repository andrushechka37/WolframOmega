#pragma once
#include "tree.h"


#define int_node_ctor(value) node_ctor(value, value_t, NULL, NULL, NULL)

#define ADD(left, right) node_ctor(OP_ADD, operator_t, left, right, NULL)
#define SUB(left, right) node_ctor(OP_SUB, operator_t, left, right, NULL)
#define MUL(left, right) node_ctor(OP_MUL, operator_t, left, right, NULL)
#define DIV(left, right) node_ctor(OP_DIV, operator_t, left, right, NULL)
#define POW(left, right) node_ctor(OP_POW, operator_t, left, right, NULL)

#define SIN node_ctor(OP_SIN, operator_t, NULL, COPY_R, NULL)
#define COS node_ctor(OP_COS, operator_t, NULL, COPY_R, NULL)

#define COPY_L copy_node(element->left)
#define COPY_R copy_node(element->right)

#define DIF_L left_node
#define DIF_R right_node

diff_tree_element * tree_diff(diff_tree_element * element);
diff_tree_element * copy_node(diff_tree_element * original);

void consts_eval(diff_tree_element * element);
void delete_fictive_nodes(diff_tree_element * element);
void tree_simplifie(diff_tree_element * element);