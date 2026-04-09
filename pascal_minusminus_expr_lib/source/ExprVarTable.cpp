#include "ExprVarTable.h"

bool ExprVarData::operator==(const ExprVarData& oth) const {
	return this->name == oth.name;
}
bool ExprVarData::operator>(const ExprVarData& oth) const {
	return this->name > oth.name;
}
bool ExprVarData::operator<(const ExprVarData& oth) const {
	return this->name < oth.name;
}
bool ExprVarData::operator>=(const ExprVarData& oth) const {
	return *this > oth || *this == oth;
}
bool ExprVarData::operator<=(const ExprVarData& oth) const {
	return *this < oth || *this == oth;
}
bool ExprVarData::operator!=(const ExprVarData& oth) const {
	return !(*this == oth);
}

double& ExprVarTable::operator[](const string& nm) {
	if (root == nullptr) { throw TREE_IS_EMPTY; }
	TreeNode<ExprVarData>* t = root;
	ExprVarData v(nm);
	while (t != nullptr) {
		if (t->val == v) return t->val.value;
		else if (t->val > v) t = t->l;
		else t = t->r;
	}
	throw VALUE_NOT_FOUND;
}