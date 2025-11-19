#pragma once

#include <vector>
#include <string>
#include <map>

#include "TStack.h"
#include "TQueue.h"

using namespace std;
using func_pointer = void(*)(char);

#define PARCER ArithmeticExpression::Parcer
#define TYPE ArithmeticExpression::Type
#define LEXEM ArithmeticExpression::lexem

class ArithmeticExpression {
public:
	// Перечисление возможных типов лексем в арифметическом выражении.
	enum class Type { zero, num, dot, l_br, r_br, op };
	// Структура, хранящая значение лексемы и её тип (по умолчанию инициализируется { "0", ZERO } ).
	struct lexem { 
		string value; Type type; 
		lexem(string s = "0", Type t = Type::zero);
	};
	friend ostream& operator<<(ostream& ostr, const LEXEM& lex);
	friend ostream& operator<<(ostream& ostr, const TYPE& tp);
private:
	TQueue<lexem> infix; // Инфиксная форма записи, переведенная в вектор строк.
	TQueue<lexem> postfix; // Постфиксная форма записи, переведенная в вектор строк.
	static map<string, int> priority; // Словарь операций с их приоритетами.

	class Parcer {
		// Состояния конечных автоматов, STX - состояние для throw.
		enum state { ST0, ST1, ST2, ST3, STX };
		// Функция, определяющая, чем является входной символ.
		static TYPE decode(char c);

		// --------------------------------------------------------------------------------------------
				//                Таблицы для конечного автомата parce_infix:
				// 
				//    |------|---------------|--------|      |------|---------------|--------|
				//    | next | [+,-,*,/,(,)] | [0..9] |      | call | [+,-,*,/,(,)] | [0..9] |
				//    |------|---------------|--------|      |------|---------------|--------|
				//    | ST0  |      ST0      |   ST1  |      | ST0  |      f0       |   f1   |
				//    |------|---------------|--------|      |------|---------------|--------|
				//    | ST1  |      ST0      |   ST1  |      | ST1  |      f2       |   f3   |
				//    |------|---------------|--------|      |------|---------------|--------|

				// Строка, нужная автомату parce_infix для выделения отдельного числа из строки.
		static string pi_s_buffer;
		// Вектор, нужный автомату parce_infix для накопления лексем.
		static vector<LEXEM> pi_v_buffer;

		// Возвращает следующее состояние конечного автомата parce_infix.
		static state pi_next(char input);

		// Возвращает указатель на функцию, которую должен выполнить 
		// конечный автомат parce_infix в данном случае.
		static func_pointer pi_call(state st, char c);

		// Функции конечного автомата для parce_infix.

		static inline void pi_f0(char c);
		static inline void pi_f1(char c);
		static inline void pi_f2(char c);
		static inline void pi_f3(char c);
		// --------------------------------------------------------------------------------------------

		// --------------------------------------------------------------------------------------------
				//        Таблицы для конечного автомата to_double:
				// 
				//    |------|---|--------|---|   |------|---|--------|
				//    | next | 0 | [1..9] | . |   | call | . | [0..9] |
				//    |------|---|--------|---|   |------|---|--------|
				//    | ST0  |ST2|  ST1   |STx|   | ST0  | f3|   f0   |
				//    |------|---|--------|---|   |------|---|--------|
				//    | ST1  |ST1|  ST1   |ST3|   | ST1  | f2|   f0   |
				//    |------|---|--------|---|   |------|---|--------|
				//    | ST2  |STx|  STx   |ST3|   | ST2  | f2|   f3   |
				//    |------|---|--------|---|   |------|---|--------|
				//    | ST3  |ST3|  ST3   |STx|   | ST3  | f3|   f1   |
				//    |------|---|--------|---|   |------|---|--------|

				// Число, в котором накапливается результат конечного автомата to_double.
		static double buffer_value;
		// Число равное степени 10, на которое делится buffer_value.
		static double buffer_dot;

		// Возвращает следующее состояние конечного автомата to_double.
		static map<TYPE, map<state, state>> td_next;

		// Возвращает указатель на функцию, которую должен выполнить 
		// конечный автомат to_double в данном случае.
		static func_pointer td_call(state st, char c);

		// Функции конечного автомата для to_double.

		static inline void td_f0(char c);
		static inline void td_f1(char c);
		static inline void td_f2(char c);
		static inline void td_f3(char c);
		static inline double switch_char(char c);

	public:
		// Конечный автомат: вход - строка арифм. выражения, выход - очередь её лексем.
		static TQueue<LEXEM> parce_infix(const string& str);
		// Принимает на вход строку и возвращает очередь с постфиксной записью ее лексем.
		static TQueue<LEXEM> parce_postfix(const string& str);
		// Принимает на вход очередь с инфиксной записью лексем и возвращает постфиксную запись этой очереди.
		static TQueue<LEXEM> parce_postfix(TQueue<LEXEM> que);
		// Проверяет арифметическое выражение со скобками на корректность расстановки скобок.
		static bool skobochniy_check(const string& str);
		// Конечный автомат, который переводит строку в число.
		static double to_double(const string& str);
	};
public:
	ArithmeticExpression(string _infix);
	inline TQueue<lexem> get_infix() const { return infix; }
	inline TQueue<lexem> get_postfix() const { return postfix; }
	// Cобственно вычисление значения выражения.
	double calculate(); 
};