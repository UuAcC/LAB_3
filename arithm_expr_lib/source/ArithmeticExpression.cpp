#include "ArithmeticExpression.h"

TYPE ArithmeticExpression::decode(char c) {
    switch (c) {
    case '0': return Type::zero;
    case '(': return Type::l_br;
    case ')': return Type::r_br;
    case '.': return Type::dot;
    case '+':
    case '-': return Type::bop;
    case '*': return Type::mul;
    case '/': return Type::div;
    case '=': return Type::equal;
    case ';': return Type::semicolon;
    default:
        if (CHAR_IN_LETTERS)
            return Type::variable;
        return Type::num;
    }
}

arithm_fp ArithmeticExpression::ae_call(string str) {
    switch (str[0]) {
    case '+': return add;
    case '-': return sub;
    case '*': return mul;
    default: return div;
    }
}

ArithmeticExpression::ArithmeticExpression(string _infix) {
    s_infix = Parcer::delete_spaces(_infix);
    q_infix = Parcer::parce_infix(s_infix);
    arithm_expr_tree = nullptr;
}

bool ArithmeticExpression::run_analyzer(bool print_smth) {
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

double ArithmeticExpression::calculate() {
    return calculate_tree();
}

double ArithmeticExpression::calculate_tree() {
    Expr* tree = get_tree();
    run_analyzer();
    CalcVisitor cv;
    return tree->accept(&cv);
}

void ArithmeticExpression::print() {
    Expr* tree = get_tree();
    run_analyzer();
    PrintVisitor pv;
    tree->accept(&pv);
}
