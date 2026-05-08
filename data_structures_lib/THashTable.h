#pragma once
#include <vector>
#include "tset.h"

enum class THT_Exceptions {
	KEY_DONT_EXIST,
	HASHTABLE_UNINSERTED
};

#define THT_BIG_PRIME 7
#define THT_SMALL_PRIME 3

#define THT_MAX_HASH_TRIES 5
#define THT_MAX_RESIZE_TRIES 5

template <class keyType, class valType>
struct THT_Pair {
	keyType key;
	valType val;
	THT_Pair() : key(keyType()), val(valType()) {}
	THT_Pair(const keyType& k, const valType& v) : key(k), val(v) {}
};

template <class keyT, class valT>
class THashTable {
	using Pair = THT_Pair<keyT, valT>;

	std::vector<Pair> data;
	TSet _using;
	int size;
	char irc; // insert recursion counter
private:
	int hash(int p) const {
		return ((p * THT_BIG_PRIME + THT_SMALL_PRIME) % size);
	}
public:
	THashTable(int _sz = 1) : _using(_sz), data(_sz), size(_sz), irc(0) {}
	int find(const keyT& key) const {
		std::hash<keyT> hasher;
		int p = hasher(key);
		p = (p > 0) ? p : -1 * p;
		for (int i = 0; i < THT_MAX_HASH_TRIES; ++i) {
			p = hash(p);
			if (_using.IsMember(p)) {
				if (data[p].key == key)
					return p;
			}
		}
		return -1;
	}
	void insert(const keyT& key, const valT& val) {
		++irc;
		int pos = find(key);
		if (pos != -1) {
			data[pos].val = val;
			_using.InsElem(pos);
			irc = 0;
			return;
		}
		std::hash<keyT> hasher;
		int p = hasher(key);
		p = (p > 0) ? p : -1 * p;
		int i = 0;
		for (; i < THT_MAX_HASH_TRIES; ++i) {
			p = hash(p);
			if (!_using.IsMember(p)) {
				data[p] = Pair(key, val);
				_using.InsElem(p);
				irc = 0;
				return;
			}
		}
		if (irc == THT_MAX_RESIZE_TRIES) {
			irc = 0;
			throw THT_Exceptions::HASHTABLE_UNINSERTED;
		}
		this->resize();
		this->insert(key, val);
	}
	void insert(Pair pair) {
		this->insert(pair.key, pair.val);
	}
	void outsert(const keyT& key) {
		int pos = find(key);
		if (pos != -1) {
			_using.DelElem(pos);
			return;
		}
		throw THT_Exceptions::KEY_DONT_EXIST;
	}
	void resize() {
		THashTable<keyT, valT> new_table(size * 2);
		for (int i = 0; i < size; ++i) {
			if (_using.IsMember(i)) {
				new_table.insert(data[i]);
			}
		}
		size = new_table.size;
		data = new_table.data;
		_using = new_table._using;
	}
	valT& operator[](const keyT& key) {
		int pos = find(key);
		if (pos == -1)
			throw THT_Exceptions::KEY_DONT_EXIST;
		return data[pos].val;
	}
};