#pragma once
#include <string>
#include "TTree.h"

using namespace std;

struct ExprVarData {
	string name;
	double value;
	ExprVarData(string _n) : name(_n) {}
	ExprVarData(string _n, double _v) : name(_n), value(_v) {}
	ExprVarData() : name(string()), value(double()) {}
	bool operator==(const ExprVarData& oth) const;
	bool operator!=(const ExprVarData& oth) const;
	bool operator>(const ExprVarData& oth) const;
	bool operator>=(const ExprVarData& oth) const;
	bool operator<(const ExprVarData& oth) const;
	bool operator<=(const ExprVarData& oth) const;
};

class ExprVarTable : public TTree<ExprVarData> {
	using TTree<ExprVarData>::root;
public:
	using TTree<ExprVarData>::insert;
	using TTree<ExprVarData>::outsert;
	using TTree<ExprVarData>::contains;
	double& operator[](const string& nm);
};