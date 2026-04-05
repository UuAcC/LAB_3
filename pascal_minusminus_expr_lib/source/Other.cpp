#include "Pascal_MinusMinus_Expression.h"

LEXEM::lexem(string s, LexemType t) { value = s; type = t; }
ERROR::error(size_t i, string v) { index = i; value = v; }
ERROR::error(size_t i, char c) { index = i; value = string(1, c); }

ostream& operator<<(ostream& ostr, const LEXEM& lex) {
	ostr << "{ " << lex.value << ", " << lex.type << " }";
	return ostr;
}
ostream& operator<<(ostream& ostr, const ERROR& er) {
	ostr << "{ " << er.index << ", " << er.value << " }";
	return ostr;
}

ostream& operator<<(ostream& ostr, const LEX_TYPE& tp) {
	switch (tp) {
	case LEX_TYPE::zero: { ostr << "ZERO"; break; }
	case LEX_TYPE::num: { ostr << "NUM"; break; }
	case LEX_TYPE::dot: { ostr << "DOT"; break; }
	case LEX_TYPE::l_br: { ostr << "L_BR"; break; }
	case LEX_TYPE::r_br: { ostr << "R_BR"; break; }
	case LEX_TYPE::bop: { ostr << "BOP"; break; }
	case LEX_TYPE::mul: { ostr << "MUL"; break; }
	case LEX_TYPE::div: { ostr << "DIV"; break; }
	case LEX_TYPE::equal: { ostr << "EQ"; break; }
	case LEX_TYPE::semicolon: { ostr << "SMCLN"; break; }
	case LEX_TYPE::variable: { ostr << "VAR"; break; }
	default: { ostr << "_"; }
	}
	return ostr;
}