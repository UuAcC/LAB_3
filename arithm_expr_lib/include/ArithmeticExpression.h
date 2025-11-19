#pragma once
#include <vector>
#include <string>
#include <map>

#include "TStack.h"
#include "TQueue.h"

using namespace std;
using func_pointer = void(*)(char);

#define PARCER ArithmeticExpression::Parcer
#define SYNTAXER ArithmeticExpression::Syntaxer

#define TYPE ArithmeticExpression::Type
#define LEXEM ArithmeticExpression::lexem
#define ERROR ArithmeticExpression::error
#define STATE ArithmeticExpression::state

class ArithmeticExpression {
public:
	// Перечисление возможных типов лексем в арифметическом выражении.
	enum class Type { dot, zero, num, l_br, r_br, bop, mul, div };
	// Структура, хранящая значение лексемы и её тип (по умолчанию инициализируется { "0", ZERO } ).
	struct lexem {
		string value; Type type;
		lexem(string s = "0", Type t = Type::zero);
	};
	// Структура, хранящая индекс ошибочной лексемы и ее значение.
	struct error {
		size_t index; string value;
		error(size_t i = 0, string v = "(A)");
	};
	friend ostream& operator<<(ostream& ostr, const LEXEM& lex);
	friend ostream& operator<<(ostream& ostr, const TYPE& tp);
	friend ostream& operator<<(ostream& ostr, const ERROR& er);
private:
	// Состояния конечных автоматов, STX - состояние для throw.
	enum state { ST0, ST1, ST2, ST3, STX };
	// Класс со static функциями и полями, выполняющий парсинг строки на лексемы и лексический анализ.
	class Parcer {
		// Очередь ошибок, полученных в ходе выполнения последней вызванной функции.
		static TQueue<error> last_errors;
		// Словарь операций с их приоритетами.
		static map<string, int> priority;
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
		// Конечный автомат, который переводит строку в число.
		static double to_double(const string& str);
	};
	// Класс со static функциями и полями, выполняющий синтаксический анализ.
	class Syntaxer {
		// Очередь ошибок, полученных в ходе выполнения последней вызванной функции.
		static TQueue<error> last_errors;
		// --------------------------------------------------------------------------------------------
		//        Таблицы для конечного автомата syntax_check:
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

		//    |------|--------|---|---|--------|---|---|---|
		//    | call | [+, -] | * | / | [1..9] | 0 | ( | ) |
		//    |------|--------|---|---|--------|---|---|---|
		//    | ST0  |   f0   |f1 |f1 |  f0    |f0 |f0 |f1 |
		//    |------|--------|---|---|--------|---|---|---|
		//    | ST1  |   f0   |f0 |f0 |  f1    |f1 |f1 |f0 |
		//    |------|--------|---|---|--------|---|---|---|
		//    | ST2  |   f1   |f1 |f1 |  f0    |f0 |f0 |f1 |
		//    |------|--------|---|---|--------|---|---|---|
		//    | ST3  |   f1   |f1 |f1 |  f0    |f1 |f0 |f1 |
		//    |------|--------|---|---|--------|---|---|---|

		// Возвращает указатель на функцию из sc_funcs, которую должен выполнить 
		// конечный автомат syntax_check в данном случае.
		static func_pointer sc_call(STATE st, TYPE tp);
		// Массив, в каждой ячейке которого лежит указатель на функцию автомата syntax_check.
		static func_pointer sc_funcs[];

		// Возвращает состояние из sc_states, на которое должен перейти
		// конечный автомат syntax_check в данном случае.
		static STATE sc_next(STATE st, TYPE tp);
		// Массив, в каждой ячейке которого лежит состояние автомата syntax_check.
		static STATE sc_states[];

		// Функции конечного автомата для syntax_check.

		static inline void sc_f0(char c);
		static inline void sc_f1(char c);
	public:
		// Проверяет арифметическое выражение со скобками на корректность расстановки скобок.
		static bool skobochniy_check(const string& str);
	};
private:
	TQueue<lexem> infix; // Инфиксная форма записи, переведенная в вектор строк.
	TQueue<lexem> postfix; // Постфиксная форма записи, переведенная в вектор строк.
public:
	ArithmeticExpression(string _infix);
	inline TQueue<lexem> get_infix() const { return infix; }
	inline TQueue<lexem> get_postfix() const { return postfix; }
	// Cобственно вычисление значения выражения.
	double calculate();
};