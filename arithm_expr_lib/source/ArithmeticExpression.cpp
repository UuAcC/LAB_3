#include "ArithmeticExpression.h"

map<string, int> ArithmeticExpression::priority{
	{"(", 0}, {"+", 1}, {"-", 1}, {"*", 2},{"/", 2}
};

ArithmeticExpression::ArithmeticExpression(string _infix) {
	if (!Parcer::skobochniy_check(_infix)) 
		throw runtime_error("Failed skobochniy_check");
	infix = Parcer::parce_infix(_infix);
	postfix = Parcer::parce_postfix(infix);
}

double ArithmeticExpression::calculate() {
	return 0.0;
}
