#pragma once
#include "ExprTreeNodes.h"
#include "ExprVarTable.h"

class Visitor {
public:
	virtual ExprRetVal visitFPNumber(FPNumber* num) = 0;
	virtual ExprRetVal visitBiOperation(BiOperation* op) = 0;
	virtual ExprRetVal visitVariable(Variable* var) = 0;
};

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