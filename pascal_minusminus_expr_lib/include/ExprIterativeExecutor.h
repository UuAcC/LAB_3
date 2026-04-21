#pragma once
#include <stack>
using namespace std;

#include "ExprBaseVisitor.h"
#include "ExprVarTable.h"

struct ExecNode {
	PMM_EXPR::Node* nod;
	int status;
	ExecNode(PMM_EXPR::Node* n, int st) : nod(n), status(st) {}
};

class IterativeExecutor : public Visitor {
	stack<NRV> exvalues;
	stack<ExecNode> exstack;
	int last_status;

	ExprVarTable var_table;
	bool table_inited = false;
private:
	virtual NRV visitFPNumber(PMM_EXPR::FPNumber* num) override;
	virtual NRV visitVariable(PMM_EXPR::Variable* var) override;

	virtual NRV visitMon(PMM_EXPR::Mon* mon) override;
	virtual NRV visitPol(PMM_EXPR::Pol* pol) override;
	virtual NRV visitComp(PMM_EXPR::Comp* comp) override;

	virtual NRV visitEqOper(PMM_EXPR::EqOper* num) override;

	virtual NRV visitIfOper(PMM_EXPR::IfOper* num) override;
	virtual NRV visitIfElse(PMM_EXPR::IfElse* num) override;
	virtual NRV visitWhileOper(PMM_EXPR::WhileOper* num) override;

	virtual NRV visitOperator(PMM_EXPR::Operator* num) override;
	virtual NRV visitExpr(PMM_EXPR::Expr* num) override;
	virtual NRV visitTree(PMM_EXPR::ExprTree* num) override;
public:
	IterativeExecutor() : last_status(false) {}
	void run(PMM_EXPR::ExprTree* tree);
};