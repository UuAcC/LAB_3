#include "ArithmeticExpression.h"

int main() {
	ArithmeticExpression _("(1234-745)+234");
	vector<string> inf = _.get_infix(), postf = _.get_postfix();
	for (int i = 0; i < inf.size(); ++i)
		cout << inf[i] << " ";
	cout << endl;
	for (int i = 0; i < postf.size(); ++i)
		cout << postf[i] << " ";
} 