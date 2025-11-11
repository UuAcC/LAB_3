#include "Parcer.h"

#define CHAR_IN_OPS operations.find(c) != string::npos
#define CHAR_IN_INTS integers.find(c) != string::npos

string Parcer::operations = "()+-*/";
string Parcer::integers = "0123456789";

string Parcer::buffer;
vector<string> Parcer::infix;

inline void Parcer::f1(char c) { infix.push_back("" + c); }
inline void Parcer::f2(char c) { buffer = "" + c; }
inline void Parcer::f3(char c) { infix.push_back(buffer); infix.push_back("" + c); }
inline void Parcer::f4(char c) { buffer += c; }

int Parcer::next(char c) {
	if (CHAR_IN_OPS) { return ST0; }
	if (CHAR_IN_INTS) { return ST1; }
	else throw -1;
}

func_pointer Parcer::call(int st, char c) {
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
	int st = ST0;
	for (int i = 0; i < str.size(); ++i) {
		st = next(str[i]);
		call(st, str[i])(str[i]);
	}
	vector<string> res(infix);
	infix.clear(); buffer = "";
	return res;
}
