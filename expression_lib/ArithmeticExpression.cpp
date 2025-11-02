#include "ArithmeticExpression.h"

map<char, int> ArithmeticExpression::priority{
	/*{'(', 0},*/ {'+', 1}, {'-', 1}, {'*', 2},{'/', 2}
};

void ArithmeticExpression::parse() {
	for (char c : infix) {
		lexems.push_back(c);
	}
}

void ArithmeticExpression::to_postfix() {
	if (!skobochniy_check(infix)) throw -1;
	parse();
	TStack<char> st(postfix.size());
	char stackItem;
	for (char item : lexems) {
		switch (item) {
		case '(':
			st.push(item);
			break;
		case ')':
			stackItem = st.pop();
			while (stackItem != '(') {
				postfix += stackItem;
				stackItem = st.pop();
			} break;
		case '+': case '-': case '*': case '/':
			while (!st.isEmpty()) {
				stackItem = st.pop();
				if (priority[item] <= priority[stackItem])
					postfix += stackItem;
				else { st.push(stackItem); break; }
			} st.push(item); break;
		default:
			operands.insert({ item, 0.0 });
			postfix += item;
		}
	}
	while (!st.isEmpty()) {
		stackItem = st.pop();
		postfix += stackItem;
	}
}

ArithmeticExpression::ArithmeticExpression(string _infix) {
	infix = _infix;
	to_postfix();
}

vector<char> ArithmeticExpression::get_operands() const {
	vector<char> op;
	for (const auto& lex : operands) {
		op.push_back(lex.first);
	} return op;
}

double ArithmeticExpression::calculate(const map<char, double>& values) {
	
}
