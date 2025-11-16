#include <iostream>
#include <stack>
using namespace std;

template <class _Tp> class Node;
template <class _Tp> class BST;

template <class _Tp>
class Node {
	public: // Member types
		typedef _Tp					__key_type;
		typedef Node<__key_type>*	__node_pointer_type;
	
	public: // Member variables
		__key_type			__key_;
		__node_pointer_type	__left_;
		__node_pointer_type	__right_;
	
	public: // Constructor
		Node(): __key_(__key_type()), __left_(nullptr), __right_(nullptr) {}
		Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr) {}
};

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	if (__x == nullptr) return 0;
	return max(__height(__x->__left_) + 1, __height(__x->__right_) + 1);
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	if (__x == nullptr) return 0;
	return 1 + __size(__x->__left_) + __size(__x->__right_);
}


template <class _NodePtr>
void __inorder(_NodePtr __x) {
	if (__x == nullptr) return;
	cout << "<";
	__inorder(__x->__left_);
	cout << " " << __x->__key_ << " ";
	__inorder(__x->__right_);
	cout << ">";
}

template <class _NodePtr, class _Tp>
pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr __p = __root;
	_NodePtr __q = nullptr;

	while (__p != nullptr) {
		__q = __p;
		if (key == __p->__key_)		return pair<_NodePtr, bool>(__p, false);
		else if (key < __p->__key_)	__p = __p->__left_;
		else						__p = __p->__right_;
	}

	_NodePtr __newNode = new Node<_Tp>(key);
	if (__root == nullptr) __root = __newNode;
	else if (key < __q->__key_)	__q->__left_ = __newNode;
	else						__q->__right_ = __newNode;

	return pair<_NodePtr, bool>(__root, true);
}

template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr __p		= __root;
	_NodePtr __q		= nullptr;
	_NodePtr tempNode	= nullptr;

	while (__p != nullptr && key != __p->__key_) {
		__q = __p;
		if (key < __p->__key_)	__p = __p->__left_;
		else					__p = __p->__right_;
	}

	if (__p == nullptr) return nullptr;

	tempNode = __p;
	if (__p->__left_ != nullptr && __p->__right_ != nullptr) {
		tempNode = __p;

		if (__height(__p->__left_) < __height(__p->__right_) || (__height(__p->__left_) == __height(__p->__right_) && __size(__p->__left_) < __size(__p->__right_))) {
			__p = __p->__right_;
			while (__p->__left_ != nullptr) {
				__q = __p;
				__p = __p->__left_;
			}
		}else {
			__p = __p->__left_;
			while (__p->__right_ != nullptr) {
				__q = __p;
				__p = __p->__right_;
			}
		}

		tempNode->__key_ = __p->__key_;
	}

	if (__p->__left_ != nullptr) {
		if (__q == nullptr)					__root = __root->__left_;
		else if (__q->__left_ == __p)		__q->__left_ = __p->__left_;
		else								__q->__right_ = __p->__left_;
	}else if (__p->__right_ != nullptr) {
		if (__q == nullptr)					__root = __root->__right_;
		else if (__q->__left_ == __p)		__q->__left_ = __p->__right_;
		else								__q->__right_ = __p->__right_;
	}else {
		if (__q == nullptr)					__root = nullptr;
		else if (__q->__left_ == __p)		__q->__left_ = nullptr;
		else								__q->__right_ = nullptr;
	}

	delete __p;

	return tempNode;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	if (__x == nullptr) return;

	__clear(__x->__left_);
	__clear(__x->__right_);

	delete __x;

	__x = nullptr;
}

// 아래는 반드시 사용해야하는 BST 클래스입니다.
template <class _Tp>
class BST {
	public: // Member types
		typedef _Tp						key_type;
		typedef size_t					size_type;
		typedef Node<key_type>*			pointer;
		typedef const Node<key_type>*	const_pointer;
	
	private: // Member variables
		pointer	__root_;
	
	public: // Constructor
		BST(): __root_(nullptr) {}

	public: // Capacity
		size_type height() const {
			return __height(__root_);
		}

		size_type size() const {
			return __size(__root_);
		}

	public:
		void inorder() const {
			__inorder(__root_);
			cout << endl;
		}
		
	public:
		pair<const_pointer, bool> insert(const key_type& key) {
			return __insertBST(__root_, key);
		}

		const_pointer erase(const key_type& key) {
			pointer __r = __eraseBST(__root_, key);

			return __r; 
		}

		void clear() {
			__clear(__root_);
		}
};

int main(int argc, char **argv) {
	BST<int>	tree;
	char		command;
	int			key;

	while (cin >> command >> key) {
		switch ((int)command) {
			case (int)'i':
				if (tree.insert(key).second == false) {
					cerr << "i " << key << ": The key already exists" << endl;
					continue;
				}
				break;
			case (int)'d':
				if (tree.erase(key) == nullptr) {
					cerr << "d " << key << ": The key does not exist" << endl;
					continue;
				}
				break;
			default:
				cerr << "Invalid command: " << command << endl;
				return (1);
				break;
		}
		tree.inorder();
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();

	return (0);
}
