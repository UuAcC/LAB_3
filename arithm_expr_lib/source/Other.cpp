#include "ArithmeticExpression.h"

LEXEM::lexem(string s, Type t) { value = s; type = t; }

ostream& operator<<(ostream& ostr, const LEXEM& lex) {
	ostr << "{" << lex.value << "," << lex.type << "}";
	return ostr;
}

ostream& operator<<(ostream& ostr, const TYPE& tp) {
	switch (tp) {
	case TYPE::zero: { ostr << "ZERO"; break; }
	case TYPE::num: { ostr << "NUM"; break; }
	case TYPE::dot: { ostr << "DOT"; break; }
	case TYPE::l_br: { ostr << "L_BR"; break; }
	case TYPE::r_br: { ostr << "R_BR"; break; }
	case TYPE::op: { ostr << "OP"; break; }
	default: { ostr << "_"; }
	}
	return ostr;
}