//DEF_TEX(name, equal, command)
#define TEX(arg) tex_node_tree(arg, file)
#define PRINT(arg) fprintf(file, arg)
#define SEPARATOR_S	 if(first_priority - second_priority > 0 || first_priority == 0){\
				fprintf(file, "%c", SEPARATOR_START);\
			 }
#define SEPARATOR_E	 if(first_priority - second_priority > 0 || first_priority == 0){\
				fprintf(file, "%c", SEPARATOR_END);\
			 }

DEF_TEX(add, '+', 1, SEPARATOR_S; TEX(LEFT); LEFT == nullptr ? PRINT(" +") : PRINT(" + "); TEX(RIGHT); SEPARATOR_E;)
DEF_TEX(sub, '-', 1, SEPARATOR_S; TEX(LEFT); LEFT == nullptr ? PRINT(" -") : PRINT(" - "); TEX(RIGHT); SEPARATOR_E;)
DEF_TEX(mul, '*', 2, SEPARATOR_S; TEX(LEFT); PRINT("\\cdot "); TEX(RIGHT); SEPARATOR_E;)
DEF_TEX(div, '/', 2, SEPARATOR_S; PRINT("\\frac{"); TEX(LEFT); PRINT("}"); PRINT("{"); TEX(RIGHT); PRINT("}"); SEPARATOR_E;)
DEF_TEX(pow, '^', 3, SEPARATOR_S; TEX(LEFT); PRINT("^");   TEX(RIGHT); SEPARATOR_E;)

/*DEF_TEX(add, 'sin', 0, TEX(LEFT); PRINT(+); TEX(RIGHT))
DEF_TEX(add, 'cos', 0, TEX(LEFT); PRINT(+); TEX(RIGHT))
DEF_TEX(add, '^', 3, TEX(LEFT); PRINT(+); TEX(RIGHT))
DEF_TEX(add, 'ln', 0, TEX(LEFT); PRINT(+); TEX(RIGHT))
*/
#undef TEX
#undef PRINT
#undef SEPARATOR_S
#undef SEPARATOR_E
