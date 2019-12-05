#pragma once
#include"node.h"
#undef USING_WCHAR_T
#include"ionegin"


#define syntax_assert(cond) return nullptr;


const int MAX_FUNC_NAME = 4;


Node_t * Get_G(Buff_elem_t * str, int * pc);


Node_t * Get_E(Buff_elem_t * str, int * pc);


Node_t * Get_T(Buff_elem_t * str, int * pc);


Node_t * Get_POW(Buff_elem_t * str, int * pc);


Node_t * Get_UNAR(Buff_elem_t * str, int * pc);


Node_t * Get_P(Buff_elem_t * str, int * pc);


Node_t * Get_FUNC_CALL(Buff_elem_t * str, int * pc);


Node_t * Get_N(Buff_elem_t * str, int * pc);


Buff_elem_t * Get_ID(Buff_elem_t * str, int * pc);




Node_t * Get_G(Buff_elem_t * str, int * pc){
	
	Node_t * val = nullptr;
	
	val = Get_E(str, pc);

	if(str[*pc] == '\0'){
		(*pc)++;
		return val;
	}
	else{
		syntax_assert(true == false);
	}
}

Node_t * Get_E(Buff_elem_t * str, int * pc){

	Node_t * val = nullptr;

	val = Get_T(str, pc);

	int mode = -1;
	while(str[*pc] == '+' || str[*pc] == '-'){

		mode = str[*pc];
		(*pc)++;
		Node_t * val2 = Get_T(str, pc);

		Node_t * temp = 
		val = node_create_new(MODE_OPER, mode, val, val2);

	}

	return val;

}

Node_t * Get_T(Buff_elem_t * str, int * pc){

	Node_t * val = nullptr;
	val = Get_POW(str, pc);

	int mode = -1;
	while(str[*pc] == '*' || str[*pc] == '/'){

		mode = str[*pc];
		(*pc)++;
		Node_t * val2 = Get_P(str, pc);

		val = node_create_new(MODE_OPER, mode, val, val2);
	}

	return val;
}

Node_t * Get_POW(Buff_elem_t * str, int * pc){

	Node_t * val = nullptr;
	val = Get_UNAR(str, pc);

	int mode = -1;
	while(str[*pc] == '^'){

		mode = str[*pc];
		(*pc)++;
		Node_t * val2 = Get_UNAR(str, pc);

		val = node_create_new(MODE_OPER, mode, val, val2);
	}

	return val;
}

Node_t * Get_UNAR(Buff_elem_t * str, int * pc){

	Node_t * val = nullptr;

	if(str[*pc] == '-' || str[*pc] == '+'){

		char mode = str[*pc];
		(*pc)++;
		Node_t * val2 = Get_P(str, pc);
		val = node_create_new(MODE_OPER, mode, nullptr, val2);
	}
	else{
		val = Get_P(str, pc);
	}

	return val;
}

Node_t * Get_P(Buff_elem_t * str, int * pc){

	Node_t * val = nullptr;
	bool found = false;	

	if(str[*pc] == '('){
	
		(*pc)++;
		val = Get_E(str, pc);
		found = true;
	}
	else{
		if('0' <= str[*pc] && str[*pc] <= '9'){

			val = Get_N(str, pc);
			return val;	
		}
		else{
			
			val = Get_FUNC_CALL(str, pc);
			if(val == nullptr){
				Buff_elem_t * val1 = Get_ID(str, pc);
				char varib = val1[0];
				free(val1);

				return node_create_new(MODE_VARL, varib, nullptr, nullptr);
			}
			return val;
		}
	}

	if(str[*pc] != ')'){
		syntax_assert(true == false);
	}
	else{
		(*pc)++;
	}

	return val;

}

Node_t * Get_FUNC_CALL(Buff_elem_t * str, int * pc){

	int saved_pc = *pc;

	Node_t * val = nullptr;
	Buff_elem_t * val1 = Get_ID(str, pc);

	if(val1 == nullptr || val1[0] == '\0'){
		syntax_assert(false == true);
	}
	
	int pos = list_find_by_elem(mstokens, val1);

	Node_t * val2 = nullptr;

	if(pos > 0){
		
		if(!str[*pc] == '('){
			free(val1);
			syntax_assert(false == true);
		}
		else{
			(*pc)++;
		}
		
		val2 = Get_E(str, pc);
		
		if(!str[*pc] == ')'){

			node_deinit(val2);
			free(val1);
			syntax_assert(false == true);
		}
		else{
			(*pc)++;
		}
	}
	else{
		*pc = saved_pc;
		syntax_assert(true == false);
	}


	free(val1);

	return node_create_new(MODE_FUNC, pos, nullptr, val2);

}

Node_t * Get_N(Buff_elem_t * str, int * pc){
	
	Node_elem_t val = 0;
	bool done = false;
	while('0' <= str[*pc] && str[*pc] <= '9'){
		
		done = true;
		val  = val*10 + str[*pc] - '0';
		(*pc)++;
	}
	if(str[*pc] == '.'){

		(*pc)++;
		int i = ACCURACY;
		for(; i > 1 && '0' <= str[*pc] && str[*pc] <= '9'; i/=10){
			done = true;
			val = val*10 + str[*pc] - '0';
			(*pc)++;
		}
		val *= i;
	
	}
	if(!done){
		assert(("Get_N", false) == true);
	}

	return node_create_new(MODE_CNST, val, nullptr, nullptr);
}


Buff_elem_t * Get_ID(Buff_elem_t * str, int * pc){

	Buff_elem_t * val = (Buff_elem_t *) calloc(MAX_FUNC_NAME, sizeof(Buff_elem_t));
	bool done = false;

	for(int i = 0; ('0' <= str[*pc] && str[*pc] <= '9' && i > 0) || ('a' <= str[*pc] && str[*pc] <= 'z') || ('A' <= str[*pc] && str[*pc] <= 'Z'); i++){

		if(i >= MAX_FUNC_NAME){
			done = false;
			break;
		}
		val[i] = str[*pc];
		(*pc)++;
		done = true;
	}

	if(!done){
		syntax_assert(false == true);
	}


	return val;
}


