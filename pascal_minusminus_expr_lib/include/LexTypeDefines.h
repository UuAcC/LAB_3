#pragma once
#include "Pascal_MinusMinus_Expression.h"

#define LIST_OF_MACROS \
    X(ZERO, LEX_TYPE::zero) \
    X(NUM, LEX_TYPE::num) \
    X(DOT, LEX_TYPE::dot) \
    X(VAR, LEX_TYPE::variable) \
        X(L_R_BR, LEX_TYPE::l_round_br) \
        X(R_R_BR, LEX_TYPE::r_round_br) \
        X(L_C_BR, LEX_TYPE::l_curly_br) \
        X(R_C_BR, LEX_TYPE::r_curly_br) \
    X(SMCLN, LEX_TYPE::semicolon) \
        X(WHILE, LEX_TYPE::keyword_while) \
        X(IF, LEX_TYPE::keyword_if) \
        X(ELSE, LEX_TYPE::keyword_else) \
    X(ADD, LEX_TYPE::add) \
    X(SUB, LEX_TYPE::sub) \
    X(MUL, LEX_TYPE::mul) \
    X(DIV, LEX_TYPE::div) \
    X(EQ, LEX_TYPE::equal) \
        X(C_EQ, LEX_TYPE::c_equal) \
        X(C_NEQ, LEX_TYPE::c_not_equal) \
        X(C_M, LEX_TYPE::c_more) \
        X(C_ME, LEX_TYPE::c_more_equal) \
        X(C_L, LEX_TYPE::c_less) \
        X(C_LE, LEX_TYPE::c_less_equal) \
    X(NAL, LEX_TYPE::not_a_lexem) 

#define X(name, value) constexpr LEX_TYPE name = value;
LIST_OF_MACROS
#undef X