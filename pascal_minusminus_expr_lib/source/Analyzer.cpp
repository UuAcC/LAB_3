#include "Pascal_MinusMinus_Expression.h"

string ANALYZER::last_func;
TQueue<ERROR> ANALYZER::last_errors;
using syntaxer_fp = void(*)(LEXEM);

void ANALYZER::print_error_message() {
	cerr << last_func;
	if (errors_occured()) {
		cerr << " failure:" << endl;
		cerr << "Errors: " << last_errors << endl;
	}
	else { cerr << " successful." << endl; }
}

// --------------------------------------------------------------------------------------------

bool ANALYZER::skobochniy_check(const string& str) {
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

// --------------------------------------------------------------------------------------------

bool ANALYZER::lexic_check(TQueue<LEXEM> que) {
	last_func = __func__;
	last_errors.clear();
	TQueue<error> curr_errors(que.get_size());

	lexem curr; size_t i = -1;
	while (!que.isEmpty()) {
		++i;
		curr = que.pop();
		if (curr.type == LEX_TYPE::zero || curr.type == LEX_TYPE::num) {
			PARCER::to_double(curr.value);
			if (PARCER::errors_occured())
				curr_errors.push(error(i, curr.value));
		}
		else if (curr.type == LEX_TYPE::variable) {
			int tp = (int)decode(curr.value[0]);
			if (CHAR_IN_INTS)
				curr_errors.push(error(i, curr.value));
			size_t sz = curr.value.size();
			for (int j = 1; j < sz; ++j) {
				if (curr.value[j] == '.')
					curr_errors.push(error(i, curr.value));
			}
		}
		else { continue; }
	}

	last_errors = curr_errors;
	return curr_errors.isEmpty();
}
// --------------------------------------------------------------------------------------------

// ---------------------------- Finite state machine for SYNTAX_CHECK -------------------------

inline void ANALYZER::sc_f0(LEXEM lex) { /* Do nothing */ }
inline void ANALYZER::sc_f1(LEXEM lex) { throw lex; }

syntaxer_fp ANALYZER::sc_call(STATE st, LEX_TYPE tp) {
	return sc_funcs[(int)st * 10 + (int)tp - 1];
}

syntaxer_fp ANALYZER::sc_funcs[60]{
	sc_f0, sc_f0, sc_f0, sc_f0, sc_f1, sc_f0, sc_f1, sc_f1, sc_f1, sc_f1,
	sc_f1, sc_f1, sc_f1, sc_f1, sc_f1, sc_f0, sc_f0, sc_f0, sc_f0, sc_f1,
	sc_f0, sc_f0, sc_f0, sc_f0, sc_f1, sc_f0, sc_f1, sc_f1, sc_f1, sc_f1,
	sc_f1, sc_f1, sc_f1, sc_f1, sc_f0, sc_f0, sc_f0, sc_f0, sc_f1, sc_f0,
	sc_f0, sc_f0, sc_f0, sc_f0, sc_f1, sc_f1, sc_f1, sc_f1, sc_f1, sc_f1,
	sc_f1, sc_f0, sc_f0, sc_f0, sc_f1, sc_f1, sc_f1, sc_f1, sc_f1, sc_f1
};

STATE ANALYZER::sc_next(STATE st, LEX_TYPE tp) {
	return sc_states[(int)st * 10 + (int)tp - 1];
}

STATE ANALYZER::sc_states[60]{
	ST3, ST3, ST1, ST2, STX, ST4, STX, STX, STX, STX,
	STX, STX, STX, STX, STX, ST4, ST4, ST5, ST2, STX,
	ST3, ST3, ST3, ST2, STX, ST4, STX, STX, STX, STX,
	STX, STX, STX, STX, ST3, ST4, ST4, ST5, STX, ST0,
	ST3, ST3, ST3, ST2, STX, STX, STX, STX, STX, STX,
	STX, ST3, ST3, ST2, STX, STX, STX, STX, STX, STX
};

bool ANALYZER::syntax_check(TQueue<LEXEM> que) {
	last_func = __func__;
	last_errors.clear();
	TQueue<error> curr_errors(que.get_size());
	size_t i = -1; lexem curr;
	state st = ST0;
	while (!que.isEmpty()) {
		++i; curr = que.pop();
		try {
			sc_call(st, curr.type)(curr);
			st = sc_next(st, curr.type);
		}
		catch (const lexem& lex) { curr_errors.push(error(i, curr.value)); }
	}
	if (st != ST3 && (curr_errors.top().index != i)) { curr_errors.push(error(i, curr.value)); }
	last_errors = curr_errors;
	return last_errors.isEmpty();
}
// --------------------------------------------------------------------------------------------