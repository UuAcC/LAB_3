#include "Pascal_MinusMinus_Expression.h"
#include "ExprExecExceprions.h"
#include "ExprIterativeExecutor.h"

LEX_TYPE PMM_EXPR::decode(char c) {
    switch (c) {
    case '0': return LexemType::zero;
    case '(': return LexemType::l_round_br;
    case ')': return LexemType::r_round_br;
    case '{': return LexemType::l_curly_br;
    case '}': return LexemType::r_curly_br;
    case '.': return LexemType::dot;
    case '+': return LexemType::add;
    case '-': return LexemType::sub;
    case '*': return LexemType::mul;
    case '/': return LexemType::div;
    case '=': return LexemType::equal;
    case '<': return LexemType::c_less;
    case '>': return LexemType::c_more;
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

inline PMM_EXPR::ExprTree* PMM_EXPR::get_tree() {
    if (expression_tree == nullptr)
        expression_tree = Parcer::bottomup_parce_tree(q_infix);
    return expression_tree;
}

void PMM_EXPR::execute() {
    try { 
        ExprTree* tree = get_tree();
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
