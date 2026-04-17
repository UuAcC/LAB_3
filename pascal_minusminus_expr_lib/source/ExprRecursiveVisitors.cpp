#include <iostream>
#include "ExprRecursiveVisitors.h"
#include "ExprExecExceprions.h"

ExprRetVal CalcVisitor::visitFPNumber(FPNumber* num) {
	return ExprRetVal(num->getVal());
}

ExprRetVal CalcVisitor::visitBiOperation(BiOperation* op) {
	if (!table_inited) { var_table = ExprVarTable(); table_inited = true; }

	char oper = op->getOp()[0];

	ExprRetVal left = op->getLeft()->accept(this);
	ExprRetVal right = op->getRight()->accept(this);

	if (!right.hv_dval && oper != ';')
		throw new EEE_invalid_argument("variable \"" + (string)right + "\" is not initialized.\n");
	double dr = (double)right;

	ExprRetVal result;
	if (oper == '=') {
		string ls = (string)left;
		if (var_table.contains(ls)) { var_table[ls] = dr; }
		else { var_table.insert(ExprVarData(ls, dr)); }

		std::cerr << ls << " = " << dr << ";\n";
		result.set_var_name(ls);
	}
	else {
		double dl = (double)left;
		switch (oper) {
		case '+': result.set_doub_val(dl + dr); break;
		case '-': result.set_doub_val(dl - dr); break;
		case '*': result.set_doub_val(dl * dr); break;
		case '/': result.set_doub_val(dl / dr); break;
		case '>': result.set_bool_val(dl > dr); break;
		case '<': result.set_bool_val(dl < dr); break;
		default: break;
		}
	}
	return result;
}

ExprRetVal CalcVisitor::visitVariable(Variable* var) {
	string name = var->getName();
	ExprRetVal res = ExprRetVal(name);
	if (var_table.contains(name))
		res.set_doub_val(var_table[name]);
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