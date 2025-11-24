#include "ArithmeticExpression.h"

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
}

bool ArithmeticExpression::run_analyzer(bool print_smth) {
    bool res = true;

    bool lx_c = Analyzer::lexic_check(q_infix);
    if (print_smth) 
        Analyzer::print_error_message(); 
    res &= lx_c;
    /*if (!lx_c) return false;*/

    /*bool sk_c = Analyzer::skobochniy_check(s_infix); 
    Analyzer::print_error_message(); res &= sk_c;*/
    /*if (!sk_c) return false;*/

    bool sy_c = Analyzer::syntax_check(q_infix); 
    if (print_smth) 
        Analyzer::print_error_message(); 
    res &= sy_c;
    /*if (!sy_c) return false;*/

    return res;
}

double ArithmeticExpression::calculate() {
    q_postfix = get_q_postfix();
    TQueue<lexem> temp(q_postfix);
    TStack<double> stack(temp.get_size());
    while (!temp.isEmpty()) {
        lexem curr = temp.pop();
        if (curr.type == Type::zero || curr.type == Type::num)
            stack.push(PARCER::to_double(curr.value));
        else {
            double arg2 = stack.pop();
            double arg1 = stack.pop();
            stack.push(ae_call(curr.value)(arg1, arg2));
        }
    } return stack.pop();
}