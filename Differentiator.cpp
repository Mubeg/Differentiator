#include "Differentiator.h"

#undef  TEST_LEVEL
#define TEST_LEVEL 0

int main(){

	std::setlocale(LC_ALL, "ru_RU.UTF-8");

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

	Node_t *node_done = differentiate_to_new(node);
	node_set_parents(node_done);

	tex_init(TEX_FILE);

	log_to_tex_file(node, TEX_FILE, "Do differentiation by x:");

	while(node_tree_optimize(node_done)){node_set_parents(node_done); log_to_tex_file(node_done, TEX_FILE);};

	log_to_tex_file(node_done, TEX_FILE, "Result:");

	tex_process(TEX_FILE_SUB);
	
	node_set_parents(node_done);

	dot_node(node,      IMAGE_OUT_2);
	dot_node(node_done, IMAGE_OUT  );

	FILE * file = fopen("myfile.txt", "w");
	if(file != nullptr){
		node_write_to_file_less_brackets(node_done, file);
	}
	fclose(file);
	
	node_deinit(node);
	node_deinit(node_done);

	return 0;
}

void tex_init(const char filename[]){

	FILE * file = fopen(filename, "w");	
	if(file == nullptr){
		fprintf(stderr, "Unable to open file %s\n", filename);
		return;
	}

	fprintf(file, "\\documentclass[a4paper]{article}\n\\begin{document}\n");

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

void log_to_tex_file(Node_t * root, const char filename[], const char proc[] /*= "Очевидное преобразование:"*/){
	
	FILE * file = fopen(filename, "a");	
	if(file == nullptr){
		fprintf(stderr, "Unable to open file %s\n", filename);
		return;
	}
	fprintf(file, "%s $$", proc);

	tex_node_tree(root, file);

	fprintf(file, "$$");

	fclose(file);
		
	return;
}

void tex_node_tree(Node_t * node, FILE * file){
	
	if(	THIS->mode == MODE_CNST){
		fprintf(file, NODE_ELEM_PRINT_VALUE, DATA);
	}
	else if(THIS->mode == MODE_VARL){
		fprintf(file, NODE_ELEM_PRINT, DATA);
	}
	else if(THIS->mode == MODE_FUNC){

		int first_priority = MIN_PRIORITY;
		int second_priority = MAX_PRIORITY;

		#define DEF_TEX(name, equal, prior, command)	else if(PARENT->data == equal){\
									first_priority = prior;\
								}
		if(PARENT == nullptr || PARENT->mode != MODE_FUNC){
			0;
		}
		#include"tex_commands.h"
		else{
			assert(("unexpected situation\n", false) == true);
		}
		#undef DEF_TEX

		#define DEF_TEX(name, equal, prior, command)	else if(DATA == equal){\
									second_priority = prior;\
								}
		if(THIS->mode != MODE_FUNC){
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
	else{
		fprintf(stderr, "Unknown mode: %d\n", THIS->mode);
		return;
	}

}

Node_t * differentiate_to_new(Node_t * node){

	Node_t * root = do_diff_recursive(node, 'x');
	return root;
}

Node_t * do_diff_recursive(Node_t * node, Node_elem_t cur_diff /* = -1*/){

	if(node == nullptr){
		return nullptr;
	}
	
	#define DEF_DIF(name, _mode, equal, command, do) \
							else if((node->mode == _mode) && (node->mode != MODE_FUNC || equal == DATA)){\
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

	#define DEF_OPTIM(condition, do)	if(THIS->mode == MODE_FUNC && condition){\
							optimized = true;\
							do;\
						}
	#include"optims.h"
	#undef DEF_OPTIM

	#define DEF_DIF(name, _mode, equal, command, do)	else if(equal == DATA){\
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

	if((LEFT != nullptr && LEFT->mode != MODE_CNST) || RIGHT == nullptr || RIGHT->mode != MODE_CNST || THIS->mode != MODE_FUNC){
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



