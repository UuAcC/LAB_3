#include "ExprIterativeExecutor.h"
#include "ExprExecExceprions.h"
#include "LexTypeDefines.h"

#define NODE PMM_EXPR::Node

#define FPNUMBER PMM_EXPR::FPNumber
NRV IterativeExecutor::visitFPNumber(FPNUMBER* num) {
	if (!last_status) { exstack.push(ExecNode(num, 1)); }
	else { exvalues.push(NRV(num->getVal())); }
	return true;
}

#define VARIABLE PMM_EXPR::Variable
NRV IterativeExecutor::visitVariable(VARIABLE* var) {
	if (!last_status) { exstack.push(ExecNode(var, 1)); }
	else {
		string name = var->getName();
		NRV res = NRV(name);
		if (var_table.contains(name))
			res.set_doub_val(var_table[name]);
		exvalues.push(res);
	}
	return true;
}

#define MON PMM_EXPR::Mon
NRV IterativeExecutor::visitMon(MON* mon) {
	if (!last_status) { 
		exstack.push(ExecNode(mon, 1)); 
		NODE* right = mon->getRight();
		if (right != nullptr) { exstack.push(ExecNode(right, 0)); }
		exstack.push(ExecNode(mon->getLeft(), 0));
	}
	else {
		if (!table_inited) { var_table = ExprVarTable(); table_inited = true; }
		NRV right = exvalues.top(); exvalues.pop();
		NRV left = exvalues.top(); exvalues.pop();
		if (!right.hv_dval) 
			throw new EEE_invalid_argument("variable \"" + (string)right + "\" is not initialized.\n");
		double dr = (double)right;
		double dl = (double)left;
		NRV result;
		if (mon->getOp() == MUL) result.set_doub_val(dl * dr);
		else result.set_doub_val(dl / dr);
		exvalues.push(result);
	}
	return true;
}

#define POL PMM_EXPR::Pol
NRV IterativeExecutor::visitPol(POL* pol) {
	if (!last_status) {
		exstack.push(ExecNode(pol, 1));
		NODE* right = pol->getRight();
		if (right != nullptr) { exstack.push(ExecNode(right, 0)); }
		exstack.push(ExecNode(pol->getLeft(), 0));
	}
	else {
		if (!table_inited) { var_table = ExprVarTable(); table_inited = true; }
		NRV right = exvalues.top(); exvalues.pop();
		NRV left = exvalues.top(); exvalues.pop();
		if (!right.hv_dval)
			throw new EEE_invalid_argument("variable \"" + (string)right + "\" is not initialized.\n");
		double dr = (double)right;
		double dl = (double)left;
		NRV result;
		if (pol->getOp() == ADD) result.set_doub_val(dl + dr);
		else result.set_doub_val(dl - dr);
		exvalues.push(result);
	}
	return true;
}

#define COMP PMM_EXPR::Comp
NRV IterativeExecutor::visitComp(COMP* comp) {
	if (!last_status) {
		exstack.push(ExecNode(comp, 1));
		exstack.push(ExecNode(comp->getRight(), 0));
		exstack.push(ExecNode(comp->getLeft(), 0));
	}
	else {
		NRV right = exvalues.top(); exvalues.pop();
		NRV left = exvalues.top(); exvalues.pop();
		double dr = (double)right;
		double dl = (double)left;
		NRV result;
		switch (comp->getOp()) {
		case C_EQ: result.set_bool_val(dl == dr); break;
		case C_NEQ: result.set_bool_val(dl != dr); break;
		case C_L: result.set_bool_val(dl < dr); break;
		case C_LE: result.set_bool_val(dl <= dr); break;
		case C_M: result.set_bool_val(dl > dr); break;
		case C_ME: result.set_bool_val(dl >= dr); break;
		}
		exvalues.push(result);
	}
	return true;
}

#define EQOPER PMM_EXPR::EqOper
NRV IterativeExecutor::visitEqOper(EQOPER* eqop) {
	if (!last_status) {
		exstack.push(ExecNode(eqop, 1));
		NODE* right = eqop->getRight();
		if (right != nullptr) { exstack.push(ExecNode(right, 0)); }
		exstack.push(ExecNode(eqop->getLeft(), 0));
	}
	else {
		if (!table_inited) { var_table = ExprVarTable(); table_inited = true; }
		NRV right = exvalues.top(); exvalues.pop();
		NRV left = exvalues.top(); exvalues.pop();
		double dr = (double)right;
		string ls = (string)left;
		NRV result;
		if (var_table.contains(ls)) { var_table[ls] = dr; }
		else { var_table.insert(ExprVarData(ls, dr)); }
		std::cerr << ls << " = " << dr << ";\n";
		result.set_var_name(ls);
	}
	return true;
}


void IterativeExecutor::run(PMM_EXPR::ExprTree* tree) {
	exstack.push(ExecNode(tree, 0));
	while (!exstack.empty()) {
		ExecNode curr = exstack.top(); exstack.pop();
		last_status = curr.status;
		curr.nod->accept(this);
	}
}
