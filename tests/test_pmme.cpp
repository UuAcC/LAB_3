#include "Pascal_MinusMinus_Expression.h"
#include <gtest/gtest.h>

// Специализация не создавалась, т.к. шаблон в глобальном пространстве имен, 
// а специализация в Pascal_MinusMinus_Expression.
// Она выглядела так:
// 
// template<>
// ostream& operator<<(std::ostream& ostr, TQueue<LEXEM>& q) { тут какой-то код }

// Далее пытался сделать перегрузку, но уперся в ошибку:
// error C2084: функция "std::ostream &TQueue<Pascal_MinusMinus_Expression::lexem>::operator <<(std::ostream &,TQueue<Pascal_MinusMinus_Expression::lexem> &)" уже имеет текст реализации

// В итоге оказалось, что я час крутил функцию, которая мне в дальнейшем будет не нужна.
// Вряд ли я это даже покажу.

void print_lexem_queue(ostream& ostr, TQueue<LEXEM>& q) {
	TQueue<LEXEM> tmp(q);
	size_t sz = q.get_size();
	TQueue<string> vals(sz);
	TQueue<LEX_TYPE> tps(sz);
	while (!tmp.isEmpty()) {
		LEXEM curr = tmp.pop();
		vals.push(curr.value);
		tps.push(curr.type);
	}
	ostr << "values: " << vals;
	ostr << endl;
	ostr << "types: " << tps;
}

// ((325 - 45.231) * 78 - 23434 / 1.00)

TEST(Pascal_MinusMinus_Expression, Previous_main) {
	string str;
	/*getline(cin, str);*/
	str = "-)-3242  (+ 3*(43  - 2723).4353)";

	Pascal_MinusMinus_Expression _(str);
	bool run_res = _.run_analyzer(1);
	if (run_res) {
		cout << endl;
		TQueue<LEXEM> inf = _.get_q_infix();
		print_lexem_queue(cout, inf);
		cout << endl << endl;
		TQueue<LEXEM> postf = _.get_q_postfix();
		print_lexem_queue(cout, postf);
		cout << endl << endl;
		_.execute();
	}
}

#include <cmath>