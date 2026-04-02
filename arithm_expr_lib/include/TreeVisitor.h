#pragma once
#include <iostream>
#include <string>

using namespace std;

struct ExprRetVal {
	string var_name;
	double doub_val;
	ExprRetVal(double _dv);
	ExprRetVal(string _vn);
};

class FPNumber;
class BiOperation;
class Variable;

class Visitor {
public:
	virtual ExprRetVal visitFPNumber(FPNumber* num) = 0;
	virtual ExprRetVal visitBiOperation(BiOperation* op) = 0;
	virtual ExprRetVal visitVariable(Variable* var) = 0;
};

class Expr {
public:
	virtual ExprRetVal accept(Visitor* v) = 0;
};

class FPNumber : public Expr {
	string val;
public:
	FPNumber(string _val) : val(_val) {}
	double getVal() const;
	virtual ExprRetVal accept(Visitor* v) override;
};

class BiOperation : public Expr {
	string op;
	Expr* left;
	Expr* right;
public:
	BiOperation(string _op, Expr* l, Expr* r) :
		op(_op), left(l), right(r) {}
	virtual ExprRetVal accept(Visitor* v) override;

	Expr* getLeft();
	Expr* getRight();

	string getOp() const;
};

class Variable : public Expr {
	string name;
public:
	Variable(string _n) : name(_n) {}
	virtual ExprRetVal accept(Visitor* v) override;
	string getName() const;
};

class CalcVisitor : public Visitor {
	//AVLTree<ExprRetVal> var_table;
	map<string, double> var_table;
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

Expr* init_fpnumber(string v);
Expr* init_bioperation(string c, Expr* l, Expr* r);
Expr* init_variable(string n);