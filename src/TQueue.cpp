#include "TQueue.h"

template<class T>
TQueue<T>::TQueue(size_t n) {
	size = n;
	arr = new T[size];
	s = 0; f = size - 1;
}

template<class T>
TQueue<T>::~TQueue() {
	delete[] arr;
}

template<class T>
TQueue<T>::TQueue(const TQueue& other) {
	size = other.size;
	s = other.s; f = other.f;
	for (size_t i = s; i != next(f); i = next(i)) {
		arr[i] = other.arr[i];
	}
}

template<class T>
TQueue<T>& TQueue<T>::operator=(const TQueue& other) {
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

template<class T>
TQueue<T>::TQueue(TQueue&& other) noexcept {
	size = other.size; other.size = 0;
	s = other.s; other.s = 0;
	f = other.f; other.f = 0;
	arr = other.arr; other.arr = nullptr;
}

template<class T>
TQueue<T>& TQueue<T>::operator=(TQueue&& other) noexcept {
	if (this != &other) {
		delete[] arr;
		size = other.size; other.size = 0;
		s = other.s; other.s = 0;
		f = other.f; other.f = 0;
		arr = other.arr; other.arr = nullptr;
	}
	return *this;
}

template<class T>
bool TQueue<T>::ifFull() { return s == next(next(f)); }

template<class T>
bool TQueue<T>::isEmpty() { s == next(f); }

template<class T>
void TQueue<T>::push(T v) {
	if (this->ifFull()) throw - 1;
	f = next(f);
	arr[f] = v;
}

template<class T>
T TQueue<T>::pop() {
	if (this->isEmpty()) throw - 1;
	T res = a[s];
	s = next(s);
	return res;
}