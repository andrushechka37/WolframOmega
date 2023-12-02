#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "deff_dump.h"
#include <string.h>  // delete
#include <math.h>
#include "diff.h"


// caps define(done)
// sin/cos (done)


// simplify
// recursivni spuuuusk
// norn quanituty of args !!!!!!!!!!!!!!!!!
// error detection
static void set_parents(diff_tree_element * root, diff_tree_element * parent);

int main(void) {
    diff_tree tree = {};
    tree_ctor(&tree);
    read_data(&tree);
    tree_visualize(tree.root);
    diff_tree_element * tree2 = diff(tree.root);
    tree_dtor(&(tree.root));


    set_parents(tree2, tree2);
    //verify(tree2);
    //tree_visualize(tree2);
    // consts_eval(tree2);
    // delete_fictive_nodes(tree2);
    print_tex(tree2);
} 

diff_tree_element * diff(diff_tree_element * element) {
    if (element->type == value_t) {
        return int_node_ctor(0);
    } else if (element->type == variable_t) {
        return int_node_ctor(1);
    }

    diff_tree_element * left_node = NULL;
    if (ELEM_OP_ARG == 2) {
        left_node = diff(element->left);
    }

    diff_tree_element * right_node = diff(element->right);



    switch (ELEM_OP_NUM)
    {
    case OP_ADD:
        return ADD(DIF_L, DIF_R);
    case OP_SUB:                
        return SUB(DIF_L, DIF_R);
    case OP_MUL:
        return ADD(MUL(DIF_L, COPY_R),MUL(DIF_R, COPY_L));
    case OP_DIV:
        return DIV(SUB(MUL(DIF_L, COPY_R),MUL(DIF_R, COPY_L)), POW(COPY_R, int_node_ctor(2)));
    case OP_SIN:
        return MUL(COS, DIF_R);
    case OP_COS:
        return MUL(MUL(int_node_ctor(-1), SIN), DIF_R);
    case OP_POW:
        return MUL(MUL(COPY_R, POW(COPY_L, int_node_ctor(element->right->value.number - 1))), DIF_L);
    
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

diff_tree_element * copy_node(diff_tree_element * element) {
    if (element == NULL) {     
        return NULL;
    }
    diff_tree_element * left =  copy_node(element->left);
    diff_tree_element * right = copy_node(element->right);
    if (element->type == operator_t) {
        return node_ctor(ELEM_OP_NUM, (types_of_node)element->type,left, right, NULL);
    }
    return node_ctor(element->value.number, (types_of_node)element->type,left, right, NULL);
}


void consts_eval(diff_tree_element * element) {
    if (element == NULL) {     
        return;
    }
    consts_eval(element->left);
    consts_eval(element->right);
    if (element->type == operator_t) {
        if (element->left->type == value_t && element->right->type == value_t) {
            
            double calc = tree_eval(element, 0);
            if (element->left != NULL) {
                tree_dtor(&(element->left));
            }
            if (element->right != NULL) {
                tree_dtor(&(element->right));
            }
            element->type = value_t;
            element->value.number = calc;
        }
    }
    return; 
}
// // otzu 
// void delete_fictive_nodes(diff_tree_element * element) {// optimise simplify eval
//     if (element == NULL) {     
//         return;
//     }
//     delete_fictive_nodes(element->left);
//     delete_fictive_nodes(element->right);
//     if (element->type == operator_t) {
//         if ((int)element->value == OP_MUL || (int)element->value == OP_MUL) {
//             if (element->left->value == 0 || element->left->value == 0) {
//                 tree_dtor(&(element->left));
//                 tree_dtor(&(element->right));
//                 element->type = value_t;
//                 element->value = 0;
//                 return;
//             }
//             if (element->left->value == 1) {
//                 if (element == element->parent->left) {
//                     element->parent->left = COPY_R;
//                 } else {
//                     element->parent->right = COPY_R;
//                 }
//                 element->right->parent = element->parent;
//                 element->left = NULL;
//                 element->right = NULL;
//                 tree_dtor(&element);
//             }
//             if (element->right->value == 1) {
//                 if (element == element->parent->left) {
//                     element->parent->left = element->left;
//                 } else {
//                     element->parent->right = element->left;
//                 }
//                 element->left->parent = element->parent;
//                 element->left = NULL;
//                 element->right = NULL;
//                 tree_dtor(&element);
//             }
//         } else if ((int)element->value == OP_ADD || (int)element->value == OP_SUB) {
//             if (element->left->value == 0) {
//                 if (element->parent->left == element) {
//                     element->parent->left = COPY_R;
//                 } else {
//                     element->parent->right = COPY_R;
//                 }
//                 tree_dtor(&element);
//             }
//             if (element->right->value == 0) {
//                 if (element->parent->left == element) {
//                     element->parent->left = COPY_L;
//                 } else {
//                     element->parent->right = COPY_L;
//                 }
//                 tree_dtor(&element);
//             }
//         }
//     }
//     return;
// }




