#include "Pascal_MinusMinus_Expression.h"
#include "ExprExecExceprions.h"
#include "ExprIterativeExecutor.h"

LEX_TYPE PMM_EXPR::decode(char c) {
    switch (c) {
    case '0': return LexemType::zero;
    case '(': return LexemType::l_round_br;
    case ')': return LexemType::r_round_br;
    case '.': return LexemType::dot;
    case '+':
    case '-': return LexemType::add_sub;
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

void PMM_EXPR::execute() {
    try { 
        Expr* tree = get_tree();
        //CalcVisitor cv;
        //tree->accept(&cv);
        IterativeExecutor ie;
        ie.run(tree);
    }
    catch (ExprExecException* err) {
        cout << err->message();
    }
}

//void PMM_EXPR::print() {
//    Expr* tree = get_tree();
//    PrintVisitor pv;
//    tree->accept(&pv);
//}
