#include "includes/test-log.define"
#include "includes/node.h"
#include "includes/RDSft.h"
//#define USING_WCHAR_T
#include "includes/ionegin"


const char TEX_FILE[] = "tex_file.tex";


Node_t * differentiate_to_new(Node_t * node);


Node_t * do_diff_recursive(Node_t * node, Node_elem_t cur_diff = -1);


bool node_tree_optimize(Node_t * node);


Node_t * node_make_from_file_RDS(FILE * file);

