#include "Differentiator.h"

#undef  TEST_LEVEL
#define TEST_LEVEL 0

int main(){

	std::setlocale(LC_ALL, "ru_RU.UTF-8");

	Node_t *node = node_make_from_file(TREE_FILE);

	Node_t *node_done = differentiate_to_new(node);
	
	while(node_tree_optimize(node_done));
	
	dot_node(node_done, IMAGE_OUT);
	dot_node(node, IMAGE_OUT_2);

	//Node_tex(node_done);
	
	node_deinit(node);
	node_deinit(node_done);

	return 0;
}


Node_t * differentiate_to_new(Node_t * node){

	Node_t * root = do_diff_recursive(node);
	return root;
}

Node_t * do_diff_recursive(Node_t * node){

	if(node == nullptr){
		return nullptr;
	}
	
	#define DEF_DIF(name, _mode, equal, command) 	else if(node->mode == _mode || (equal == DATA && node->mode == MODE_FUNC)){\
								command;\
					      		}

	if(true == false){assert(true == false);}
	#include"commands.h"
	else{

		fprintf(stderr, "Not such command found " ELEM_PRINT "\n", DATA);
		return nullptr;
	}

	#undef DEF_DIF
}

Node_t * node_copy(Node_t * node){

	if(node == nullptr){
		return nullptr;
	}

	return node_create_new(node->mode, node->data, node_copy(node->left), node_copy(node->right), node->parent);
	
}

Node_t * node_create_new(const int mode, const Elem_t elem, Node_t * left, Node_t * right, Node_t * parent /* = nullptr */){
	
	Node_t * new_node = nullptr;

	new_node = (Node_t *) calloc(1, sizeof(Node_t));
	NODE_INIT(new_node, nullptr);
	node_set(new_node, elem, mode);

	new_node->parent = parent;
	new_node->left  = left;
	new_node->right = right;

	return new_node;

}

bool node_tree_optimize(Node_t * node){
	return false;
}


void node_write_to_file(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;
	}

	node_assert(check_nullptr(THIS));

	node_write_to_file(LEFT, file);

	fprintf(file, ELEM_PRINT, SEPARATOR_START);
	fprintf(file, "\"" ELEM_PRINT "\"", DATA);

	node_write_to_file(RIGHT, file);

	fprintf(file, ELEM_PRINT, SEPARATOR_END);

}

Node_t * node_make_from_file(const char file_name[]){

	int buff_size = 0;
	Buff_elem_t * buff = read_file_to_created_buff(file_name, &buff_size);

	int pc = 0;
	Node_t * root = node_make_from_buff(nullptr, buff, buff_size, &pc);
	free(buff);
	return root;
}

Node_t * node_make_from_buff(Node_t * node, Buff_elem_t * buff, const int buff_size, int *pc){

	if(*pc >= buff_size){
		return nullptr;
	}

	if(buff[PC_N] == SEPARATOR_START){

		NODE_ADD(THIS, 0);
		PC_INCR;

	}
	else{
		return nullptr;
	}



	LEFT  = node_make_from_buff(THIS, buff, buff_size, pc);


	str string = {};
	if(!get_data(buff, buff_size, pc, &string)){
		return nullptr;
	}	
	int mode = get_mode(&string);

	node_set(THIS, string.str[0], mode);


	RIGHT = node_make_from_buff(THIS, buff, buff_size, pc);



	THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

	if(buff[PC_N] == SEPARATOR_END){
		PC_INCR;
		return node;
	}

	return nullptr;
}

int get_mode(str_ptr string){
	
	if(string == nullptr || string->size <= 0){
		return -1;
	}

#define DEF_DIF(name, _mode, equal, code) 	else if(equal == string->str[0] == 0){\
							return MODE_FUNC;\
						} 
	
	if(is_digits(string)){
		return MODE_CNST;
	}
	#include"commands.h"
	else{
		return MODE_VARL;
	}

#undef DEF_DIF
	
	return -1;
	
}

bool get_data(Buff_elem_t * buff, const int buff_size, int *pc, str_ptr string){

	assert(string != nullptr);

	Buff_elem_t temp_buff[MAX_LINE_LEN] = {};
	int temp = 0;
	int ans = sscanf(buff + *pc, Long_string("%[^()]%n"), temp_buff, &temp);

	if(ans == 0){
		return false;
	}
	*pc += temp;

	string->str = temp_buff;
	
	//printf("1: [%p] %ls %d\n", str->str, str->str, mode);
	string->size = strlen(string->str);
	//int mode = get_mode(str);	
	//printf("%d\n", get_mode(str));

	return true;

}


bool node_init(Node_t *node, const char name[], Node_t *parent){

	node_assert(check_nullptr(THIS));

	THIS->canary1 = CANARY_VALUE;

	LEFT   = nullptr;
	RIGHT  = nullptr;
	PARENT = parent;
	
	THIS->level = parent == nullptr ? 0 : parent->level + 1;

	THIS->errnum = 0;

	THIS->mode = -1;

	THIS->name = name[0] == '&' ? name + 1 : name;

	THIS->canary2 = CANARY_VALUE;

	THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

	node_assert(check_node(THIS, __LOCATION__));
 
	return true;
}

Node_t *node_add_right(Node_t *node, Elem_t elem){

	node_assert(check_node(THIS, __LOCATION__));

	Node_t * new_node = nullptr;
	NODE_ADD(RIGHT, elem);
	
	node_assert(check_node(THIS, __LOCATION__));

	return new_node;
}

Node_t *node_add_left(Node_t *node, Elem_t elem){

	node_assert(check_node(THIS, __LOCATION__));

	Node_t * new_node = nullptr;
	NODE_ADD(LEFT, elem);
	
	node_assert(check_node(THIS, __LOCATION__));

	return new_node;
}

Node_t * node_find(Node_t *node, str_ptr cmp_elem){

	if(cmp_elem == nullptr || cmp_elem->size < 0){
		return nullptr;
	}
	
	if(THIS == nullptr){
		return nullptr;	
	}

	if(cmp_elem->size > MAX_LINE_LEN){	
		cmp_elem->str[MAX_LINE_LEN] = '\0';
		cmp_elem->size = MAX_LINE_LEN;
	}

	if(DATA, cmp_elem->str[0] == 0){
		return THIS;
	}

	node_assert(check_node(THIS, __LOCATION__));

	Node_t * ans = node_find(LEFT, cmp_elem);
	if(ans != nullptr){
		return ans;	
	}

	ans = node_find(RIGHT, cmp_elem);
	
	return ans;
}

bool node_deinit(Node_t *node){

	if(THIS == nullptr){
		return true;
	}

	node_assert(check_node(THIS, __LOCATION__));

	node_deinit(LEFT);
	node_deinit(RIGHT);
	
	zero_data(THIS, 0, sizeof(Node_t));
	free(THIS);

	return true;
}


Elem_t node_get(Node_t *node){

	node_assert(check_node(THIS, __LOCATION__));
	
	return DATA;
}


bool node_set(Node_t *node, Elem_t elem, const int mode){

	//printf("%d %d\n", elem->str[0], elem->str[1]);

	node_assert(check_node(THIS, __LOCATION__));

	node->mode = mode;

	DATA = elem;

	THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

	node_assert(check_node(THIS, __LOCATION__));

	return true;
}

bool fill_data_with_poison(void * data, const size_t size, long long int poison, const size_t Elem_size){
	
	node_assert(data != nullptr);

	for(unsigned int i = 0; i < size * Elem_size; i++){
		((char *)data)[i] = ((char *) &poison)[i % Elem_size];
	}

	return true;

}

void zero_data(void * const data, const int start, const int end){

	node_assert(data != nullptr);
	node_assert(start < end);	

	for(int i = start; i < end; i++){
		((char *) data)[i] = '\0';
	}

}

bool check_node(Node_t *node, const char called_from_file[], int line, const char func[], bool print_all/* = true*/){


	bool passed = true;
	bool hashable = true;

	//printf("%p\n", THIS);

	if(!check_nullptr(THIS)){
		return false;
	}

	if(THIS->errnum != 0){
		return false;	
	}

// A Lot of checks below
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_canary(THIS)                     ? passed : (THIS->errnum += CHECK_CANARY, false);
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_hash(THIS)		        ? passed : (THIS->errnum += CHECK_HASH, false);
	if(DEBUG) printf("Passed: %d\n", passed);
 
	passed = check_nullptr(THIS->name)              ? passed : (THIS->errnum += NAME_NULLPTR, false);
	if(DEBUG) printf("Passed: %d\n", passed);

/*	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);*/

	if(DEBUG) printf("\n");

// End of a Lot of checks

	//printf("%d %d\n", DATA[0], DATA[1]);

	if(print_all || !passed){

		node_dump(THIS, called_from_file, line, func, passed);
	}

	return passed;
}


#undef fprintf

void node_dump(Node_t * const node, const char called_from_file[], int line, const char func[], bool ok, time_t curtime /* = -1*/){

	if(THIS == nullptr) return;

	bool cur_time_setted = true;

	if(curtime == -1){
		time(&curtime);	
		cur_time_setted = false;
	}

	FILE *log_file = fopen(LOG_FILE, "a");
	if(log_file == nullptr){
		fprintf(stderr, "Not able to open %s in %s line %d in %s\n", LOG_FILE, __LOCATION__);
	}


	fprintf(log_file, (cur_time_setted) ? "%s" : "\nTime and date: %s\n"
			  "Dumping node from file %s, line %d, function %s [%s]\n"
			  "node [%p] %s [%p] {\n"
			  "\n\t hash    = " HASH_PRINT "\n"
			  "\n\t canary1 = " CANARY_PRINT "\n"
			  "\t data    = [%p] {" ELEM_PRINT "}\n"
			  "\t right   = [%p]\n"
			  "\t left    = [%p]\n"
			  "\t parent  = [%p]\n",
		(cur_time_setted) ? "" : ctime(&curtime),
		called_from_file, line, func, ok ? "ok" : "ERROR!!!",
		THIS, THIS->name, THIS->name,
		THIS->hash,
		THIS->canary1,
		DATA, DATA,
		RIGHT,
		LEFT,
		PARENT);
	fflush(log_file);
	
	fprintf(log_file, "\n\t errnum   = ");
	for(int i = 0; i < MAX_ERROR_NO; i++){
		fprintf(log_file, "%d", (THIS->errnum >> i) % 2);
	}
	fprintf(log_file, "\n");
	fflush(log_file); 

	fprintf(log_file, "\n\t canary2 = " CANARY_PRINT "\n\n}\n\n",
		THIS->canary2);

	fflush(log_file);

	fclose(log_file);

}

void dot_node(Node_t * root, const char ofile[]){

	node_assert(check_node(root, __LOCATION__));

	FILE *file = fopen(TEMPFILE, "wb");
	fprintf(file, "digraph %s{\n", root->name);

	color_node(root, file);
	
	node_dot_dependences(root, file);	

	fprintf(file, "}\n");

	fclose(file);
	
	char command[MAX_COMMAND_LEN];

	sprintf(command, "dot -Tpdf %s -o %s\n", TEMPFILE, ofile);
	std::system(command);

	//sprintf(command, "rm %s\n", TEMPFILE);
	//std::system(command);
	
}

void color_node(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;	
	}

	char color[MAX_COMMAND_LEN] = {};
	sprintf(color, "color = \"%s\"", THIS->mode == MODE_FUNC ? "blue" :
					(THIS->mode == MODE_VARL ? "red" :
					(THIS->mode == MODE_CNST ? "green" : "black")));

	fprintf(file, "%d [%s; shape = record; label =\"" ELEM_PRINT "\"]\n", THIS, color, DATA);

	color_node(LEFT, file);
	color_node(RIGHT, file);

}

void node_dot_dependences(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;	
	}

	if(LEFT   != nullptr){
		fprintf(file, "%d -> %d\n", THIS, LEFT);
	}
	if(RIGHT  != nullptr){
		fprintf(file, "%d -> %d\n", THIS, RIGHT);
	}
	
	node_dot_dependences(LEFT, file);
	node_dot_dependences(RIGHT, file);
}

bool check_nullptr(const void * const data){
	
	return data != nullptr;
}

bool check_canary(Node_t * const node){

	if(THIS == nullptr) return false;
	return THIS->canary1 == THIS->canary2 && THIS->canary2 == CANARY_VALUE;
}

bool check_hash(Node_t * const node){

	if(!check_hashable(THIS)) return false;
	return is_equal_hash_t(find_sum_hash(THIS, sizeof(Node_t)), THIS->hash);
}

bool check_hashable(Node_t * const node){
	
	return true;
}

bool is_equal_hash_t(const hash_t left, const hash_t right){
	
	return left == right;
}

hash_t find_sum_hash(Node_t * const node, size_t size){

	if(THIS == nullptr) return false;

	hash_t hash_sum = 0;
	
	hash_sum += hash(((char *)THIS) + sizeof(hash_t), size - sizeof(hash_t));

	return hash_sum;

}

hash_t hash(const void * const data, size_t size){

	if(data == nullptr) return false;

	hash_t hash_sum = 0;

	for(size_t counter = 0; counter < size; counter++){
		
		hash_sum = (hash_sum << 2) + (counter+1)*((*((char *) data + counter))^0xfc);
	}
	
	return hash_sum;

}


int clear_stdin(void){

	int did = 0;
	Elem_t c = fgetc(stdin);

	while(c != Long_string('\n') && c != EOF){
		did = 1;
		c = fgetc(stdin);
	}

	return did;	
}


