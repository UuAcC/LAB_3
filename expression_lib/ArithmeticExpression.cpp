#include "ArithmeticExpression.h"
#include "Parcer.h"

map<char, int> ArithmeticExpression::priority {
	/*{'(', 0},*/ {'+', 1}, {'-', 1}, {'*', 2},{'/', 2}
};

void ArithmeticExpression::to_postfix() {
	if (!skobochniy_check(infix.)) throw -1;
	lexems = Parcer::parce_infix(infix);
	
}

ArithmeticExpression::ArithmeticExpression(string _infix) {
	infix = Parcer::parce_infix(_infix); to_postfix();
}

double ArithmeticExpression::calculate(const map<char, double>& values) {
	
}

static bool skobochniy_check(std::string str) {
	const size_t n = str.size();
	TStack<int> s(n);
	try {
		for (size_t i = 0; i < n; i++) {
			if (str[i] == '(') s.push(i);
			if (str[i] == ')') s.pop();
			else continue;
		} return s.isEmpty();
	}
	catch (...) { return false; }
}