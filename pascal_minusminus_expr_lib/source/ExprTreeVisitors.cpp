#include "ExprTreeVisitors.h"
#include <iostream>

double FPNumber::getVal() const { return std::stod(val); }

ExprRetVal FPNumber::accept(Visitor* v) {
	return (double)v->visitFPNumber(this);
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


ExprRetVal CalcVisitor::visitFPNumber(FPNumber* num) {
	return ExprRetVal(num->getVal());
}
ExprRetVal CalcVisitor::visitBiOperation(BiOperation* op) {
	if (!table_inited) { var_table = ExprVarTable(); table_inited = true; }

	ExprRetVal left = op->getLeft()->accept(this);
	ExprRetVal right = op->getRight()->accept(this);

	char oper = op->getOp()[0];
	if (oper == '=') {
		var_table.insert(ExprVarData((string)left, (double)right));
		return (string)left;
	}
	else { // из-за этого придется изменить парсер )
		double dl = (double)left, dr = (double)right;
		switch (oper) {
		case '+': return dl + dr;
		case '-': return dl - dr;
		case '*': return dl * dr;
		case '/': return dl / dr;

		case '>': return ExprRetVal(dl > dr);
		case '<': return ExprRetVal(dl < dr);

		default: return 0.0;
		}
	}
}
ExprRetVal CalcVisitor::visitVariable(Variable* var) {
	string name = var->getName();
	ExprRetVal res = ExprRetVal(name);
	if (var_table.contains(name)) 
		res.doub_val = var_table[name];
	return res;
}


ExprRetVal PrintVisitor::visitFPNumber(FPNumber* num) {
	std::cout << num->getVal() << " ";
	return 0.0;
}
ExprRetVal PrintVisitor::visitBiOperation(BiOperation* op) {
	op->getLeft()->accept(this);
	op->getRight()->accept(this);
	std::cout << op->getOp() << " ";
	return 0.0;
}
ExprRetVal PrintVisitor::visitVariable(Variable* var) {
	std::cout << var->getName() << " ";
	return 0.0;
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
