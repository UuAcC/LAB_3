#include "TreeVisitor.h"

double FPNumber::getVal() const { return std::stod(val); }
double FPNumber::accept(Visitor* v) {
	return v->visitFPNumber(this);
}

double BiOperation::accept(Visitor* v) {
	return v->visitBiOperation(this);
}
Expr* BiOperation::getLeft() { return left; }
Expr* BiOperation::getRight() { return right; }
std::string BiOperation::getOp() const { return op; }

double CalcVisitor::visitFPNumber(FPNumber* num) {
	return num->getVal();
}
double CalcVisitor::visitBiOperation(BiOperation* op) {
	double left = op->getLeft()->accept(this),
		right = op->getRight()->accept(this);
	switch (op->getOp()[0]) {
	case '+': return left + right; break;
	case '-': return left - right; break;
	case '*': return left * right; break;
	case '/': return left / right; break;
	default: return 0.0;
	}
}

double PrintVisitor::visitFPNumber(FPNumber* num) {
	std::cout << num->getVal() << " ";
	return 0.0;
}
double PrintVisitor::visitBiOperation(BiOperation* op) {
	op->getLeft()->accept(this);
	op->getRight()->accept(this);
	std::cout << op->getOp() << " ";
	return 0.0;
}

Expr* init_fpnumber(std::string v) {
	return new FPNumber(v);
}

Expr* init_bioperation(std::string c, Expr* l, Expr* r) {
	return new BiOperation(c, l, r);
}
