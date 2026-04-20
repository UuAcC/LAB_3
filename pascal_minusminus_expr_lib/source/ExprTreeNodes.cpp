#include "ExprBaseVisitor.h"
#include "Pascal_MinusMinus_Expression.h"
#include <iostream>

PMM_EXPR::BiNode::BiNode(Node* l, Node* r) : left(l), right(r) {}
PMM_EXPR::Node* PMM_EXPR::BiNode::getLeft() const { return left; }
PMM_EXPR::Node* PMM_EXPR::BiNode::getRight() const { return right; }

PMM_EXPR::NodeRetVal PMM_EXPR::Variable::accept(Visitor* v) {
    return v->visitVariable(this);
}

PMM_EXPR::NodeRetVal PMM_EXPR::FPNumber::accept(Visitor* v) {
    return v->visitFPNumber(this);
}

PMM_EXPR::Mon::Mon(Terminal* l) : BiNode(l) {}
PMM_EXPR::Mon::Mon(Mon* l, LexemType _op, Terminal* r) : BiNode(l, r) {
    op = _op;
}
LEX_TYPE PMM_EXPR::Mon::getOp() const { return op; }
PMM_EXPR::NodeRetVal PMM_EXPR::Mon::accept(Visitor* v) {
    return v->visitMon(this);
}

PMM_EXPR::Pol::Pol(Mon* l) : BiNode(l) {}
PMM_EXPR::Pol::Pol(Pol* l, LexemType _op, Mon* r) : BiNode(l, r) {
    op = _op;
}
LEX_TYPE PMM_EXPR::Pol::getOp() const { return op; }
PMM_EXPR::NodeRetVal PMM_EXPR::Pol::accept(Visitor* v) {
    return v->visitPol(this);
}

PMM_EXPR::Comp::Comp(Pol* l, LexemType _op, Pol* r) : BiNode(l, r) {
    op = _op;
}
LEX_TYPE PMM_EXPR::Comp::getOp() const { return op; }
PMM_EXPR::NodeRetVal PMM_EXPR::Comp::accept(Visitor* v) {
    return v->visitComp(this);
}

PMM_EXPR::EqOper::EqOper(Variable* l, Pol* r) : BiNode(l, r) {}
PMM_EXPR::NodeRetVal PMM_EXPR::EqOper::accept(Visitor* v) {
    return v->visitEqu(this);
}

PMM_EXPR::Expr::Expr(Operator* l) : BiNode(l) {}
PMM_EXPR::Expr::Expr(Expr* l, Operator* r) : BiNode(l, r) {}
PMM_EXPR::NodeRetVal PMM_EXPR::Expr::accept(Visitor* v) {
    return v->visitExpr(this);
}

PMM_EXPR::IfOper::IfOper(Comp* l, Expr* r) : BiNode(l, r) {}
PMM_EXPR::NodeRetVal PMM_EXPR::IfOper::accept(Visitor* v) {
    return v->visitIfOper(this);
}

PMM_EXPR::IfElse::IfElse(IfOper* l, Expr* r) : BiNode(l, r) {}
PMM_EXPR::NodeRetVal PMM_EXPR::IfElse::accept(Visitor* v) {
    return v->visitIfElse(this);
}

PMM_EXPR::Operator::Operator(EqOper* l) : BiNode(l) {}
PMM_EXPR::Operator::Operator(Comp* l, Expr* r) : BiNode(l, r) {}
PMM_EXPR::Operator::Operator(IfElse* l) : BiNode(l) {}
PMM_EXPR::NodeRetVal PMM_EXPR::Operator::accept(Visitor* v) {
    return v->visitOperator(this);
}

PMM_EXPR::ExprTree::ExprTree(Expr* c) : child(c) {}
PMM_EXPR::NodeRetVal PMM_EXPR::ExprTree::accept(Visitor* v) {
    return v->visitTree(this);
}