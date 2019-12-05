#pragma G++ diagnostic ignored "-Wwrite-strings"
#pragma G++ diagnostic ignored "-Wmultichar"

#include "includes/test-log.define"
#include "includes/node.h"
//#define USING_WCHAR_T
#include "includes/ionegin"


const char TEX_FILE[] 		= "tex_file.tex";
const char TEX_FILE_SUB[] 	= "tex_file";
const int ACCURACY 		= 1000;

const int MAX_PROC_NUM 		= 10;
char *procs[MAX_PROC_NUM]  	= {};
int PROCS_NUM 			= 0;

typedef struct Multi_symbol_token_t{
	
	int mode;
	Buff_elem_t *token;
	int size;

} List_elem_t;

#include "includes/List.h"

List_t * mstokens = nullptr;

#include "includes/RDSft.h"

#define DATA   node->data


void color_node(Node_t * node, FILE * file);


void tex_init(const char filename[]);


void tex_process(const char filename[]);


void log_to_tex_file(Node_t * root, const char filename[], const char proc[] = "Obvious transformation:", bool is_first = false, int var_diff = 'x', bool is_end = false);


void tex_node_tree(Node_t * node, FILE * file);


Node_t * differentiate_to_new(Node_t * node);


Node_t * do_diff_recursive(Node_t * node, Node_elem_t cur_diff = -1);


bool node_tree_optimize(Node_t * node);


Node_t * node_make_from_file_RDS(FILE * file);

