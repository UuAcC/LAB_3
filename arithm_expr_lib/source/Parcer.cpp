#include "ArithmeticExpression.h"

#define CHAR_IN_OPS decode(c) >= 3 && decode(c) <= 5
#define CHAR_IN_INTS decode(c) >= 0 && decode(c) <= 2
 
#define PARCER ArithmeticExpression::Parcer

#define STATE PARCER::state
#define TYPE PARCER::type

// ------------------------------- КОНЕЧНЫЙ АВТОМАТ PARCE_INFIX -------------------------------

string PARCER::pi_s_buffer;
vector<string> PARCER::pi_v_buffer;

inline void PARCER::pi_f0(char c) { pi_v_buffer.push_back(string(1, c)); }
inline void PARCER::pi_f1(char c) { pi_s_buffer = string(1, c); }
inline void PARCER::pi_f2(char c) {
	pi_v_buffer.push_back(pi_s_buffer);
	pi_v_buffer.push_back(string(1, c));
	pi_s_buffer.clear();
}
inline void PARCER::pi_f3(char c) { pi_s_buffer += c; }

TYPE PARCER::decode(char c) {
	if (c >= '1' && c <= '9')
		return NUM;
	switch (c) {
	case '0': return ZERO;
	case '(': return L_BR;
	case ')': return R_BR;
	case '.': return DOT;
	case '+': 
	case '-':
	case '*':
	case '/': return OP;
	default: throw runtime_error("DECODING_ERROR: unknown char " + c);
	}
}

STATE PARCER::pi_next(char c) {
	if (CHAR_IN_OPS) { return ST0; }
	if (CHAR_IN_INTS) { return ST1; }
	else throw -1;
}

func_pointer PARCER::pi_call(state st, char c) {
	if (st == ST0) {
		if (CHAR_IN_OPS) { return Parcer::pi_f0; }
		if (CHAR_IN_INTS) { return Parcer::pi_f1; }
		else throw - 1;
	}
	else if (st == ST1) {
		if (CHAR_IN_OPS) { return Parcer::pi_f2; }
		if (decode) { return Parcer::pi_f3; }
		else throw - 1;
	}
}

TQueue<string> PARCER::parce_infix(const string& str) {
	pi_v_buffer.clear();
	pi_s_buffer.clear();
	// Парсинг строки в вектор лексем
	state st = ST0;
	for (int i = 0; i < str.size(); ++i) {
		pi_call(st, str[i])(str[i]);
		st = pi_next(str[i]);
	}
	if (!pi_s_buffer.empty()) 
		pi_v_buffer.push_back(pi_s_buffer);
	pi_s_buffer.clear();
	// Обработка унарных операторов             
	auto iterator = pi_v_buffer.begin();
	if (pi_v_buffer[0] == "-" || pi_v_buffer[0] == "+") {
		pi_v_buffer.insert(iterator, "0");
	}
	for (int i = 1; i < pi_v_buffer.size(); ++i) {
		if (pi_v_buffer[i - 1] == "(" && (pi_v_buffer[i] == "-" || pi_v_buffer[i] == "+")) {
			pi_v_buffer.insert(iterator + i, "0");
		}
	}
	// Очистка буфера и возврат значения
	TQueue<string> res(pi_v_buffer);
	pi_v_buffer.clear();
	return res;
}
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------

bool PARCER::skobochniy_check(const string& str) {
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

// ------------------------------- КОНЕЧНЫЙ АВТОМАТ TO_DOUBLE ---------------------------------

double PARCER::buffer_value = 0.0;
double PARCER::buffer_dot = 1.0;
// std::function<void(state, char)> 
map<TYPE, map<STATE, STATE>> PARCER::td_next{ 
	//          ST0         ST1         ST2         ST3
	{ZERO, { {ST0, ST2}, {ST1, ST1}, {ST2, STX}, {ST3, ST3} } },
	{NUM, { {ST0, ST1}, {ST1, ST1}, {ST2, STX}, {ST3, ST3} } },
	{DOT, { {ST0, STX}, {ST1, ST3}, {ST2, ST3}, {ST3, STX} } }
};

func_pointer PARCER::td_call(state st, char c) {
	switch (st) {
	case ST0:
		if (CHAR_IN_INTS) { return PARCER::td_f0; }
		else { return PARCER::td_f3; }
		break;
	case ST1:
		if (CHAR_IN_INTS) { return PARCER::td_f0; }
		else { return PARCER::td_f2; }
		break;
	case ST2:
		if (CHAR_IN_INTS) { return PARCER::td_f3; }
		else { return PARCER::td_f2; }
		break;
	case ST3:
		if (CHAR_IN_INTS) { return PARCER::td_f1; }
		else { return PARCER::td_f3; }
		break;
	default: return PARCER::td_f3;
	}
}

inline void PARCER::td_f0(char c) { buffer_value *= 10; buffer_value += switch_char(c); }
inline void PARCER::td_f1(char c) { buffer_dot *= 10; td_f0(c); }
inline void PARCER::td_f2(char c) { /* ничего не делает */ }
inline void PARCER::td_f3(char c) { throw c; }

inline double PARCER::switch_char(char c) {
	switch (c) {
	case '1': return 1.0;
	case '2': return 2.0;
	case '3': return 3.0;
	case '4': return 4.0;
	case '5': return 5.0;
	case '6': return 6.0;
	case '7': return 7.0;
	case '8': return 8.0;
	case '9': return 9.0;
	default: return 0.0;
	}
}

double PARCER::to_double(const string& str) {
	buffer_dot = 1.0;
	buffer_value = 0.0;

	state st = ST0;
	for (int i = 0; i < str.size(); ++i) {
		td_call(st, str[i])(str[i]);
		st = td_next[decode(str[i])][st];
		if (st == STX) { throw str[i]; }
	}

	double res = buffer_value / buffer_dot;
	buffer_dot = 1.0;
	buffer_value = 0.0;
	return res;
}
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------

TQueue<string> PARCER::parce_postfix(const string& str) {
	TQueue<string> inf = parce_infix(str);
	size_t sz = inf.get_size();
	TQueue<string> postf(sz);
	TStack<string> st(sz);
	string stackItem;
	while (!inf.isEmpty()) {
		string item = inf.pop();  // все строки менять на class lexem
		/*type item_type = decode(item);*/
		if (item == "(") { st.push(item); }
		else if (item == ")") {
			stackItem = st.pop();
			while (stackItem != "(") {
				postf.push(stackItem);
				stackItem = st.pop();
			} 
		}
		else if (item == "+" || item == "-" || item == "*" || item == "/") {
			while (!st.isEmpty()) {
				stackItem = st.top();
				if (priority[item] <= priority[stackItem])
					postf.push(st.pop());
				else { break; }
			} st.push(item);
		}
		else { postf.push(item); }
	}
	while (!st.isEmpty()) {
		stackItem = st.pop();
		postf.push(stackItem);
	}
	return postf;
}

TQueue<string> PARCER::parce_postfix(TQueue<string> inf) {
	size_t sz = inf.get_size();
	TQueue<string> postf(sz);
	TStack<string> st(sz);
	string stackItem;
	while (!inf.isEmpty()) {
		string item = inf.pop();
		if (item == "(") { st.push(item); }
		else if (item == ")") {
			stackItem = st.pop();
			while (stackItem != "(") {
				postf.push(stackItem);
				stackItem = st.pop();
			}
		}
		else if (item == "+" || item == "-" || item == "*" || item == "/") {
			while (!st.isEmpty()) {
				stackItem = st.top();
				if (priority[item] <= priority[stackItem])
					postf.push(st.pop());
				else { break; }
			} st.push(item);
		}
		else { postf.push(item); }
	}
	while (!st.isEmpty()) {
		stackItem = st.pop();
		postf.push(stackItem);
	}
	return postf;
}
// --------------------------------------------------------------------------------------------