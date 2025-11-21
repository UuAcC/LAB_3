#pragma once
#include <vector>
#include <string>
#include <map>

#include "TStack.h"
#include "TQueue.h"

#define PARCER ArithmeticExpression::Parcer
#define ANALYZER ArithmeticExpression::Analyzer

#define TYPE ArithmeticExpression::Type
#define LEXEM ArithmeticExpression::lexem

#define STATE ArithmeticExpression::state
#define ERROR ArithmeticExpression::error

using namespace std;
// Указатель на бинарную арифметическую функцию.
using arithm_fp = double(*)(double, double);

class ArithmeticExpression {
public:
	// Перечисление возможных типов лексем в арифметическом выражении.
	enum class Type { dot, zero, num, l_br, r_br, bop, mul, div };
	// Структура, хранящая значение лексемы и её тип (по умолчанию инициализируется { "0", ZERO } ).
	struct lexem {
		string value; Type type;
		lexem(string s = "0", Type t = Type::zero);
	};
	friend ostream& operator<<(ostream& ostr, const LEXEM& lex);
	friend ostream& operator<<(ostream& ostr, const TYPE& tp);
private:
	// Состояния конечных автоматов, STX - состояние для throw.
	enum state { ST0, ST1, ST2, ST3, STX };
	// Структура, хранящая индекс ошибочной лексемы и ее значение.
	struct error {
		size_t index; string value;
		error(size_t i = 0, string v = "(A)");
		error(size_t i, char c);
	};
	friend ostream& operator<<(ostream& ostr, const ERROR& er);
	// Класс со static функциями и полями, выполняющий парсинг строки на лексемы и лексический анализ.
	class Parcer {
		// Название последней выполненной функции.
		static string last_func;
		// Очередь ошибок, полученных в ходе выполнения последней вызванной функции.
		static TQueue<ERROR> last_errors;

		// Словарь операций с их приоритетами.
		static map<string, int> priority;
		// Функция, определяющая, чем является входной символ.
		static TYPE decode(char c);
		// Функция, определяющая, является ли значение лексемы нулем.
		static TYPE type_num(string str);

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
		static void (*pi_call(state st, char c))(char);

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

		// Возвращает указатель на функцию, которую должен выполнить конечный автомат to_double в данном случае.
		static void (*td_call(state st, char c))(char);

		// Функции конечного автомата для to_double.

		static inline void td_f0(char c);
		static inline void td_f1(char c);
		static inline void td_f2(char c);
		static inline void td_f3(char c);
		static inline double switch_char(char c);
		// --------------------------------------------------------------------------------------------

		// Функция, обрабатывающая унарные операторы в очереди лексем инфиксной формы.
		static void unary_handle(TQueue<LEXEM>& _infix) noexcept;
	public:
		// Возвращает, были ли ошибки в исполнении последней функции.
		static inline bool errors_occured() { return !last_errors.isEmpty(); };
		// Выводит информацию об ошибках в последней выполненной функции.
		static void print_error_message();
		// Удаляет из строки все пробелы.
		static string delete_spaces(const string& str);
		// Конечный автомат: вход - строка арифм. выражения, выход - очередь её лексем.
		static TQueue<LEXEM> parce_infix(const string& str);
		// Принимает на вход очередь с инфиксной записью лексем и возвращает постфиксную запись этой очереди.
		static TQueue<LEXEM> parce_postfix(const TQueue<LEXEM>& que);
		// Конечный автомат, который переводит строку в число.
		static double to_double(const string& str);
	};
	// Класс со static функциями и полями, выполняющий синтаксический анализ инфиксной формы арифметического выражения.
	class Analyzer {
		// Название последней выполненной функции.
		static string last_func;
		// Очередь ошибок, полученных в ходе выполнения последней вызванной функции.
		static TQueue<ERROR> last_errors;

		// --------------------------------------------------------------------------------------------
		//        Таблицы для конечного автомата syntax_check:
		// 
		//    |------|--------|---|---|--------|---|---|---|    |------|--------|---|---|--------|---|---|---|
		//    | next | [+, -] | * | / | [1..9] | 0 | ( | ) |    | call | [+, -] | * | / | [1..9] | 0 | ( | ) |   
		//    |------|--------|---|---|--------|---|---|---|    |------|--------|---|---|--------|---|---|---|
		//    | ST0  |   ST2  |STx|STx|  ST1   |ST1|ST0|STx|    | ST0  |   f0   |f1 |f1 |  f0    |f0 |f0 |f1 |
		//    |------|--------|---|---|--------|---|---|---|    |------|--------|---|---|--------|---|---|---|
		//    | ST1  |   ST2  |ST2|ST3|  STx   |STx|STx|ST1|    | ST1  |   f0   |f0 |f0 |  f1    |f1 |f1 |f0 |
		//    |------|--------|---|---|--------|---|---|---|    |------|--------|---|---|--------|---|---|---|
		//    | ST2  |   STx  |STx|STx|  ST1   |ST1|ST0|STx|    | ST2  |   f1   |f1 |f1 |  f0    |f0 |f0 |f1 |
		//    |------|--------|---|---|--------|---|---|---|    |------|--------|---|---|--------|---|---|---|
		//    | ST3  |   STx  |STx|STx|  ST1   |STx|ST0|STx|    | ST3  |   f1   |f1 |f1 |  f0    |f1 |f0 |f1 |
		//    |------|--------|---|---|--------|---|---|---|    |------|--------|---|---|--------|---|---|---|

		// Возвращает указатель на функцию из sc_funcs, которую должен выполнить конечный автомат syntax_check в данном случае.
		static void (*sc_call(STATE st, TYPE tp))(LEXEM);
		// Массив, в каждой ячейке которого лежит указатель на функцию автомата syntax_check.
		static void (*sc_funcs[])(LEXEM);

		// Возвращает состояние из sc_states, на которое должен перейти конечный автомат syntax_check в данном случае.
		static STATE sc_next(STATE st, TYPE tp);
		// Массив, в каждой ячейке которого лежит состояние автомата syntax_check.
		static STATE sc_states[];

		// Функции конечного автомата для syntax_check.

		static inline void sc_f0(LEXEM lex);
		static inline void sc_f1(LEXEM lex);
	public:
		// Возвращает, были ли ошибки в исполнении последней функции.
		static inline bool errors_occured() { return !last_errors.isEmpty(); };
		// Выводит информацию об ошибках в последней выполненной функции.
		static void print_error_message();
		// Проверяет арифметическое выражение со скобками на корректность расстановки скобок.
		static bool skobochniy_check(const string& str);
		// Выполняет лексический анализ арифметического выражения (использует PARCER::to_double).
		static bool lexic_check(TQueue<LEXEM> que);
		// Выполняет синтаксический анализ арифметического выражения.
		static bool syntax_check(TQueue<LEXEM> que);
	};
private:
	string s_infix; // Инфиксная форма записи арифметического выражения.
	TQueue<lexem> q_infix; // Инфиксная форма записи, переведенная в вектор строк.
	TQueue<lexem> q_postfix; // Постфиксная форма записи, переведенная в вектор строк.

	// Арифметические функции, вызываемые в методе calculate с помощью ae_call.

	static inline double add(double arg1, double arg2) { return arg1 + arg2; }
	static inline double sub(double arg1, double arg2) { return arg1 - arg2; }
	static inline double mul(double arg1, double arg2) { return arg1 * arg2; }
	static inline double div(double arg1, double arg2) { return arg1 / arg2; }

	// Возвращает указатель на арифм. функцию, соответствующую входной операции.
	arithm_fp ae_call(string str);
public:
	ArithmeticExpression(string _infix);
	inline string get_s_infix() const { return s_infix; }
	inline TQueue<lexem> get_q_infix() const { return q_infix; }
	inline const TQueue<lexem>& get_q_postfix() {
		if (q_postfix.isEmpty())
			q_postfix = Parcer::parce_postfix(q_infix);
		return q_postfix;
	}
	// Функция, выполняющая все проверки синтаксиса, возвращает true <=> все проверки пройдены.
	bool run_analyzer();
	// Cобственно вычисление значения выражения.
	double calculate();
};