#include "ExprTreeNodes.h"
#include "ExprTreeVisitors.h"
#include <iostream>

double FPNumber::getVal() const { return std::stod(val); }

ExprRetVal FPNumber::accept(Visitor* v) {
	return v->visitFPNumber(this);
}

Expr* BiOperation::getLeft() { return left; }
Expr* BiOperation::getRight() { return right; }
std::string BiOperation::getOp() const { return op; }

ExprRetVal BiOperation::accept(Visitor* v) {
	return v->visitBiOperation(this);
}

string Variable::getName() const { return name; }

ExprRetVal Variable::accept(Visitor* v) {
	return v->visitVariable(this);
}

Expr* init_fpnumber(std::string v) {
	return new FPNumber(v);
}
Expr* init_bioperation(std::string c, Expr* l, Expr* r) {
	return new BiOperation(c, l, r);
}
Expr* init_variable(std::string n) {
	return new Variable(n);
}