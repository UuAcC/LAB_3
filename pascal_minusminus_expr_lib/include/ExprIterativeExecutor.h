#pragma once
#include <stack>
using namespace std;

#include "ExprBaseVisitor.h"
#include "ExprVarTable.h"

struct ExecNode {
	PMM_EXPR::Node* nod;
	bool status;
	ExecNode(PMM_EXPR::Node* n, bool st) : nod(n), status(st) {}
};

class IterativeExecutor : public Visitor {
	stack<NRV> exvalues;
	stack<ExecNode> exstack;
	bool last_status;

	ExprVarTable var_table;
	bool table_inited = false;
private:
	
public:
	IterativeExecutor() : last_status(false) {}
	void run(PMM_EXPR::ExprTree* tree);
};