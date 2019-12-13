#pragma G++ diagnostic ignored "-Wwrite-strings"
#pragma G++ diagnostic ignored "-Wmultichar"
#include "Differentiator.h"

#undef  TEST_LEVEL
#define TEST_LEVEL 0

int main(){

	//printf("%d %f\n", 2, log(((float) 2000)/ACCURACY)/log(2.718));

	const int LINE_ 		= __LINE__;
	procs[__LINE__ - LINE_ - 1] 	= "Очевидное преобразование:";
	procs[__LINE__ - LINE_ - 1] 	= "Нетрудно увидеть, что:";
	procs[__LINE__ - LINE_ - 1] 	= "При помощи простейших действий получем:";
	procs[__LINE__ - LINE_ - 1]	= "Упростим:";
	PROCS_NUM 			= __LINE__ - LINE_ - 1;

	std::setlocale(LC_ALL, "ru_RU.UTF-8");

	List_t mstoken_list = {};
	mstokens = &mstoken_list;

	LIST_INIT(mstokens);

	list_add_head(mstokens);

	Multi_symbol_token_t temp = {};	

	#define DEF_DIF(name, mode, equiv, code, do) \
	if(mode == MODE_FUNC){\
		temp.token = #name;\
		temp.size = strlen(temp.token);\
		list_add_after(mstokens, temp, mstokens->head);\
	}

	#include"commands.h"
	#undef DEF_DIF

	FILE * tree_file = fopen(TREE_FILE, "r");
	if(tree_file == nullptr){
		fprintf(stderr, "Unable to open file %s\n", TREE_FILE);
		return 1;
	}

	Node_t *node = node_make_from_file_RDS(tree_file);
	if(node == nullptr){
		fprintf(stderr, "Unable to make node from %s\n", TREE_FILE);
		return 1;
	}

	fclose(tree_file);

	node_set_parents(node);
	dot_node(node, IMAGE_OUT_2);

	Node_t *node_done = differentiate_to_new(node);
	node_set_parents(node_done);

	tex_init(TEX_FILE);

	log_to_tex_file(node, TEX_FILE, "", true);
	log_to_tex_file(node_done, TEX_FILE, "Продиффиренциреум:");

	std::srand(unsigned(time(0)));

	for(int i = 0; node_tree_optimize(node_done); i++){node_set_parents(node_done); 
							   log_to_tex_file(node_done, TEX_FILE, procs[std::rand() % PROCS_NUM]);};

	log_to_tex_file(node_done, TEX_FILE, "В результате:", false, DIFF_VAR, true);

	tex_process(TEX_FILE_SUB);
	
	node_set_parents(node_done);

	dot_node(node_done, IMAGE_OUT  );
	
	node_deinit(node);
	node_deinit(node_done);

	return 0;
}

void color_node(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;	
	}

	char color[MAX_COMMAND_LEN] = {};
	sprintf(color, "color = \"%s\"", THIS->mode == MODE_OPER ? "yellow"  :
					(THIS->mode == MODE_VARL ? "red"     :
					(THIS->mode == MODE_CNST ? "green"   :
					(THIS->mode == MODE_FUNC ? "blue"    : "black"))));

	if(THIS->mode == MODE_CNST)
		fprintf(file, "%d [%s; shape = record; label =\"" NODE_ELEM_PRINT_VALUE    "\"]\n", THIS, color, (float) DATA / ACCURACY);
	else if(THIS->mode == MODE_FUNC)
		fprintf(file, "%d [%s; shape = record; label =\"" NODE_ELEM_PRINT_FUNCTION "\"]\n", THIS, color, list_get(mstokens, DATA).token);
	else
		fprintf(file, "%d [%s; shape = record; label =\"" NODE_ELEM_PRINT          "\"]\n", THIS, color, DATA);

	color_node(LEFT, file);
	color_node(RIGHT, file);

}

void tex_init(const char filename[]){

	FILE * file = fopen(filename, "w");	
	if(file == nullptr){
		fprintf(stderr, "Unable to open file %s\n", filename);
		return;
	}

	fprintf(file, "\\documentclass{article}\n\\usepackage[utf8]{inputenc}\n\\usepackage[english,russian]{babel}\n\\usepackage{setspace, amsmath}\n\\begin{document}\n");

	fclose(file);
}

void tex_process(const char filename[]){

	char temp[strlen(filename) + 4] = {};
	sprintf(temp, "%s.tex", filename);	

	FILE * file = fopen(temp, "a");
	if(file == nullptr){
		fprintf(stderr, "Unable to open file %s\n", temp);
		return;
	}

	fprintf(file, "\\end{document}\n");

	fclose(file);
	
	char command[MAX_LINE_LEN] = {};

	sprintf(command, "pdflatex %s.tex > tempfile\n", filename);
	std::system(command);

	sprintf(command, "rm tempfile %s.tex %s.aux %s.log\n", filename, filename, filename);
	std::system(command);
}

void log_to_tex_file(Node_t * root, const char filename[], const char proc[] /*= "Очевидное преобразование:"*/, bool is_first/* = false*/, int var_diff /*= 'x'*/, bool is_end /*= false*/){
	
	FILE * file = fopen(filename, "a");	
	if(file == nullptr){
		fprintf(stderr, "Unable to open file %s\n", filename);
		return;
	}
	fprintf(file, "%s $$", proc);

	if(is_first){
		fprintf(file, "\\left( ");
	}
	if(!is_first){
		fprintf(file, "=");
	}

	tex_node_tree(root, file);

	if(is_first){
		fprintf(file, " \\right)'_%c", var_diff);
	}

	if(!is_end){
		fprintf(file, "=");
	}

	fprintf(file, " $$");

	fclose(file);
		
	return;
}

void tex_node_tree(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;
	}
	
	if(	THIS->mode == MODE_CNST){
		fprintf(file, NODE_ELEM_PRINT_VALUE, ((double) DATA) / ACCURACY);
	}
	else if(THIS->mode == MODE_VARL){
		fprintf(file, NODE_ELEM_PRINT, DATA);
	}
	else if(THIS->mode == MODE_OPER){

		int first_priority = MIN_PRIORITY;
		int second_priority = MAX_PRIORITY;

		#define DEF_TEX(name, equal, prior, command)	else if(PARENT->data == equal){\
									first_priority = prior;\
									if(PARENT->left == nullptr) first_priority = 0;\
								}
		if(PARENT == nullptr || PARENT->mode != MODE_OPER){
			0;
		}
		#include"tex_commands.h"
		else{
			assert(("unexpected situation\n", false) == true);
		}
		#undef DEF_TEX

		#define DEF_TEX(name, equal, prior, command)	else if(DATA == equal){\
									second_priority = prior;\
									if(LEFT == nullptr) second_priority = 0;\
								}
		if(THIS->mode != MODE_OPER){
			0;
		}
		#include"tex_commands.h"
		else{
			assert(("unexpected situation\n", false) == true);
		}
		#undef DEF_TEX

		switch(DATA){

		#define DEF_TEX(name, equal, prior, command) \
			case equal:\
				command;\
				break;			
			#include "tex_commands.h"
			default:
				fprintf(stderr, "Can't resolve func: " NODE_ELEM_PRINT "\n", DATA);
				return;
		}
		#undef DEF_TEX
	}
	else if(THIS->mode == MODE_FUNC){

			fprintf(file, NODE_ELEM_PRINT_FUNCTION, list_get(mstokens, DATA).token);
			fprintf(file, "%c", SEPARATOR_START);

			tex_node_tree(RIGHT, file);

			fprintf(file, "%c", SEPARATOR_END);
	}
	else{
		fprintf(stderr, "Unknown mode: %d\n", THIS->mode);
		return;
	}

}

Node_t * differentiate_to_new(Node_t * node){

	Node_t * root = do_diff_recursive(node, DIFF_VAR);
	return root;
}

Node_t * do_diff_recursive(Node_t * node, Node_elem_t cur_diff /* = -1*/){

	if(node == nullptr){
		return nullptr;
	}
	
	#define DEF_DIF(name, _mode, equal, command, do) \
							else if((node->mode == _mode) && (node->mode != MODE_OPER || equal == DATA)\
								&& (node->mode != MODE_FUNC || list_find_by_elem(mstokens, #name) == DATA)){\
								command;\
					      		}

	if(true == false){assert(true == false);}
	#include"commands.h"
	else{

		fprintf(stderr, "Not such command found " NODE_ELEM_PRINT "\n", DATA);
		return nullptr;
	}

	#undef DEF_DIF
}

bool node_tree_optimize(Node_t * node){


	if(node == nullptr){
		return false;
	}

	bool optimized = false;

	#define DEF_OPTIM(condition, do)	if(THIS->mode == MODE_OPER && LEFT != nullptr && condition){\
							optimized = true;\
							do;\
						}
	#include"optims.h"
	#undef DEF_OPTIM

	#define DEF_DIF(name, _mode, equal, command, do)	else if(equal == DATA || list_find_by_elem(mstokens, #name) == DATA){\
									optimized = true;\
									\
									THIS->mode = MODE_CNST;\
									do;\
									\
									node_deinit(LEFT);\
									node_deinit(RIGHT);\
									LEFT = nullptr;\
									RIGHT = nullptr;\
			      					}

	if(optimized || (LEFT != nullptr && LEFT->mode != MODE_CNST) || RIGHT == nullptr || RIGHT->mode != MODE_CNST ||
	   (THIS->mode != MODE_OPER && THIS->mode != MODE_FUNC)){
		0;
	}
	#include"commands.h"
	else{
		assert(("unexpected situation\n", false) == true);
	}
	#undef DEF_DIF


	bool temp1 = node_tree_optimize(LEFT );
	bool temp2 = node_tree_optimize(RIGHT);

	optimized = optimized || temp1 || temp2;

	return optimized;
}


Node_t * node_make_from_file_RDS(FILE * file){

	assert(file != nullptr);

	int buff_size = 0;
	Buff_elem_t * buff = read_file_to_created_buff_opened_file(file, &buff_size);

	int pc = 0;
	buff[buff_size - 2] = '\0';

	Node_t * root = Get_G(buff, &pc);
	free(buff);

	return root;
}



