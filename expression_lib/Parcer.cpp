#include "Parcer.h"

#define CHAR_IN_OPS operations.find(c) != string::npos
#define CHAR_IN_INTS integers.find(c) != string::npos

string Parcer::operations = "()+-*/";
string Parcer::integers = "0123456789";

string Parcer::s_buffer;
vector<string> Parcer::v_buffer;

inline void Parcer::f1(char c) { v_buffer.push_back("" + c); }
inline void Parcer::f2(char c) { s_buffer = "" + c; }
inline void Parcer::f3(char c) { v_buffer.push_back(s_buffer); v_buffer.push_back("" + c); }
inline void Parcer::f4(char c) { s_buffer += c; }

int Parcer::pi_next(char c) {
	if (CHAR_IN_OPS) { return ST0; }
	if (CHAR_IN_INTS) { return ST1; }
	else throw -1;
}

func_pointer Parcer::pi_call(int st, char c) {
	if (st == ST0) {
		if (CHAR_IN_OPS) { return Parcer::f1; }
		if (CHAR_IN_INTS) { return Parcer::f2; }
		else throw - 1;
	}
	if (st == ST1) {
		if (CHAR_IN_OPS) { return Parcer::f3; }
		if (CHAR_IN_INTS) { return Parcer::f4; }
		else throw - 1;
	}
}

vector<string> Parcer::parce_infix(string str) {
	// Парсинг строки в вектор лексем
	int st = ST0;
	for (int i = 0; i < str.size(); ++i) {
		st = pi_next(str[i]);
		pi_call(st, str[i])(str[i]);
	}
	// Обработка унарных операторов             
	auto iterator = v_buffer.begin();
	if (v_buffer[0] == "-" || v_buffer[0] == "+") {
		v_buffer.insert(iterator, "0");
	}
	for (int i = 1; i < v_buffer.size(); ++i) {
		if (v_buffer[i - 1] == "(" && (v_buffer[i] == "-" || v_buffer[i] == "+")) {
			v_buffer.insert(iterator + i, "0");
		}
	}
	// Очистка буфера и возврат значения
	vector<string> res(v_buffer);
	v_buffer.clear(); s_buffer = "";
	return res;
}
vector<string> Parcer::parce_postfix(string str, vector<string> _infix) {
	vector<string> infix = _infix;
	if (infix.empty()) { infix = parce_infix(str); }
	// здесь должен быть этот автомат
	return vector<string>();
}


//TStack<string> st(postfix.size());              как это в автомат перевести 
//string stackItem;                               я ваще понятия не имею, но придется иметь
//for (string item : infix) {
//	if (item == "(") { st.push(item); }
//	if (item == ")") {
//		stackItem = st.pop();
//		while (stackItem != "(") {
//			postfix.push_back(stackItem);
//			stackItem = st.pop();
//		}
//	}
//	if (item == "+" || item == "-" || item == "*" || item == "/") {
//		while (!st.isEmpty()) {
//			stackItem = st.pop();
//			if (priority[item] <= priority[stackItem])
//				postfix.push_back(stackItem);
//			else { st.push(stackItem); break; }
//		} st.push(item);
//	}
//	else { postfix.push_back(item); }
//}
//while (!st.isEmpty()) {
//	stackItem = st.pop();
//	postfix.push_back(stackItem);
//}