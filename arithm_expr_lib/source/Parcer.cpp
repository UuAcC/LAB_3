#include "ArithmeticExpression.h"

#define CHAR_IN_OPS tp >= 3 && tp <= 7
#define CHAR_IN_INTS tp >= 0 && tp <= 2

#define ZERO Type::zero
#define NUM Type::num
#define DOT Type::dot
#define L_BR Type::l_br
#define R_BR Type::r_br
#define BOP Type::bop
#define MUL Type::mul
#define DIV Type::div

map<string, int> PARCER::priority{
	{"(", 0}, {"+", 1}, {"-", 1}, {"*", 2},{"/", 2}
};

string PARCER::last_func;
TQueue<ERROR> PARCER::last_errors;
using parcer_fp = void(*)(char);

void PARCER::print_error_message() {
	cerr << last_func << " failure:" << endl;
	cerr << "Errors: " << last_errors << endl;
}

// ------------------------------- КОНЕЧНЫЙ АВТОМАТ PARCE_INFIX -------------------------------

string PARCER::pi_s_buffer;
vector<LEXEM> PARCER::pi_v_buffer;

inline void PARCER::pi_f0(char c) { pi_v_buffer.push_back(lexem(string(1, c), decode(c))); }
inline void PARCER::pi_f1(char c) { pi_s_buffer = string(1, c); }
inline void PARCER::pi_f2(char c) {
	pi_v_buffer.push_back(lexem(pi_s_buffer, NUM));
	pi_v_buffer.push_back(lexem(string(1, c), decode(c)));
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
	case '-': return BOP;
	case '*': return MUL;
	case '/': return DIV;
	default: throw c;
	}
}

STATE PARCER::pi_next(char c) {
	int tp = (int)decode(c);
	if (CHAR_IN_OPS) { return ST0; }
	if (CHAR_IN_INTS) { return ST1; }
	else throw -1;
}

parcer_fp PARCER::pi_call(state st, char c) {
	int tp = (int)decode(c);
	if (st == ST0) {
		if (CHAR_IN_OPS) { return pi_f0; }
		if (CHAR_IN_INTS) { return pi_f1; }
		else throw - 1;
	}
	else if (st == ST1) {
		if (CHAR_IN_OPS) { return pi_f2; }
		if (CHAR_IN_INTS) { return pi_f3; }
		else throw - 1;
	}
}

TQueue<LEXEM> PARCER::parce_infix(const string& str) {
	last_func = __func__;
	last_errors.clear();
	TQueue<ERROR> curr_errors(str.size());

	pi_v_buffer.clear();
	pi_s_buffer.clear();
	// Парсинг строки в вектор лексем
	state st = ST0;
	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] == ' ') continue;
		try { 
			pi_call(st, str[i])(str[i]); 
			st = pi_next(str[i]);
		}
		catch (char c) { curr_errors.push(error(i, c)); }
		
	}
	if (!pi_s_buffer.empty())
		pi_v_buffer.push_back(lexem(pi_s_buffer, NUM));
	pi_s_buffer.clear();
	// Очистка буфера и возврат значения
	TQueue<lexem> res(pi_v_buffer);
	pi_v_buffer.clear();
	return res;
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

parcer_fp PARCER::td_call(state st, char c) {
	int tp = (int)decode(c);
	switch (st) {
	case ST0:
		if (CHAR_IN_INTS) { return td_f0; }
		else { return td_f3; }
		break;
	case ST1:
		if (CHAR_IN_INTS) { return td_f0; }
		else { return td_f2; }
		break;
	case ST2:
		if (CHAR_IN_INTS) { return td_f3; }
		else { return td_f2; }
		break;
	case ST3:
		if (CHAR_IN_INTS) { return td_f1; }
		else { return td_f3; }
		break;
	default: return td_f3;
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
	last_func = __func__;
	last_errors.clear();
	TQueue<ERROR> curr_errors(str.size());

	buffer_dot = 1.0;
	buffer_value = 0.0;

	state st = ST0;
	for (size_t i = 0; i < str.size(); ++i) {
		try { 
			td_call(st, str[i])(str[i]); 
			st = td_next[decode(str[i])][st];
		}
		catch (char c) { curr_errors.push(error(i, c)); }
		
		/*if (st == STX) { curr_errors.push(error(i, str[i])); }*/
	}

	double res = buffer_value / buffer_dot;
	last_errors = curr_errors;
	return res;
}
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------

void PARCER::unary_handle(TQueue<LEXEM>& _infix) noexcept {
	vector<lexem> buffer(_infix.to_vector());          
	auto iterator = buffer.begin();
	Type pvi = buffer[0].type;
	if (pvi == BOP) {
		buffer.insert(iterator, lexem("0", ZERO));
	}
	iterator = buffer.begin();
	for (int i = 1; i < buffer.size(); ++i) {
		pvi = buffer[i].type;
		if (buffer[i - 1].type == L_BR && (pvi == BOP)) {
			buffer.insert(iterator + i, lexem("0", ZERO));
		}
	}
	_infix = TQueue<lexem>(buffer);
}

TQueue<LEXEM> PARCER::parce_postfix(const TQueue<LEXEM>& in) {
	TQueue<lexem> inf(in); unary_handle(inf);

	size_t sz = inf.get_size();
	TQueue<lexem> postf(sz);
	TStack<lexem> st(sz);
	lexem stackItem;
	while (!inf.isEmpty()) {
		lexem item = inf.pop();
		switch (item.type) {
		case L_BR:
			st.push(item);
			break;
		case R_BR:
			stackItem = st.pop();
			while (stackItem.type != L_BR) {
				postf.push(stackItem);
				stackItem = st.pop();
			}
			break;
		case MUL:
		case DIV:
		case BOP:
			while (!st.isEmpty()) {
				stackItem = st.top();
				if (priority[item.value] <= priority[stackItem.value])
					postf.push(st.pop());
				else { break; }
			} st.push(item);
			break;
		default:
			postf.push(item);
		}
	}
	while (!st.isEmpty()) {
		stackItem = st.pop();
		postf.push(stackItem);
	}
	return postf;
}
// --------------------------------------------------------------------------------------------