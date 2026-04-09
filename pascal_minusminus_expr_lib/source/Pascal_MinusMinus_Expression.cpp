#include "Pascal_MinusMinus_Expression.h"
#include "ExprExecExceprions.h"

LEX_TYPE PMM_EXPR::decode(char c) {
    switch (c) {
    case '0': return LexemType::zero;
    case '(': return LexemType::l_br;
    case ')': return LexemType::r_br;
    case '.': return LexemType::dot;
    case '+':
    case '-': return LexemType::bop;
    case '*': return LexemType::mul;
    case '/': return LexemType::div;
    case '=': return LexemType::equal;
    case ';': return LexemType::semicolon;
    default:
        if (CHAR_IN_LETTERS)
            return LexemType::variable;
        return LexemType::num;
    }
}

PMM_EXPR::Pascal_MinusMinus_Expression(string _infix) {
    s_infix = Parcer::delete_spaces(_infix);
    q_infix = Parcer::parce_infix(s_infix);
    expression_tree = nullptr;
}

bool PMM_EXPR::run_analyzer(bool print_smth) {
    bool res = true;

    bool lx_c = Analyzer::lexic_check(q_infix);
    if (print_smth) 
        Analyzer::print_error_message(); 
    res &= lx_c;

    bool sy_c = Analyzer::syntax_check(q_infix); 
    if (print_smth) 
        Analyzer::print_error_message(); 
    res &= sy_c;

    return res;
}

void PMM_EXPR::execute(bool da) {
    try { 
        Expr* tree = get_tree();
        if (da) { run_analyzer(); }
        CalcVisitor cv;
        tree->accept(&cv);
    }
    catch (ExprExecException* err) {
        cout << err->message();
    }
}

void PMM_EXPR::print(bool da) {
    Expr* tree = get_tree();
    if (da) { run_analyzer(); }
    PrintVisitor pv;
    tree->accept(&pv);
}
