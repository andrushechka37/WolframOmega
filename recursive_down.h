#pragma once 
#include "tree.h"

inline int ip = 0;
inline const char * str = "x^2+sin(x)/cos(x)-x^3";


diff_tree_element * get_expression();
inline FILE * plog = NULL;

#define print_log(text, spec) fprintf(plog, text, spec)
