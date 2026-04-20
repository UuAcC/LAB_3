#pragma once
#include "Pascal_MinusMinus_Expression.h"

#define NRV PMM_EXPR::NodeRetVal

class Visitor {
public:
	virtual NRV visitFPNumber(PMM_EXPR::FPNumber* num) = 0;
	virtual NRV visitVariable(PMM_EXPR::Variable* var) = 0;
	
	virtual NRV visitMon(PMM_EXPR::Mon* mon) = 0;
	virtual NRV visitPol(PMM_EXPR::Pol* pol) = 0;
	virtual NRV visitComp(PMM_EXPR::Comp* comp) = 0;

	virtual NRV visitEqu(PMM_EXPR::EqOper* num) = 0;

	virtual NRV visitIfOper(PMM_EXPR::IfOper* num) = 0;
	virtual NRV visitIfElse(PMM_EXPR::IfElse* num) = 0;

	virtual NRV visitOperator(PMM_EXPR::Operator* num) = 0;
	virtual NRV visitExpr(PMM_EXPR::Expr* num) = 0;
	virtual NRV visitTree(PMM_EXPR::ExprTree* num) = 0;
};