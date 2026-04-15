#pragma once
#include <stack>
using namespace std;

#include "ExprTreeNodes.h"
#include "ExprTreeVisitors.h"

struct ExecNode {
	Expr* exp;
	bool status;
	ExecNode(Expr* e, bool st) : exp(e), status(st) {}
};

class IterativeExecutor : public Visitor {
	stack<ExprRetVal> exvalues;
	stack<ExecNode> exstack;
	bool last_status;

	ExprVarTable var_table;
	bool table_inited = false;
private:
	virtual ExprRetVal visitFPNumber(FPNumber* num) override;
	virtual ExprRetVal visitBiOperation(BiOperation* op) override;
	virtual ExprRetVal visitVariable(Variable* var) override;
public:
	IterativeExecutor() : last_status(false) {}
	void run(Expr* tree);
};