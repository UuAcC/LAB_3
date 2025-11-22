#include "ArithmeticExpression.h"

LEXEM::lexem(string s, Type t) { value = s; type = t; }
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

ostream& operator<<(ostream& ostr, const TYPE& tp) {
    switch (tp) {
    case TYPE::zero: { ostr << "ZERO"; break; }
    case TYPE::num: { ostr << "NUM"; break; }
    case TYPE::dot: { ostr << "DOT"; break; }
    case TYPE::l_br: { ostr << "L_BR"; break; }
    case TYPE::r_br: { ostr << "R_BR"; break; }
    case TYPE::bop: { ostr << "BOP"; break; }
    case TYPE::mul: { ostr << "MUL"; break; }
    case TYPE::div: { ostr << "DIV"; break; }
    default: { ostr << "_"; }
    }
    return ostr;
}