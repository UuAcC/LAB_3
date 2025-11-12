#include "ArithmeticExpression.h"

map<string, int> ArithmeticExpression::priority{
	{"(", 0}, {"+", 1}, {"-", 1}, {"*", 2},{"/", 2}
};

void ArithmeticExpression::to_postfix() {
	TStack<string> st(infix.size());
	string stackItem;
	for (string item : infix) {
		if (item == "(") { st.push(item); }
		else if (item == ")") {
			stackItem = st.pop();
			while (stackItem != "(") {
				postfix.push_back(stackItem);
				stackItem = st.pop();
			}
		}
		else if (item == "+" || item == "-" || item == "*" || item == "/") {
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
	if (!Parcer::skobochniy_check(_infix)) 
		throw runtime_error("Failed skobochniy_check");
	infix = Parcer::parce_infix(_infix);
	to_postfix();
}

double ArithmeticExpression::calculate() {
	return 0.0;
}

