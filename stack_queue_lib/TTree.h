#pragma once
#include <stack>

template<class T>
struct Node {
	T val;
	Node* l;
	Node* r;
	Node() : val(T()), l(nullptr), r(nullptr) {}
	Node(const T& v) : val(v), l(nullptr), r(nullptr) {}
};

template<class T>
class IterTLR {
	stack<Node<T>*> st;
public:
	IterTLR(Node<T>* root) { st.push(root); }
	bool HasNext() { return !st.empty(); }
	T Next() {
		if (!HasNext()) throw - 1;
		Node<T>* t = st.top(); st.pop();
		if (t->r) st.push(t->r);
		if (t->l) st.push(t->l);
		return t->val;
	}
};

template<class T>
class TTree {
	Node<T>* root;
	stack<Node<T>*> path;
	Node<T>* curr;
private:
	Node<T>* copyNode(Node<T>* other) {
		if (!other) return nullptr;
		Node<T>* tmp = new Node<T>();
		tmp->val = other->val;
		tmp->l = copyNode(other->l);
		tmp->r = copyNode(other->r);
		return tmp;
	}
public:
	TTree(Node<T>* _rt = nullptr) { root = _rt; curr = _rt; path = stack<Node<T>*>(); }
	TTree(const TTree& t) {
		root = copyNode(t.root);
		stack<Node<T>*> tmp(t.path);
		path = stack<Node<T>*>();
		stack<int> pre_path;
		Node<T>* prev = tmp.top();
		tmp.pop();
		while (!tmp.empty()) {
			if (tmp.top()->l == prev) { pre_path.push(0); }
			else { pre_path.push(1); }
			prev = tmp.top();
			tmp.pop();
		}
		Node<T>* temp = root;
		while (!pre_path.empty()) {
			int curr = pre_path.top();
			if (curr) { path.push(temp->r); }
			else { path.push(temp->l); }
		}
	}
	void addLeft(T v) {
		Node<T>* n = new Node<T>();
		n->val = v;
		n->r = nullptr;
		if (root == nullptr) {
			n->l = nullptr;
			root = n;
			curr = root;
		}
		else {
			n->l = curr->l;
			curr->l = n;
			path.push(curr);
			curr = n;
		}
	}
	void addRight(T v) {
		Node<T>* n = new Node<T>();
		n->val = v;
		n->l = nullptr;
		if (root == nullptr) {
			n->r = nullptr;
			root = n;
			curr = root;
		}
		else {
			n->r = curr->r;
			curr->r = n;
			path.push(curr);
			curr = n;
		}
	}
	void left() {
		if (curr == nullptr) throw - 1;
		if (curr->l == nullptr) throw - 2;
		path.push(curr);
		curr = curr->l;
	}
	void right() {
		if (curr == nullptr) throw - 1;
		if (curr->r == nullptr) throw - 2;
		path.push(curr);
		curr = curr->r;
	}
	void top() {
		if (path.empty()) throw - 1;
		curr = path.top(); path.pop();
	}
	void freeLRT(Node<T>* node) {
		freeLRT(node->l);
		freeLRT(node->r);
		delete node;
	}
	bool isEmpty() const { return root == nullptr; }
};