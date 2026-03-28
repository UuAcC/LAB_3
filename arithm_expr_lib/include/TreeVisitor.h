#pragma once
#include <iostream>
#include <string>

class FPNumber;
class BiOperation;

//double to_double(std::string str);
// я не придумал, как их прилинковать друг к другу, использовал встроенное

class Visitor {
public:
	virtual double visitFPNumber(FPNumber* num) = 0;
	virtual double visitBiOperation(BiOperation* op) = 0;
};

class Expr {
public:
	virtual double accept(Visitor* v) = 0;
};

class FPNumber : public Expr {
	std::string val;
public:
	FPNumber(std::string _val) : val(_val) {}
	double getVal() const;
	virtual double accept(Visitor* v) override;
};

class BiOperation : public Expr {
	std::string op;
	Expr* left;
	Expr* right;
public:
	BiOperation(std::string _op, Expr* l, Expr* r) :
		op(_op), left(l), right(r) {}
	virtual double accept(Visitor* v) override;

	Expr* getLeft();
	Expr* getRight();

	std::string getOp() const;
};

class CalcVisitor : public Visitor {
public:
	virtual double visitFPNumber(FPNumber* num) override;
	virtual double visitBiOperation(BiOperation* op) override;
};

class PrintVisitor : public Visitor {
public:
	virtual double visitFPNumber(FPNumber* num) override;
	virtual double visitBiOperation(BiOperation* op) override;
};

Expr* init_fpnumber(std::string v);
Expr* init_bioperation(std::string c, Expr* l, Expr* r);