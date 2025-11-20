#include "ArithmeticExpression.h"

ArithmeticExpression::ArithmeticExpression(string _infix) {
	// ВАЖНО: постфикс тут создается только на время отладки. После я уберу эту строку.
	s_infix = _infix;
	q_infix = Parcer::parce_infix(_infix);
}

bool ArithmeticExpression::run_syntaxer() {
	bool res = true;

	bool sk_c = Syntaxer::skobochniy_check(s_infix); res &= sk_c;
	if (!sk_c) { Syntaxer::print_error_message(); }
	else { cerr << "skobochniy_check successful." << endl; }

	bool sy_c = Syntaxer::syntax_check(q_infix); res &= sy_c;
	if (!sy_c) { Syntaxer::print_error_message(); }
	else { cerr << "syntax_check successful." << endl; }

	return res;
}

double ArithmeticExpression::calculate() {
	return 0.0;
}
