#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "deff_dump.h"
#include <string.h>  // delete
#include <math.h>
#include "diff.h"


// caps define
static void set_parents(diff_tree_element * root, diff_tree_element * parent);
int main(void) {
    diff_tree tree = {};
    tree_ctor(&tree);
    read_data(&tree);

    tree_visualize(tree.root);
    
    diff_tree_element * tree2 = diff(tree.root);
    tree_dtor(&(tree.root));
    //tree_visualize(tree.root);
    set_parents(tree2, tree2);
    verify(tree2);
    tree_visualize(tree2);
    print_tex(tree2);
} 

diff_tree_element * diff(diff_tree_element * element) {
    if (element->type == value_t) {
        return node_ctor(0, value_t, NULL, NULL, NULL);
    } else if (element->type == variable_t) {
        return node_ctor(1, value_t, NULL, NULL, NULL);
    }

    diff_tree_element * left_node = NULL;
    if (OP_SQRT >= (int)element->value || (int)element->value >= OP_COS) {
        left_node = diff(element->left);
    }

    diff_tree_element * right_node = diff(element->right);



    switch ((int)element->value)
    {
    case OP_ADD:
        return _ADD(_DIF_L, _DIF_R);
    case OP_SUB:
        return _SUB(_DIF_L, _DIF_R);
    case OP_MUL:
        return _ADD(_MUL(_DIF_L, _COPY_R),_MUL(_DIF_R, _COPY_L));
    case OP_DIV:
        return _DIV(_SUB(_MUL(_DIF_L, _COPY_R),_MUL(_DIF_R, _COPY_L)), _POW(_COPY_R, int_node_ctor(2)));
    case OP_SIN:
        return _MUL(_COS, _DIF_R);
    case OP_COS:
        return _MUL(_MUL(int_node_ctor(-1), _SIN), _DIF_R);
    case OP_POW:
        return _MUL(_MUL(_COPY_R, _POW(_COPY_L, int_node_ctor(element->right->value - 1))), _DIF_L);
    
    default:
        printf("govno pizdec\n");
        break;
    }
}

static void set_parents(diff_tree_element * element, diff_tree_element * parent) {
    if (element == NULL) {     
        return;
    }
    set_parents(element->left, element);
    element->parent = parent;
    set_parents(element->right, element);
    return;
}

diff_tree_element * copy_node(diff_tree_element * original) {
    if (original == NULL) {     
        return NULL;
    }
    diff_tree_element * left =  copy_node(original->left);
    diff_tree_element * right = copy_node(original->right);
    return node_ctor(original->value, (types_of_node)original->type,left, right, NULL); // troubles
}








