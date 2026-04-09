#pragma once

#include <stack>
#include <iostream>

enum class TreeExceptions {
	tree_is_empty, value_already_exists, value_not_found
};

#define TREE_IS_EMPTY TreeExceptions::tree_is_empty
#define VALUE_ALREADY_EXISTS TreeExceptions::value_already_exists
#define VALUE_NOT_FOUND TreeExceptions::value_not_found

template<typename T>
const T& maximum(const T& a, const T& b) { return (a > b) ? a : b; }

template<class T>
struct TreeNode {
	T val;
	int height;
	TreeNode<T>* l;
	TreeNode<T>* r;
	TreeNode(const T& _val) {
		val = _val;
		l = nullptr;
		r = nullptr;
	}
	void print(std::ostream& ostr) const {
		ostr << val << " ( ";
		if (l) { ostr << "l: "; l->print(ostr); }
		if (l && r) ostr << " | ";
		if (r) { ostr << "r: "; r->print(ostr); }
		ostr << " ) ";
	}
};

template<class T = int>
class TTree {
protected:
	TreeNode<T>* root;
private:
	int updHeight(TreeNode<T>* curr) { // ¬ысоту считаем от листьев, у них единица.
		if (!curr) return 0;
		int rh = 0, lh = 0;
		if (curr->l) lh = curr->l->height;
		if (curr->r) rh = curr->r->height;
		curr->height = maximum(rh, lh) + 1;
		return curr->height;
	}

	int getDiff(TreeNode<T>* a, TreeNode<T>* b) {
		return updHeight(a) - updHeight(b);
	}

	TreeNode<T>* SLRotate(TreeNode<T>* curr) {
		//if (!curr || !curr->r) return curr;

		int balance = getDiff(curr->r, curr->l);
		//if (balance < 2) return curr;

		TreeNode<T>* right = curr->r;

		int rightBalance = getDiff(right->r, right->l);
		if (rightBalance < 0) {
			curr->r = SRRotate(right);
			right = curr->r;
		}
		TreeNode<T>* p2 = right->l;

		curr->r = p2;
		right->l = curr;

		updHeight(curr);
		updHeight(right);

		return right;
	}
	TreeNode<T>* SRRotate(TreeNode<T>* curr) {
		//if (!curr || !curr->l) return curr;

		int balance = getDiff(curr->r, curr->l);
		//if (balance > -2) return curr;

		TreeNode<T>* left = curr->l;

		int leftBalance = getDiff(left->r, left->l);
		if (leftBalance > 0) {
			curr->l = SLRotate(left);
			left = curr->l;
		}

		TreeNode<T>* p2 = left->r;

		curr->l = p2;
		left->r = curr;

		updHeight(curr);
		updHeight(left);

		return left;
	}
	TreeNode<T>* balance_node(TreeNode<T>* curr) {
		if (!curr) return nullptr;

		int balance = getDiff(curr->r, curr->l);

		if (balance >= 2) {
			TreeNode<T>* right = curr->r;
			if (updHeight(right->l) > updHeight(right->r)) {
				curr->r = SRRotate(right);
			}
			return SLRotate(curr);
		}
		else if (balance <= -2) {
			TreeNode<T>* left = curr->l;
			if (updHeight(left->l) < updHeight(left->r)) {
				curr->l = SLRotate(left);
			}
			return SRRotate(curr);
		}
		return curr;
	}
	void balance(const std::stack<TreeNode<T>*>& p) {
		std::stack<TreeNode<T>*> path = p;
		while (!path.empty()) {
			TreeNode<T>* node = path.top(); path.pop();
			updHeight(node);
			TreeNode<T>* balanced = balance_node(node);
			if (path.empty()) {
				root = balanced;
			}
			else {
				TreeNode<T>* parent = path.top();
				if (parent->l == node) {
					parent->l = balanced;
				}
				else {
					parent->r = balanced;
				}
			}
		}
	}
public:
	TTree() { root = nullptr; }

	TreeNode<T>* copy(TreeNode<T>* nod) {
		if (nod == nullptr) { return nullptr; }
		TreeNode<T>* nn = new TreeNode<T>(nod->val);
		nn->height = nod->height;
		nn->l = copy(nod->l);
		nn->r = copy(nod->r);
		return nn;
	}
	TTree(const TTree<T>& other) {
		root = copy(other.root);
	}
	TTree& operator=(const TTree& other) {
		if (this != &other) {
			root = copy(other.root);
		} return *this;
	}

	TTree(TTree<T>&& other) {
		root = other.root;
		other.root = nullptr;
	}
	TTree& operator=(TTree&& other) {
		if (this != &other) {
			root = other.root;
			other.root = nullptr;
		} return *this;
	}

	void insert(const T& v) {
		if (root == nullptr) {
			root = new TreeNode<T>(v);
			return;
		}
		std::stack<TreeNode<T>*> path;
		TreeNode<T>* current = root;
		while (true) {
			if (v == current->val) {
				throw VALUE_ALREADY_EXISTS;
			}
			path.push(current);
			if (v < current->val) {
				if (current->l == nullptr) {
					current->l = new TreeNode<T>(v);
					break;
				}
				current = current->l;
			}
			else {
				if (current->r == nullptr) {
					current->r = new TreeNode<T>(v);
					break;
				}
				current = current->r;
			}
		}
		balance(path);
	}
	void outsert(const T& v) {
		if (!root) throw TREE_IS_EMPTY;
		TreeNode<T>* t = root;
		std::stack<TreeNode<T>*> path;
		while (t) {
			if (t->val == v) break;
			path.push(t);
			if (t->val > v) t = t->l;
			else t = t->r;
		}
		if (!t) throw VALUE_NOT_FOUND;
		if (t->l && t->r) {
			TreeNode<T>* parent_of_pred = t;
			TreeNode<T>* pred = t->l;
			while (pred->r) {
				parent_of_pred = pred;
				pred = pred->r;
			}
			T tmp = t->val;
			t->val = pred->val;
			pred->val = tmp;

			if (parent_of_pred == t) {
				parent_of_pred->l = pred->l;
			}
			else {
				parent_of_pred->r = pred->l;
			}
			delete pred;
			std::stack<TreeNode<T>*> balance_path;
			TreeNode<T>* cur = root;
			while (cur != parent_of_pred) {
				balance_path.push(cur);
				if (parent_of_pred->val < cur->val) cur = cur->l;
				else cur = cur->r;
			}
			balance_path.push(parent_of_pred);
			balance(balance_path);
			return;
		}
		TreeNode<T>* td = t->l;
		if (!td) td = t->r;
		if (path.empty()) {
			root = td;
		}
		else {
			TreeNode<T>* c = path.top();
			path.pop();
			if (c->l == t) c->l = td;
			else c->r = td;
		}
		delete t;
		balance(path);
	}

	bool contains(const T& v) const {
		if (root == nullptr) {
			return false;
		}
		TreeNode<T>* t = root;
		while (t != nullptr) {
			if (t->val == v) return true;
			else if (t->val > v) t = t->l;
			else t = t->r;
		}
		return false;
	}

	const T& max() const {
		if (root == nullptr) {
			throw TREE_IS_EMPTY;
		}
		TreeNode<T>* t = root;
		while (t->r != nullptr) {
			t = t->r;
		} return t->val;
	}
	const T& min() const {
		if (root == nullptr) {
			throw TREE_IS_EMPTY;
		}
		TreeNode<T>* t = root;
		while (t->l != nullptr) {
			t = t->l;
		} return t->val;
	}
	const T& getRootVal() const {
		return root->val;
	}

	friend std::ostream& operator<<(std::ostream& ostr, const TTree<T>& tree) {
		tree.root->print(ostr);
		return ostr;
	}
};