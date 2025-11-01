#include "TStack.h"

template<class T>
TStack<T>::TStack(int _n) {
	n = _n;
	curr = -1;
	mem = new T[n];
}

template<class T>
TStack<T>::~TStack() { delete[] mem; }

template<class T>
TStack<T>::TStack(const TStack& oth) {
	curr = oth.curr;
	n = oth.n;
	mem = new T[n];
	for (int i = 0; i <= curr; i++) {
		mem[i] = oth.mem[i];
	}
}

template<class T>
TStack<T>& TStack<T>::operator=(const TStack<T>& oth) {
	if (this != &oth) {
		curr = oth.curr;
		n = oth.n;
		T* tempmem = new T[n];
		delete[] mem;
		for (int i = 0; i <= curr; i++) {
			tempmem[i] = oth.mem[i];
		}
		mem = tempmem;
	}
	return *this;
}

template <class T>
TStack<T>::TStack(TStack<T>&& oth) noexcept {
	curr = oth.curr;
	n = oth.n;
	mem = oth.mem;
	oth.curr = 0;
	oth.n = 0;
	oth.mem = nullptr;
}

template<class T>
TStack<T>& TStack<T>::operator=(TStack<T>&& oth) noexcept {
	if (this != &oth) {
		delete[] mem;
		curr = oth.curr;
		n = oth.n;
		mem = oth.mem;
		oth.curr = 0;
		oth.n = 0;
		oth.mem = nullptr;
	}
	return *this;
}

template<class T>
void TStack<T>::push(const T& v) {
	if (curr == n - 1) { throw - 1; }
	curr++;
	mem[curr] = v;
}

template<class T>
const T& TStack<T>::pop() {
	if (curr < 0) { throw - 1; }
	return mem[curr--];
}

template<class T>
T& TStack<T>::top() const {
	if (curr < 0) { throw - 1; }
	return mem[curr];
}

template<class T>
bool TStack<T>::isEmpty() {
	return curr == -1;
}

//template<class T>
//std::ostream& operator<<(std::ostream& ostr, TStack<T>& s) {
//	ostr << "[ ";
//	for (int i = 0; i <= s.curr; i++)          оно здесь орет, что нет 
//		ostr << s.mem[i] << " ";               доступа к полям класса,
//	ostr << "]";                               а почему?
//	return ostr;
//}
//
//template<class T>
//std::istream& operator>>(std::istream& istr, TStack<T>& s) {
//	size_t n, i = 0; T val;
//	istr >> n;
//	while (!s.isEmpty()) {
//		if (i >= n) break;
//		istr >> val; s.push(val);
//	}
//	return istr;
//}
