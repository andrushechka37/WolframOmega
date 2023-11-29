#pragma once
#include "tree.h"


#define int_node_ctor(value) node_ctor(value, value_t, NULL, NULL, NULL)
#define _ADD(left, right) node_ctor(OP_ADD, operator_t, left, right, NULL)
#define _SUB(left, right) node_ctor(OP_SUB, operator_t, left, right, NULL)
#define _MUL(left, right) node_ctor(OP_MUL, operator_t, left, right, NULL)
#define _DIV(left, right) node_ctor(OP_DIV, operator_t, left, right, NULL)
#define _POW(left, right) node_ctor(OP_POW, operator_t, left, right, NULL)

#define _SIN node_ctor(OP_SIN, operator_t, NULL, element->right, NULL)
#define _COS node_ctor(OP_COS, operator_t, NULL, element->right, NULL)


#define _COPY_L node_ctor(element->left->value, (types_of_node)element->left->type, \
                  element->left->left, element->left->right, element->left->parent)
#define _COPY_R node_ctor(element->right->value, (types_of_node)element->right->type, \
                  element->right->left, element->right->right, element->right->parent)
#define _DIF_L left_node
#define _DIF_R right_node

diff_tree_element * diff(diff_tree_element * element);