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
    s_infix = PARCER::delete_spaces(_infix);
    q_infix = Parcer::parce_infix(s_infix);
}

bool ArithmeticExpression::run_analyzer() {
    bool res = true;

    bool lx_c = Analyzer::lexic_check(q_infix); res &= lx_c;
    Analyzer::print_error_message();

    bool sk_c = Analyzer::skobochniy_check(s_infix); res &= sk_c;
    Analyzer::print_error_message();

    bool sy_c = Analyzer::syntax_check(q_infix); res &= sy_c;
    Analyzer::print_error_message();

    return res;
}

double ArithmeticExpression::calculate() {
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