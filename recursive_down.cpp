#include <stdlib.h>
#include <stdio.h>
#include "deff_dump.h"
#include <string.h>
#include "tree.h"
#include "deff_dump.h"
#include <math.h>
#include "diff.h"
#include "recursive_down.h"

static diff_tree_element * get_subexpression();

static diff_tree_element * get_number() {
    int value = 0;
    while ('0' <= str[ip] && str[ip] <= '9') {
        value = value * 10 + str[ip] - '0';
        ip++;
    }
    return int_node_ctor(value);
}

static diff_tree_element * get_variable() {
    if (str[ip] == 'x') {
        ip++;
        return node_ctor(0, variable_t, NULL, NULL, NULL);
    } else {
        return get_number();
    }
}
static diff_tree_element * get_long_op() {
    if(('a' <= str[ip] && str[ip] <= 'z' && str[ip] != 'x')) {
        char op[op_name_len] = "";
        int i = 0;
        while ('a' <= str[ip] && str[ip] <= 'z') {
            op[i] = str[ip];
            i++;
            ip++;
        }
        if (strcmp(op, "sin") == 0) {
            ip++; // chech bracket
            diff_tree_element * sin = SIN(get_subexpression());
            ip++;// chech bracket
            return sin;
        } else if (strcmp(op, "cos") == 0) {
            ip++; // chech bracket
            diff_tree_element * cos = COS(get_subexpression());
            ip++;// che
            return cos;
        }
    } else {
        return get_variable();
    }
}

static diff_tree_element * get_bracket() {
    if (str[ip] == '(') {
        ip++;
        diff_tree_element * value = get_subexpression();
        if (str[ip] == ')') {
            ip++;
        } else {
            printf("bracket trouble\n");
        }
        return value;
    } else {
        return get_long_op();
    }
}
static diff_tree_element * get_pow() {
    diff_tree_element * value = get_bracket();
    while (str[ip] == '^') {
        char op = str[ip++];
        diff_tree_element * value2 = get_bracket();
        value =  POW(value, value2);
    }
    return value;
}

static diff_tree_element * get_add_or_sub() {   
    diff_tree_element * value = get_pow();
    while (str[ip] == '*' || str[ip] == '/') {
        char op = str[ip++];
        diff_tree_element * value2 = get_pow();
        switch (op)
        {
        case '*':
            value =  MUL(value, value2);
            break;
        case '/':
            value = DIV(value, value2);
            break;
        default:
            printf("errrrror\n");
            break;
        }
    }
    return value;
}

static diff_tree_element * get_subexpression() {
    diff_tree_element * value = get_add_or_sub();
    while (str[ip] == '+' || str[ip] == '-') {
        char op = str[ip++];
        diff_tree_element * value2 = get_add_or_sub();
        switch (op)
        {
        case '+':
            value = ADD(value, value2);
            break;
        case '-':
            value = SUB(value, value2);
            break;
        default:
            printf("errrrror");
            break;
        }
    }
    return value;
}

diff_tree_element * get_expression() {
    diff_tree_element * value = get_subexpression();
    if (str[ip] == '\0') {
        return value;
    } else {
        printf("syntax eroooooor");
    }
}



















