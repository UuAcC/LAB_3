#include "ArithmeticExpression.h"

ArithmeticExpression::ArithmeticExpression(string _infix) {
	if (!Syntaxer::skobochniy_check(_infix))
		throw runtime_error("Failed skobochniy_check");
	infix = Parcer::parce_infix(_infix);
	postfix = Parcer::parce_postfix(infix);
}

double ArithmeticExpression::calculate() {
	return 0.0;
}
