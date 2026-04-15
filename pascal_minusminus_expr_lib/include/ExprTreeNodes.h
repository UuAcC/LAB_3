#pragma once
#include <string>

using std::string;

struct ExprRetVal {
	string var_name;
	double doub_val;
	bool bool_value;

	bool hv_name = false;
	bool hv_dval = false;
	bool hv_bval = false;

	ExprRetVal() {}
	ExprRetVal(string _vn) : var_name(_vn), hv_name(true) {}
	ExprRetVal(double _dv) : doub_val(_dv), hv_dval(true) {}
	ExprRetVal(bool _bv) : bool_value(_bv), hv_bval(true) {}

	inline explicit operator string() const { return var_name; }
	inline explicit operator double() const { return doub_val; }
	inline explicit operator bool() const { return bool_value; }

	inline void set_var_name(string _vn) { var_name = _vn; hv_name = true; }
	inline void set_doub_val(double _dv) { doub_val = _dv; hv_dval = true; }
	inline void set_bool_val(bool _bv) { bool_value = _bv; hv_bval = true; }
};

class Visitor;

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
	BiOperation(string _op, Expr* l, Expr* r) : op(_op), left(l), right(r) {}
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

Expr* init_fpnumber(string v);
Expr* init_bioperation(string c, Expr* l, Expr* r);
Expr* init_variable(string n);