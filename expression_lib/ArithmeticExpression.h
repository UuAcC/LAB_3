#include "TStack.h"
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class ArithmeticExpression {
	string infix; // инфиксна€ форма записи
	string postfix; // постфиксна€ форма записи
	vector<char> lexems; // инфиксна€ форма записи, переведенна€ в вектор символов
	static map<char, int> priority; // словарь операций с их приоритетами
	map<char, double> operands; // словарь операндов с их значени€ми
private:
	void parse(); // поле infix -> поле lexems
	void to_postfix(); // поле infix -> поле postfix
public:
	ArithmeticExpression(string _infix);
	inline string get_infix() const { return infix; }      // объ€сн€ть, думаю, не нужно
	inline string get_postfix() const { return postfix; }  // объ€сн€ть, думаю, не нужно
	vector<char> get_operands() const; // возвращает имена используемых операндов
	double calculate(const map<char, double>& values); // собсна вычисление
};

// если понадоб€тс€ именно ошибки - раскомментирую и переделаю
//static TStack<int> skobochniy_check(std::string str) {
//	size_t n = str.size();
//	TStack<int> s(n);
//	for (int i = 0; i < n; i++) {
//		if (str[i] == '(') {
//			try { s.push(i); }
//			catch (...) {}
//		}
//		else {
//			try { s.pop(); }
//			catch (...) {}
//		}
//	} return s;
//}

// ѕровер€ет арифметическое выражение со скобками на корректность расстановки скобок.
static bool skobochniy_check(std::string str) {
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