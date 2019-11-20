//DEF_DIF(name, mode, wchar_t equivalent, code)

#define dL do_diff_recursive(node->left)
#define cL node_copy(node->left)
#define dR do_diff_recursive(node->right)
#define cR node_copy(node->right)
#define NEW(mode, elem, left, right) node_create_new(mode, elem, left, right)

DEF_DIF(cst, MODE_CNST, L"" , return NEW(MODE_CNST, L"0", nullptr, nullptr);)

DEF_DIF(var, MODE_VARL, L"" , return NEW(MODE_CNST, L"1", nullptr, nullptr);)

DEF_DIF(add, MODE_FUNC, L"+", return NEW(MODE_FUNC, L"+", dL, dR);)

DEF_DIF(sub, MODE_FUNC, L"-", return NEW(MODE_FUNC, L"-", dL, dR);)

DEF_DIF(mul, MODE_FUNC, L"*", return NEW(MODE_FUNC, L"+", NEW(MODE_FUNC, L"*", dL, cR), NEW(MODE_FUNC, L"*", cL, dR));)

//DEF_DIF(div, L"/", return NEW(MODE_FUNC, L"/", dL, dR);)

#undef dL
#undef cL
#undef dR
#undef cR
#undef NEW


