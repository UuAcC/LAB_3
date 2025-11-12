#include "ArithmeticExpression.h"

template<class T>
void print_vector(vector<T> vec, bool print_endl = 0) {
	for (int i = 0; i < vec.size(); ++i)
		cout << vec[i] << " ";
	if (print_endl) cout << endl;
}

int main() {
	ArithmeticExpression _("(1234-745.43)+234+(-3)");
	vector<string> inf = _.get_infix(), postf = _.get_postfix();
	print_vector(inf, 1);
	print_vector(postf);
} 