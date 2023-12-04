#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "diff.h"
#include "recursive_down.h"
#include "deff_dump.h"

static bool is_change = 1;

// error detection
static void set_parents(diff_tree_element * root, diff_tree_element * parent);

int main(void) {
    diff_tree_element * tree = get_expression();
    set_parents(tree, tree);
    print_tex(tree);
    tree_visualize(tree);
    diff_tree_element * tree2 = tree_diff(tree);
    tree_dtor(&tree);
    set_parents(tree2, tree2);
    verify(tree2);
    print_tex(tree2, "not_opt.md");
    tree_simplify(tree2);
    tree_visualize(tree2);
    print_tex(tree2);
} 

diff_tree_element * tree_diff(diff_tree_element * element) {
    if (element->type == value_t) {
        return NUMBER_NODE(0);
    } else if (element->type == variable_t) {
        return NUMBER_NODE(1);
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
        return DIV(SUB(MUL(DIF_L, COPY_R),MUL(DIF_R, COPY_L)), POW(COPY_R, NUMBER_NODE(2)));
    case OP_SIN:
        return MUL(COS(COPY_R), DIF_R);
    case OP_COS:
        return MUL(MUL(NUMBER_NODE(-1), SIN(COPY_R)), DIF_R);
    case OP_POW:
        return MUL(MUL(COPY_R, POW(COPY_L, NUMBER_NODE(element->right->value.number - 1))), DIF_L);
    
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

#define LEFT_CHILD (*element)->left
#define RIGHT_CHILD (*element)->left

void single_node_dtor(elem_ptr * element) {
    if (*element == NULL) {
        return;
    }
    if (LEFT_CHILD != NULL) {
        LEFT_CHILD = NULL;
    }
    if (RIGHT_CHILD != NULL) {
        RIGHT_CHILD = NULL;
    }
    (*element)->type = zero_t; // enum 
    (*element)->value.operator_info.arg_quantity = 0;
    (*element)->value.operator_info.op_number = (operations)0;
    (*element)->value.number = 0;
    free((*element));
    (*element) = NULL;
    return;
}

#undef LEFT_CHILD
#undef RIGHT_CHILD

#define LEFT_CHILD element->left
#define RIGHT_CHILD element->right

void throw_away_node(diff_tree_element * element, char junk_side) {
    if (junk_side == 'R') {
        tree_dtor(&(RIGHT_CHILD)); // define maybe
        element->type = LEFT_CHILD->type;
        element->value = LEFT_CHILD->value;
        RIGHT_CHILD = LEFT_CHILD->right;
        diff_tree_element * left =LEFT_CHILD;
        LEFT_CHILD = LEFT_CHILD->left;
        single_node_dtor(&(left));
    } else if (junk_side == 'L') {
        tree_dtor(&(LEFT_CHILD));
        element->type = RIGHT_CHILD->type;
        element->value = RIGHT_CHILD->value;
        LEFT_CHILD = RIGHT_CHILD->left;
        diff_tree_element * right = RIGHT_CHILD;
        RIGHT_CHILD = RIGHT_CHILD->right;
        single_node_dtor(&(right));
    } else {
        printf("there is wrong junk side chosen");
    }
    is_change = 1;
    return;
}

#undef LEFT_CHILD
#undef RIGHT_CHILD

#define LEFT_TYPE element->left->type
#define RIGHT_TYPE element->right->type
#define LEFT_NUMBER element->left->value.number
#define RIGHT_NUMBER element->right->value.number

void delete_fictive_nodes(diff_tree_element * element) {
    if (element == NULL) {     
        return;
    }
    delete_fictive_nodes(element->left);
    delete_fictive_nodes(element->right);
    if (element->type == operator_t) {
        if (ELEM_OP_NUM == OP_MUL || ELEM_OP_NUM == OP_DIV) {   //*0  0/9989
            if ((LEFT_NUMBER == 0  && LEFT_TYPE == value_t) || 
                (RIGHT_NUMBER == 0 && RIGHT_TYPE == value_t)) {
                tree_dtor(&(element->left));
                tree_dtor(&(element->right));
                element->type = value_t;
                element->value.number = 0;
                is_change = 1;
                return;
            } else if (LEFT_NUMBER == 1 && LEFT_TYPE == value_t) { // switch   // *1
                throw_away_node(element, 'L');
            } else if (RIGHT_NUMBER == 1 && RIGHT_TYPE == value_t) {    // *1
                throw_away_node(element, 'R');
            }
        } else if (ELEM_OP_NUM == OP_ADD || ELEM_OP_NUM == OP_SUB) { // +- 0
            if (LEFT_NUMBER == 0 && LEFT_TYPE == value_t) {
                throw_away_node(element, 'L');
            } else if (RIGHT_NUMBER == 0 && RIGHT_TYPE == value_t) {
                throw_away_node(element, 'R');
            }
        } else if (ELEM_OP_NUM == OP_POW) {
            if (LEFT_NUMBER == 1 && LEFT_TYPE == value_t) { // ^1
                throw_away_node(element, 'L');
            } else if (RIGHT_NUMBER == 1 && RIGHT_TYPE == value_t) { //   ^1
                throw_away_node(element, 'R');
            }
        }
    }
    return;
}

#undef LEFT_TYPE
#undef RIGHT_TYPE
#undef LEFT_NUMBER
#undef RIGHT_NUMBER

void tree_simplify(diff_tree_element * element) {
    while (is_change == 1) {
        is_change = 0;
        consts_eval(element);
        delete_fictive_nodes(element);
    }
    return;
}
