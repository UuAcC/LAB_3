#include "Pascal_MinusMinus_Expression.h"



LEXEM::lexem(string s, LexemType t) { value = s; type = t; }
ostream& operator<<(ostream& ostr, const LEXEM& lex) {
	ostr << "{ " << lex.value << ", " << lex.type << " }";
	return ostr;
}



ERROR::error(size_t i, string v) { index = i; value = v; }
ERROR::error(size_t i, char c) { index = i; value = string(1, c); }
ostream& operator<<(ostream& ostr, const ERROR& er) {
	ostr << "{ " << er.index << ", " << er.value << " }";
	return ostr;
}



#include "LexTypeDefines.h"

inline std::string getLexemTypeName(LEX_TYPE type) {
#define X(name, value) if (type == value) return #name;
	LIST_OF_MACROS
#undef X
		return "_";
}
ostream& operator<<(ostream& ostr, const LEX_TYPE& tp) {
	ostr << getLexemTypeName(tp);
	return ostr;
}