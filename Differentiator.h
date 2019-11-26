#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <locale.h>
#include "includes/test-log.define"
//#define USING_WCHAR_T
#include "includes/ionegin"

#define IS_ANS(arg) strcasecmp(ans, Long_string((arg))) == 0

#define asserted || (fprintf(stderr, "Asserted from %s on %d in %s\n", __LOCATION__), abort(), false);
#define node_assert(condition) if(!(condition)){fprintf(stderr, "File %s line %d function %s %s\n", __LOCATION__, #condition); abort();}
//#define node_assert(condition)

#define check_parent(arg) arg->parent == nullptr ? false : arg->parent->right == arg 

#define NODE_INIT(name, ...) node_init((name), #name, ##__VA_ARGS__)

#define NODE_ADD(DIR, elem) 	if(DIR != nullptr && DIR != THIS){return nullptr;}\
				DIR = node_create_new(-1, elem, nullptr, nullptr, (DIR != THIS) ? THIS : nullptr);\
				THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

#define PC_N (*pc)
#define PC_INCR (*pc)+=1

#define THIS   node
#define DATA   node->data
#define LEFT   node->left
#define RIGHT  node->right
#define PARENT node->parent

//#define TESTING

#ifdef TESTING
#define NDEBUG
#endif
#include <assert.h>

typedef int Elem_t;
#define ELEM_PRINT "%c" 

typedef unsigned long long int Canary_t;
#define CANARY_PRINT "%LX"

typedef unsigned long long int hash_t;
#define HASH_PRINT "%Lu"

struct Node_t {

	hash_t hash; // always comes first
	const char *name;

	Canary_t canary1;

	Elem_t data;
	Node_t * left;
	Node_t * right;
	Node_t * parent;
	int level;
	int mode;

	int errnum;

	Canary_t canary2;

};


const int CHECK_ZERO_ 		   	= __LINE__;
const int CHECK_CANARY             	= 1 << (__LINE__ - CHECK_ZERO_ - 1); //= 1;
const int CHECK_HASH               	= 1 << (__LINE__ - CHECK_ZERO_ - 1); //= 2;
const int NAME_NULLPTR             	= 1 << (__LINE__ - CHECK_ZERO_ - 1); //= 4;
const int MAX_ERROR_NO 		   	= (__LINE__ - CHECK_ZERO_ - 1);


const bool DEBUG 			= 0;
const Canary_t CANARY_VALUE 		= 0xBEDAAAAAAAAAAAAA;
const long long int POISON 		= 'IKIN';
const long long int POISON_POISON 	= '3DED';
const int LIST_INIT_SIZE 		= 5;
const int START_FUNCTION 		= 1;
const char LOG_FILE[]   		= "node_log.txt";
const char IMAGE_OUT[]  		= "Graph.pdf";	
const char IMAGE_OUT_2[]  		= "Graph_1.pdf";	
const char TEMPFILE[] 			= "tempfile.txt";
const char TREE_FILE[]			= "tree_file.tree";
const int MAX_COMMAND_LEN 		= 100;
const int END_FUNCTION 			= 0;
const char SEPARATOR_START		= L'(';
const char SEPARATOR_END		= L')';

const int MODE_VARL			= 1;
const int MODE_CNST			= 2;
const int MODE_FUNC			= 3;
//const int LIST_CLUSTER_SIZE = sizeof(Elem_t) + 2*sizeof(int);


Node_t * differentiate_to_new(Node_t * node);


Node_t * do_diff_recursive(Node_t * node);


Node_t * node_copy(Node_t * node);


Node_t * node_create_new(const int mode, const Elem_t elem, Node_t * left, Node_t * right, Node_t * parent = nullptr);


bool node_tree_optimize(Node_t * node);


void node_write_to_file(Node_t * node, FILE * file);


Node_t * node_make_from_file(const char file_name[]);


Node_t * node_make_from_buff(Node_t * node, Buff_elem_t * buff, const int buff_size, int *pc);


int get_mode(str_ptr str);


bool get_data(Buff_elem_t * buff, const int buff_size, int *pc, str_ptr str);


bool node_init(Node_t *node, const char name[], Node_t *parent);


Node_t *node_add_right(Node_t *node, Elem_t elem);


Node_t *node_add_left(Node_t *node, Elem_t elem);


Node_t * node_find(Node_t *node, str_ptr cmp_elem);


bool node_deinit(Node_t *node);


Elem_t node_get(Node_t *node);


bool node_set(Node_t *node, Elem_t elem, const int mode);


bool fill_data_with_poison(void * data, const size_t size, long long int poison, const size_t Elem_size);


void zero_data(void * const data, const int start, const int end);


bool check_node(Node_t *node, const char called_from_file[], int line, const char func[], bool print_all = true);


void node_dump(Node_t * const node, const char called_from_file[], int line, const char func[], bool ok, time_t curtime = -1);


void dot_node(Node_t * root, const char ofile[]);


void color_node(Node_t * node, FILE * file);


void node_dot_dependences(Node_t * node, FILE * file);


bool check_nullptr(const void * const node);


bool check_canary(Node_t * const node);


bool check_hash(Node_t * const node);


bool check_hashable(Node_t * const node);


bool is_equal_hash_t(const hash_t left, const hash_t right);


hash_t find_sum_hash(Node_t * const node, size_t size);


hash_t hash(const void * const node, size_t size);


bool check_size(Node_t * const node);


int clear_stdin(void);
