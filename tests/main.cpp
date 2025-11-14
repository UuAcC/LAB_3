#include "ArithmeticExpression.h"

int main() {
	ArithmeticExpression _("(1234-745.43)+234+(-3)");
	TQueue<string> inf = _.get_infix(), postf = _.get_postfix();
	cout << inf << endl << postf;
} 