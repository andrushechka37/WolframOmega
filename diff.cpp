#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "deff_dump.h"
#include <string.h>  // delete
#include <math.h>
#include "diff.h"

bool is_change = 1;
// simplify
// recursivni spuuuusk
// error detection
static void set_parents(diff_tree_element * root, diff_tree_element * parent);

int main(void) {
    diff_tree tree = {};
    tree_ctor(&tree);
    read_data(&tree);
    //tree_visualize(tree.root);
    diff_tree_element * tree2 = tree_diff(tree.root);
    tree_dtor(&(tree.root));


    set_parents(tree2, tree2);
    //verify(tree2);
    tree_visualize(tree2);
    tree_simplifie(tree2);
    tree_visualize(tree2);
    print_tex(tree2);
} 

diff_tree_element * tree_diff(diff_tree_element * element) {
    if (element->type == value_t) {
        return int_node_ctor(0);
    } else if (element->type == variable_t) {
        return int_node_ctor(1);
    }

    diff_tree_element * left_node = NULL;
    if (ELEM_OP_ARG == 2) {
        left_node = tree_diff(element->left);
    }

    diff_tree_element * right_node = tree_diff(element->right);



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
    if (element->type == operator_t && ELEM_OP_ARG == 2) {
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
            is_change = 1;
        }
    }
    return; 
}

void single_node_dtor(elem_ptr * element) {
    if (*element == NULL) {
        return;
    }
    if ((*element)->left != NULL) {
        (*element)->left = NULL;
    }
    if ((*element)->right != NULL) {
        (*element)->right = NULL;
    }
    (*element)->type = 0;
    (*element)->value.operetor.arg_quantity = 0;
    (*element)->value.operetor.op_number = (operations)0;
    (*element)->value.number = 0;
    free((*element));
    (*element) = NULL;
    return;
}

void throw_away_node(diff_tree_element * element, char junk_side) {
    if (junk_side == 'R') {
        tree_dtor(&(element->right));
        element->type = element->left->type;
        element->value = element->left->value;
        element->right = element->left->right;
        diff_tree_element * left = element->left;
        element->left = element->left->left;
        single_node_dtor(&(left));
    } else if (junk_side == 'L') {
        tree_dtor(&(element->left));
        element->type = element->right->type;
        element->value = element->right->value;
        element->left = element->right->left;
        diff_tree_element * right = element->right;
        element->right = element->right->right;
        single_node_dtor(&(right));
    } else {
        printf("there is wrong junk side chosen");
    }
    is_change = 1;
    return;
}

#define left_type element->left->type
#define right_type element->right->type
#define left_number element->left->value.number
#define right_number element->right->value.number

void delete_fictive_nodes(diff_tree_element * element) {
    if (element == NULL) {     
        return;
    }
    delete_fictive_nodes(element->left);
    delete_fictive_nodes(element->right);
    if (element->type == operator_t) {
        if (ELEM_OP_NUM == OP_MUL || ELEM_OP_NUM == OP_DIV) {
            if ((left_number == 0  && left_type == value_t) || 
                (right_number == 0 && right_type == value_t)) {
                tree_dtor(&(element->left));
                tree_dtor(&(element->right));
                element->type = value_t;
                element->value.number = 0;
                is_change = 1;
                return;
            } else if (left_number == 1 && left_type == value_t) { // switch
                throw_away_node(element, 'L');
            } else if (right_number == 1 && right_type == value_t) {
                throw_away_node(element, 'R');
            }
        } else if (ELEM_OP_NUM == OP_ADD || ELEM_OP_NUM == OP_SUB) {
            if (left_number == 0 && left_type == value_t) {
                throw_away_node(element, 'L');
            } else if (right_number == 0 && right_type == value_t) {
                throw_away_node(element, 'R');
            }
        }
    }
    return;
}

#undef left_type
#undef right_type
#undef left_number
#undef right_number

void tree_simplifie(diff_tree_element * element) {
    while (is_change == 1) {
    is_change = 0;
    consts_eval(element);
    delete_fictive_nodes(element);
    }
    return;
}