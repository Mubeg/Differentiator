#include "includes/test-log.define"
#include "includes/node.h"
#include "includes/RDSft.h"
//#define USING_WCHAR_T
#include "includes/ionegin"


const char TEX_FILE[] = "tex_file.tex";
const char TEX_FILE_SUB[] = "tex_file";



void tex_init(const char filename[]);


void tex_process(const char filename[]);


void log_to_tex_file(Node_t * root, const char filename[], const char proc[] = "Obvious transformation:");


void tex_node_tree(Node_t * node, FILE * file);


Node_t * differentiate_to_new(Node_t * node);


Node_t * do_diff_recursive(Node_t * node, Node_elem_t cur_diff = -1);


bool node_tree_optimize(Node_t * node);


Node_t * node_make_from_file_RDS(FILE * file);

