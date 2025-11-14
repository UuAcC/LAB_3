#pragma once
#include <iostream>
#include <vector>

template<class T>
class TStack {
	T* mem;
	int curr;
	int n;
public:
	TStack(int _n = 1) { n = _n; curr = -1; mem = new T[n]; }
	TStack(const std::vector<T>& vec) : TStack(vec.size()) {
		for (size_t i = 0; i < n; ++i) { push(vec[i]); }
	}
	~TStack() { delete[] mem; }

	TStack(const TStack& oth) {
		curr = oth.curr;
		n = oth.n;
		mem = new T[n];
		for (int i = 0; i <= curr; i++) {
			mem[i] = oth.mem[i];
		}
	}
	TStack& operator=(const TStack& oth) {
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

	TStack(TStack&& oth) noexcept {
		curr = oth.curr;
		n = oth.n;
		mem = oth.mem;
		oth.curr = 0;
		oth.n = 0;
		oth.mem = nullptr;
	}
	TStack& operator=(TStack&& oth) noexcept {
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

	void push(const T& v) {
		if (curr == n - 1) { throw - 1; }
		curr++;
		mem[curr] = v;
	}
	const T& pop() {
		if (curr < 0) { throw - 1; }
		return mem[curr--];
	}
	T& top() const { 
		if (curr < 0) { throw - 1; }
		return mem[curr];
	}

	bool isEmpty() { return curr == -1; }
	bool isFull() { return curr == n - 1; }

	friend std::ostream& operator<<(std::ostream& ostr, TStack& s) {
		ostr << "[ ";
		for (int i = 0; i <= s.curr; i++)
			ostr << s.mem[i] << " ";
		ostr << "]";
		return ostr;
	}
	friend std::istream& operator>>(std::istream& istr, TStack& s) {
		size_t n; T val;
		istr >> n;
		for (size_t i = 0; i < n; ++i) {
			if (s.isFull()) break;
			istr >> val; s.push(val);
		}
	}
};