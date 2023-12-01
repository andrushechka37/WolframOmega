#pragma once
#include "tree.h"


#define int_node_ctor(value) node_ctor(value, value_t, NULL, NULL, NULL)
#define _ADD(left, right) node_ctor(OP_ADD, operator_t, left, right, NULL)
#define _SUB(left, right) node_ctor(OP_SUB, operator_t, left, right, NULL)
#define _MUL(left, right) node_ctor(OP_MUL, operator_t, left, right, NULL)
#define _DIV(left, right) node_ctor(OP_DIV, operator_t, left, right, NULL)
#define _POW(left, right) node_ctor(OP_POW, operator_t, left, right, NULL)

#define _SIN node_ctor(OP_SIN, operator_t, NULL, _COPY_R, NULL)
#define _COS node_ctor(OP_COS, operator_t, NULL, _COPY_R, NULL)


#define _COPY_L copy_node(element->left)
#define _COPY_R copy_node(element->right)
#define _DIF_L left_node
#define _DIF_R right_node

diff_tree_element * diff(diff_tree_element * element);
diff_tree_element * copy_node(diff_tree_element * original);