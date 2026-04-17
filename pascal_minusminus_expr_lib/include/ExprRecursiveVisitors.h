#pragma once
#include "ExprBaseVisitor.h"
#include "ExprVarTable.h"

class CalcVisitor : public Visitor {
	ExprVarTable var_table;
	bool table_inited = false;
public:
	virtual ExprRetVal visitFPNumber(FPNumber* num) override;
	virtual ExprRetVal visitBiOperation(BiOperation* op) override;
	virtual ExprRetVal visitVariable(Variable* var) override;
};

class PrintVisitor : public Visitor {
public:
	virtual ExprRetVal visitFPNumber(FPNumber* num) override;
	virtual ExprRetVal visitBiOperation(BiOperation* op) override;
	virtual ExprRetVal visitVariable(Variable* var) override;
};