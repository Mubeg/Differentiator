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

	while(node_tree_optimize(node_done)){dot_node(node_done, IMAGE_OUT);};
	
	node_set_parents(node_done);

	dot_node(node,      IMAGE_OUT_2);
	dot_node(node_done, IMAGE_OUT  );

	FILE * file = fopen("myfile.txt", "w");
	if(file != nullptr){
		node_write_to_file_less_brackets(node_done, file);
	}
	fclose(file);
	//Node_tex(node_done);
	
	node_deinit(node);
	node_deinit(node_done);

	return 0;
}

//Node_t * tex_node_tree(Node_t * node, FILE * file){
	
	

//}

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



