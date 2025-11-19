#include "ArithmeticExpression.h"

// Специализация не создавалась, т.к. шаблон в глобальном пространстве имен, 
// а специализация в ArithmeticExpression.
// Она выглядела так:
// 
// template<>
// ostream& operator<<(std::ostream& ostr, TQueue<LEXEM>& q) { тут какой-то код }

// Далее пытался сделать перегрузку, но уперся в ошибку:
// error C2084: функция "std::ostream &TQueue<ArithmeticExpression::lexem>::operator <<(std::ostream &,TQueue<ArithmeticExpression::lexem> &)" уже имеет текст реализации

// В итоге оказалось, что я час крутил функцию, которая мне в дальнейшем будет не нужна.
// Вряд ли я это даже покажу.

void print_lexem_queue(ostream& ostr, TQueue<LEXEM>& q) {
	TQueue<LEXEM> tmp(q);
	size_t sz = q.get_size();
	TQueue<string> vals(sz);
	TQueue<TYPE> tps(sz);
	while (!tmp.isEmpty()) {
		LEXEM curr = tmp.pop();
		vals.push(curr.value);
		tps.push(curr.type);
	}
	ostr << "values: " << vals;
	ostr << endl;
	ostr << "types: " << tps;
}

int main() {
	ArithmeticExpression _("-34+41*25");
	TQueue<LEXEM> inf = _.get_infix();
	print_lexem_queue(cout, inf);
	cout << endl << endl;
	TQueue<LEXEM> postf = _.get_postfix();
	print_lexem_queue(cout, postf);
} 