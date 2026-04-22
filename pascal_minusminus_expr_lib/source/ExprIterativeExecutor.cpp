#include "ExprIterativeExecutor.h"
#include "ExprExecExceprions.h"
#include "LexTypeDefines.h"

#define NODE PMM_EXPR::Node

NRV IterativeExecutor::visitFPNumber(PMM_EXPR::FPNumber* num) {
	if (!last_status) { exstack.push(ExecNode(num, 1)); }
	else { exvalues.push(NRV(num->getVal())); }
	return true;
}

NRV IterativeExecutor::visitVariable(PMM_EXPR::Variable* var) {
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

NRV IterativeExecutor::visitMon(PMM_EXPR::Mon* mon) {
	if (!last_status) {
		exstack.push(ExecNode(mon, 1)); 
		NODE* right = mon->getRight();
		if (right != nullptr) { exstack.push(ExecNode(right, 0)); }
		exstack.push(ExecNode(mon->getLeft(), 0));
	}
	else {
		if (!table_inited) { var_table = ExprVarTable(); table_inited = true; }
		if (mon->getRight() != nullptr) {
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
	}
	return true;
}

NRV IterativeExecutor::visitPol(PMM_EXPR::Pol* pol) {
	if (!last_status) {
		exstack.push(ExecNode(pol, 1));
		NODE* right = pol->getRight();
		if (right != nullptr) { exstack.push(ExecNode(right, 0)); }
		exstack.push(ExecNode(pol->getLeft(), 0));
	}
	else {
		if (!table_inited) { var_table = ExprVarTable(); table_inited = true; }
		if (pol->getRight() != nullptr) {
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
	}
	return true;
}

NRV IterativeExecutor::visitComp(PMM_EXPR::Comp* comp) {
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

NRV IterativeExecutor::visitEqOper(PMM_EXPR::EqOper* eqop) {
	if (!last_status) {
		exstack.push(ExecNode(eqop, 1));
		exstack.push(ExecNode(eqop->getRight(), 0));
		exstack.push(ExecNode(eqop->getLeft(), 0));
	}
	else {
		if (!table_inited) { var_table = ExprVarTable(); table_inited = true; }
		NRV right = exvalues.top(); exvalues.pop();
		NRV left = exvalues.top(); exvalues.pop();
		if (!right.hv_dval)
			throw new EEE_invalid_argument("variable \"" + (string)right + "\" is not initialized.\n");
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

NRV IterativeExecutor::visitIfOper(PMM_EXPR::IfOper* ifop) {
	if (!last_status) {
		exstack.push(ExecNode(ifop, 1));
		exstack.push(ExecNode(ifop->getLeft(), 0));
	}
	else {
		bool comp_res = (bool)exvalues.top(); exvalues.pop();
		if (comp_res) {
			exstack.push(ExecNode(ifop->getRight(), 0));
			exvalues.push(true);
		}
		else { exvalues.push(false); }
	}
	return true;
}

NRV IterativeExecutor::visitIfElse(PMM_EXPR::IfElse* ifel) {
	if (!last_status) {
		exstack.push(ExecNode(ifel, 1));
		exstack.push(ExecNode(ifel->getLeft(), 0));
	}
	else {
		bool ifoper_res = (bool)exvalues.top(); exvalues.pop();
		NODE* right = ifel->getRight();
		if (right != nullptr) {
			if (!ifoper_res) {
				exstack.push(ExecNode(right, 0));
			}
		}
	}
	return true;
}

NRV IterativeExecutor::visitWhileOper(PMM_EXPR::WhileOper* whop) {
	if (!last_status) {
		exstack.push(ExecNode(whop, 1));
		exstack.push(ExecNode(whop->getLeft(), 0));
	}
	else {
		bool comp_res = (bool)exvalues.top(); exvalues.pop();
		if (comp_res) {
			exstack.push(ExecNode(whop, 1));
			exstack.push(ExecNode(whop->getLeft(), 0));
			exstack.push(ExecNode(whop->getRight(), 0));
		}
	}
	return true;
}

NRV IterativeExecutor::visitOperator(PMM_EXPR::Operator* op) {
	exstack.push(ExecNode(op->getChild(), 0));
	return true;
}

NRV IterativeExecutor::visitExpr(PMM_EXPR::Expr* ex) {
	NODE* right = ex->getRight();
	if (right != nullptr)
		exstack.push(ExecNode(right, 0));
	exstack.push(ExecNode(ex->getLeft(), 0));
	return true;
}

NRV IterativeExecutor::visitTree(PMM_EXPR::ExprTree* extree) {
	exstack.push(ExecNode(extree->getChild(), 0));
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
