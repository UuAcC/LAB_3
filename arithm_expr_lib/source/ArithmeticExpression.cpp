#include "ArithmeticExpression.h"
#include "Parcer.h"

map<string, int> ArithmeticExpression::priority {
	{"(", 0}, {"+", 1}, {"-", 1}, {"*", 2},{"/", 2}
};

void ArithmeticExpression::to_postfix() {
	TStack<string> st(infix.size());
	string stackItem;
	for (string item : infix) {
		if (item == "(") { st.push(item); }
		if (item == ")") {
			stackItem = st.pop();
			while (stackItem != "(") {
				postfix.push_back(stackItem);
				stackItem = st.pop();
			}
		}
		if (item == "+" || item == "-" || item == "*" || item == "/") {
			while (!st.isEmpty()) {
				stackItem = st.top();
				if (priority[item] <= priority[stackItem])
					postfix.push_back(st.pop());
				else { break; }
			} st.push(item);
		}
		else { postfix.push_back(item); }
	}
	while (!st.isEmpty()) {
		stackItem = st.pop();
		postfix.push_back(stackItem);
	}
}

ArithmeticExpression::ArithmeticExpression(string _infix) {
	if (!skobochniy_check(_infix)) throw - 1;
	infix = Parcer::parce_infix(_infix); 
	to_postfix();
}

double ArithmeticExpression::calculate() {
	return 0.0;
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