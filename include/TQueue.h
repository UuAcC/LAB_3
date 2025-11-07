#pragma once

template<class T>
class TQueue {
	T* arr;
	size_t size;
	size_t s, f;

	inline size_t next(size_t i) {
		return (i + 1) % size;
	}
public:
	TQueue(size_t n);
	~TQueue();

	TQueue(const TQueue& other);
	TQueue& operator=(const TQueue& other);

	TQueue(TQueue&& other) noexcept;
	TQueue& operator=(TQueue&& other) noexcept;

	bool ifFull();
	bool isEmpty();

	void push(T v);
	const T& pop();
};