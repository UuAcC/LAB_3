#include <vector>
#include <string>
#include "TStack.h"

using namespace std;
using func_pointer = void(*)(char);

class Parcer {
	static string operations; // Строка, содержащая все допустимые арифметические операции, а также ().
	static string integers; // Строка, содержащая цифры.

	static string s_buffer; // Строка, нужная для выделения отдельного числа из строки.
	static vector<string> v_buffer; // Вектор, в котором накапливаются лексемы.

	// Перечисление состояний конечных автоматов.
	static enum states { ST0, ST1, ST2, ST3 };

	//                Таблицы для конечного автомата parce_infix:
	// 
	//    |------|---------------|--------|      |------|---------------|--------|
	//    | next | [+,-,*,/,(,)] | [0..9] |      | call | [+,-,*,/,(,)] | [0..9] |
	//    |------|---------------|--------|      |------|---------------|--------|
	//    | ST0  |      ST0      |   ST1  |      | ST0  |      f1       |   f2   |
	//    |------|---------------|--------|      |------|---------------|--------|
	//    | ST1  |      ST0      |   ST1  |      | ST1  |      f3       |   f4   |
	//    |------|---------------|--------|      |------|---------------|--------|

	// Возвращает следующее состояние конечного автомата parce_infix.
	static int pi_next(char input);

	// Возвращает указатель на функцию, которую должен выполнить 
	// конечный автомат parce_infix в данном случае.
	// Раньше было void(*Parcer::call(int st, char c))(char) 
	static func_pointer pi_call(int st, char c);

	// Функции конечного автомата для parce_infix.

	static inline void f1(char c);
	static inline void f2(char c);
	static inline void f3(char c);
	static inline void f4(char c);
public:
	// Возвращает вектор лексем строки, содержащий операции и числа.
	static vector<string> parce_infix(string str);
	static vector<string> parce_postfix(string str, vector<string> _infix = {});
};

