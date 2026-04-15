#include "ExprIterativeExecutor.h"
#include "ExprExecExceprions.h"

ExprRetVal IterativeExecutor::visitFPNumber(FPNumber* num) {
	if (!last_status) { exstack.push(ExecNode(num, 1)); }
	else { exvalues.push(ExprRetVal(num->getVal())); }
	return 0.0;
}

ExprRetVal IterativeExecutor::visitVariable(Variable* var) {
	if (!last_status) { exstack.push(ExecNode(var, 1)); }
	else {
		string name = var->getName();
		ExprRetVal res = ExprRetVal(name);
		if (var_table.contains(name))
			res.set_doub_val(var_table[name]);
		exvalues.push(res);
	}
	return 0.0;
}

ExprRetVal IterativeExecutor::visitBiOperation(BiOperation* op) {
	if (!last_status) {
		exstack.push(ExecNode(op, 1)); 
		exstack.push(ExecNode(op->getRight(), 0));
		exstack.push(ExecNode(op->getLeft(), 0));
	}
	else {
		if (!table_inited) { var_table = ExprVarTable(); table_inited = true; }

		ExprRetVal right = exvalues.top(); exvalues.pop();
		ExprRetVal left = exvalues.top(); exvalues.pop();

		char oper = op->getOp()[0];

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
		exvalues.push(result);
	}
	return 0.0;
}

void IterativeExecutor::run(Expr* tree) {
	exstack.push(ExecNode(tree, 0));
	while (!exstack.empty()) {
		ExecNode curr = exstack.top(); exstack.pop();
		last_status = curr.status;
		curr.exp->accept(this);
	}
}

// Я этот обход
// ненавижу