#pragma once
#include "ExprTreeNodes.h"

class Visitor {
public:
	virtual ExprRetVal visitFPNumber(FPNumber* num) = 0;
	virtual ExprRetVal visitBiOperation(BiOperation* op) = 0;
	virtual ExprRetVal visitVariable(Variable* var) = 0;
};