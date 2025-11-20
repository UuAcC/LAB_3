#include "ArithmeticExpression.h"

string SYNTAXER::last_func;
TQueue<ERROR> SYNTAXER::last_errors;
using syntaxer_fp = void(*)(LEXEM);

void SYNTAXER::print_error_message() {
	cerr << last_func << " failure:" << endl;
	cerr << "Errors: " << last_errors << endl;
}

// --------------------------------------------------------------------------------------------

bool SYNTAXER::skobochniy_check(const string& str) {
	last_func = __func__;
	last_errors.clear();

	size_t n = str.size();
	TStack<error> res_o(n);
	TStack<error> res_c(n);

	TQueue<error> curr_errors(n);

	for (int i = 0; i < n; i++) {
		if (str[i] == '(') {
			try { res_o.push(error(i, "(")); }
			catch (...) {}
		}
		if (str[i] == ')') {
			try { res_o.pop(); }
			catch (...) { res_c.push(error(i, ")")); }
		}
		else continue;
	}
	error* arr = new error[n]();
	while (!res_o.isEmpty()) { error curr = res_o.pop(); arr[curr.index] = curr; }
	while (!res_c.isEmpty()) { error curr = res_c.pop(); arr[curr.index] = curr; }
	for (int i = 0; i < n; ++i) {
		string cv = arr[i].value;
		if (cv == "(" || cv == ")") { curr_errors.push(arr[i]); }
	}
	last_errors = curr_errors;
	return curr_errors.isEmpty();
}
// --------------------------------------------------------------------------------------------

// ------------------------------- ÊÎÍÅ×ÍÛÉ ÀÂÒÎÌÀÒ SYNTAX_CHECK ------------------------------

inline void SYNTAXER::sc_f0(LEXEM lex) { /* íè÷åãî íå äåëàåò */ }
inline void SYNTAXER::sc_f1(LEXEM lex) { throw lex; }

syntaxer_fp SYNTAXER::sc_call(STATE st, TYPE tp) {
	return sc_funcs[(int)st * 7 + (int)tp - 1];
}

syntaxer_fp SYNTAXER::sc_funcs[28]{
	sc_f0, sc_f0, sc_f0, sc_f1, sc_f0, sc_f1, sc_f1,
	sc_f1, sc_f1, sc_f1, sc_f0, sc_f0, sc_f0, sc_f0,
	sc_f0, sc_f0, sc_f0, sc_f1, sc_f1, sc_f1, sc_f1,
	sc_f1, sc_f0, sc_f0, sc_f1, sc_f1, sc_f1, sc_f1
};

STATE SYNTAXER::sc_next(STATE st, TYPE tp) {
	return sc_states[(int)st * 7 + (int)tp - 1];
}

STATE SYNTAXER::sc_states[28]{
	ST1, ST1, ST0, STX, ST2, STX, STX,
	STX, STX, STX, ST1, ST2, ST2, ST3,
	ST1, ST1, ST0, STX, STX, STX, STX,
	STX, ST1, ST0, STX, STX, STX, STX,
};

bool SYNTAXER::syntax_check(TQueue<LEXEM> que) {
	last_func = __func__;
	last_errors.clear();
	TQueue<error> curr_errors(que.get_size());
	size_t i = 0; lexem curr;
	state st = ST0;
	while (!que.isEmpty()) {
		curr = que.pop();
		try { 
			sc_call(st, curr.type)(curr); 
			st = sc_next(st, curr.type);
		}
		catch (const lexem& lex) { curr_errors.push(error(i, curr.value)); }
		i++;
	}
	if (st != ST1) { curr_errors.push(error(i, curr.value)); }
	last_errors = curr_errors;
	return last_errors.isEmpty();
}
