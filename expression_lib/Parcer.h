#include <vector>
#include <string>

using namespace std;
using func_pointer = void(*)(char);

class Parcer {
	// Перечисление состояний конечного автомата.
	static enum states { ST0, ST1 };

	static string operations; // Строка, содержащая все допустимые арифметические операции, а также ().
	static string integers; // Строка, содержащая цифры.

	static string buffer; // Строка, нужная для выделения отдельного числа из строки.
	static vector<string> infix; // Вектор, в котором накапливаются лексемы.

	// Функции конечного автомата.

	static inline void f1(char c);
	static inline void f2(char c);
	static inline void f3(char c);
	static inline void f4(char c);

	// Возвращает следующее состояние конечного автомата.
	static int next(char input);

	// Возвращает указатель на функцию, которую должен выполнить 
	// конечный автомат в данном случае.
	// Раньше было void(*Parcer::call(int st, char c))(char) 
	static func_pointer call(int st, char c);
public:
	// Возвращает вектор лексем строки, содержащий операции и числа.
	static vector<string> parce_infix(string str);
};

