//DEF_DIF(name, mode, wchar_t equivalent, code)

#define dL do_diff_recursive(node->left)
#define cL node_copy(node->left)
#define dR do_diff_recursive(node->right)
#define cR node_copy(node->right)
#define NEW(mode, elem, left, right) node_create_new(mode, elem, left, right)

DEF_DIF(cst, MODE_CNST, '\0' , return NEW(MODE_CNST, '0', nullptr, nullptr);)

DEF_DIF(var, MODE_VARL, '\0' , return NEW(MODE_CNST, '1', nullptr, nullptr);)

DEF_DIF(add, MODE_FUNC, '+', return NEW(MODE_FUNC, '+', dL, dR);)

DEF_DIF(sub, MODE_FUNC, '-', return NEW(MODE_FUNC, '-', dL, dR);)

DEF_DIF(mul, MODE_FUNC, '*', return NEW(MODE_FUNC, '+', NEW(MODE_FUNC, '*', dL, cR), NEW(MODE_FUNC, '*', cL, dR));)

//DEF_DIF(div, '/', return NEW(MODE_FUNC, '/', dL, dR);)

#undef dL
#undef cL
#undef dR
#undef cR
#undef NEW


