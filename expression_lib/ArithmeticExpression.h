#include "TStack.h"
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class ArithmeticExpression {
	vector<string> infix; // Инфиксная форма записи, переведенная в вектор строк.
	vector<string> postfix; // Постфиксная форма записи, переведенная в вектор строк.
	static map<char, int> priority; // Словарь операций с их приоритетами.
private:
	// Поле infix -> поле postfix
	void to_postfix(); 
public:
	ArithmeticExpression(string _infix);
	inline vector<string> get_infix() const { return infix; }
	inline vector<string> get_postfix() const { return postfix; }

	// Cобственно вычисление значения выражения.
	double calculate(const map<char, double>& values); 
};

// Проверяет арифметическое выражение со скобками на корректность расстановки скобок.
static bool skobochniy_check(std::string str);