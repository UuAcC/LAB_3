#pragma once
#include <iostream>

template<class T>
class TStack {
	T* mem;
	int curr;
	int n;
public:
	TStack(int _n);
	~TStack();

	TStack(const TStack& oth);
	TStack& operator=(const TStack& oth);

	TStack(TStack&& oth) noexcept;
	TStack& operator=(TStack&& oth) noexcept;

	void push(const T& v);
	const T& pop();
	T& top() const;

	bool isEmpty();
	friend std::ostream& operator<<(std::ostream& ostr, TStack& s) {
		ostr << "[ ";
		for (int i = 0; i <= s.curr; i++)
			ostr << s.mem[i] << " ";
		ostr << "]";
		return ostr;
	}
	friend std::istream& operator>>(std::istream& istr, TStack& s) {
		size_t n, i = 0; T val;
		istr >> n;
		while (!s.isEmpty()) {
			if (i >= n) break;
			istr >> val; s.push(val);
		}
	}
};