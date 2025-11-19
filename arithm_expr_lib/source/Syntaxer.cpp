#include "ArithmeticExpression.h"

// zero, num, l_br, r_br, bop, mul, div

//        Таблица для конечного автомата syntax_check:
		// 
		//    |------|--------|---|---|--------|---|---|---|
		//    | next | [+, -] | * | / | [1..9] | 0 | ( | ) |
		//    |------|--------|---|---|--------|---|---|---|
		//    | ST0  |   ST2  |STx|STx|  ST1   |ST1|ST0|STx|
		//    |------|--------|---|---|--------|---|---|---|
		//    | ST1  |   ST2  |ST2|ST3|  STx   |STx|STx|ST1|
		//    |------|--------|---|---|--------|---|---|---|
		//    | ST2  |   STx  |STx|STx|  ST1   |ST1|ST0|STx|
		//    |------|--------|---|---|--------|---|---|---|
		//    | ST3  |   STx  |STx|STx|  ST1   |STx|ST0|STx|
		//    |------|--------|---|---|--------|---|---|---|

TQueue<ERROR> SYNTAXER::last_errors;
func_pointer SYNTAXER::sc_call(STATE st, TYPE tp) {
	return sc_funcs[(int)st * 7 + (int)tp - 1];
}
func_pointer SYNTAXER::sc_funcs[28]{
	sc_f0, sc_f0, sc_f0, sc_f1, sc_f0, sc_f1, sc_f1,
	sc_f1, sc_f1, sc_f1, sc_f0, sc_f0, sc_f0, sc_f0,
	sc_f0, sc_f0, sc_f0, sc_f1, sc_f1, sc_f1, sc_f1,
	sc_f1, sc_f0, sc_f0, sc_f1, sc_f1, sc_f1, sc_f1
};

// --------------------------------------------------------------------------------------------

bool SYNTAXER::skobochniy_check(const string& str) {
	const size_t n = str.size();
	TStack<int> s(n);
	try {
		for (size_t i = 0; i < n; i++) {
			if (str[i] == '(') s.push(i);
			if (str[i] == ')') s.pop();
			else continue;
		} return s.isEmpty();
	}
	catch (...) { return false; }
}
// --------------------------------------------------------------------------------------------

// ------------------------------- КОНЕЧНЫЙ АВТОМАТ SYNTAX_CHECK ------------------------------

inline void SYNTAXER::sc_f0(char c) { /* ничего не делает */ }
inline void SYNTAXER::sc_f1(char c) { throw c; }