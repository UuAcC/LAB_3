#include <gtest/gtest.h>
#include "Pascal_MinusMinus_Expression.h"

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

void run_PMME_array(string arr[], int sz) {
	cout << endl;
	for (int i = 0; i < sz; ++i) {
		cout << arr[i] << endl;
		PMM_EXPR _(arr[i]);
		bool run_res = _.run_analyzer(1);
		if (run_res) {
			//cout << endl;

			TQueue<LEXEM> inf = _.get_q_infix();

			//print_lexem_queue(cout, inf);
			//cout << endl << endl;

			TQueue<LEXEM> postf = _.get_q_postfix();

			//print_lexem_queue(cout, postf);
			//cout << endl << endl;

			_.execute(0);
			cout << endl;
		}
	}
}

// ((325 - 45.231) * 78 - 23434 / 1.00)

TEST(PMM_EXPR, Previous_main) {
	string arr[3];
	arr[0] = "a = 34; b = a + a; a = b";
	arr[1] = "a = ((325 - 45.231) * 78 - 23434 / 1.00); b = a + 1";
	arr[2] = "a = b; b = a + 1";
	run_PMME_array(arr, 3);
}
