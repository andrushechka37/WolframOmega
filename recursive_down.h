#pragma once 
#include "tree.h"

inline int ip = 0;
inline const char * str = "sin(cos(x^2))";


diff_tree_element * get_expression();
inline FILE * plog = NULL;

#define print_log(text, spec) fprintf(plog, text, spec)
