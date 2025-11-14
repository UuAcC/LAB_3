#pragma once
#include <vector>
#include <stdexcept>

template<class T>
class TQueue {
	T* arr;
	size_t size;
	size_t s, f;

	inline size_t next(size_t i) {
		return (i + 1) % size;
	}
public:
	TQueue(size_t n = 1) { size = n + 1; arr = new T[size]; s = 0; f = size - 1; }
	TQueue(const std::vector<T>& vec) : TQueue(vec.size()) {
		for (size_t i = 0; i < size - 1; ++i) { push(vec[i]); }
	}
	~TQueue() { delete[] arr; }

	TQueue(const TQueue& other) {
		size = other.size;
		s = other.s; f = other.f;
		arr = new T[size];
		for (size_t i = s; i != next(f); i = next(i)) {
			arr[i] = other.arr[i];
		}
	}
	TQueue& operator=(const TQueue& other) {
		if (this != &other) {
			size = other.size;
			s = other.s; f = other.f;
			T* temparr = new T[size];
			for (size_t i = s; i != next(f); i = next(i)) {
				temparr[i] = other.arr[i];
			}
			delete[] arr;
			arr = temparr;
		}
		return *this;
	}

	TQueue(TQueue&& other) noexcept {
		size = other.size; other.size = 0;
		s = other.s; other.s = 0;
		f = other.f; other.f = 0;
		arr = other.arr; other.arr = nullptr;
	}
	TQueue& operator=(TQueue&& other) noexcept {
		if (this != &other) {
			delete[] arr;
			size = other.size; other.size = 0;
			s = other.s; other.s = 0;
			f = other.f; other.f = 0;
			arr = other.arr; other.arr = nullptr;
		}
		return *this;
	}

	bool ifFull() { return s == next(next(f)); }
	bool isEmpty() { return s == next(f); }

	void push(T v) {
		if (this->ifFull()) throw -1;
		f = next(f);
		arr[f] = v;
	}
	const T& pop() {
		if (this->isEmpty()) throw -1;
		T& res = arr[s];
		s = next(s);
		return res;
	}

	size_t get_size() const { return size - 1; }
	void clear() { delete[] arr; arr = new T[size]; s = 0; f = size - 1; }

	friend std::ostream& operator<<(std::ostream& ostr, TQueue& q) {
		ostr << "[ ";
		for (size_t i = q.s; i != q.next(q.f); i = q.next(i)) {
			ostr << q.arr[i] << " ";
		} ostr << "]";
		return ostr;
	}
	friend std::istream& operator>>(std::istream& istr, TQueue& q) {
		size_t n, i = 0; T val;
		istr >> n;
		for (size_t i = 0; i < n; ++i) {
			if (q.ifFull()) break;
			istr >> val; q.push(val);
		}
	}
};