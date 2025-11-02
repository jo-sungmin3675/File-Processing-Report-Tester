#include <iostream>
#include <stack>
using namespace std;

template <class _Tp> class Node;
template <class _Tp> class BST;

/*
* 아래는 반드시 사용해야하는 Node 클래스입니다.
* height, size를 제외한 멤버 변수는 추가, 삭제 및 변경이 불가능합니다.
*/
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

	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend ostream& operator<<(ostream& os, const __node_pointer_type& np) {
		// write your own code here
	}
};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	if (__x == nullptr) return 0;
	return max(__height(__x.__left_) + 1, __height(__x.__right_) + 1);
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	if (__x.__key_ == nullptr) return 0;
	return 1 + __size(__x.__left_) + __size(__x.__right_);
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* ❗️잘못된 출력 형식은 0점 처리됩니다.❗️
*/
template <class _NodePtr>
void __inorder(_NodePtr __x) {
	if (__x == nullptr) return;
	cout << "<";
	__inorder(__x.__left_);
	cout << __x.__key_;
	__inorder(__x.__right_);
	cout << ">";
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
pair<_NodePtr, bool> __insertBST(_NodePtr __root, const _Tp& key) {
	Node<_Tp>	__p(__root);
	Node<_Tp>	__q;

	while (__p != nullptr) {
		if (key == __p.__key_)	return pair<_NodePtr, bool>(__p, false);
		__q = __p;
		if (key < __p.__key_)	__p = __p.__left_;
		else					__p = __p.__right_;
	}

	Node<_Tp>	__newNode(key);

	if (__root == nullptr)		return pair<_NodePtr, bool>(__newNode, true);
	else if (key < __q.__key_)	__q.__left_ = __newNode;
	else						__q.__right_ = __newNode;

	return pair<_NodePtr, bool>(__root, true);
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	Node<_Tp>	__p(__root);
	Node<_Tp>	__q;

	while (__p != nullptr && key != __p.__key_) {
		__q = __p;
		if (key < __p.__key_)	__p = __p.__left_;
		else					__p = __p.__right_;
	}

	if (__p == nullptr) return nullptr;

	if (__p.__left_ != nullptr && __p.__right_ != nullptr) {
		if (__height(__p.__left_) < __height(__p.__right_) || (__height(__p.__left_) == __height(__p.__right_) && __size(__p.__left_) < __size(__p.__right_))) {
			__p = __p.__right_;
			while (__p.__left_ != nullptr) {
				__q = __p;
				__p = __p.__left_;
			}
		}else {
			__p = __p.__left_;
			while (__p.__right_ != nullptr) {
				__q = __p;
				__p = __p.__right_;
			}
		}
	}

	if (__p.__left_ != nullptr) {
		if (__q == nullptr)					__p = __p.__left_;
		else if (__q.__left_ == __p)		__q.__left_ = __p.__left_;
		else								__q.__right_ = __p.__left_;
	}else if (__p.__right_ != nullptr) {
		if (__q == nullptr)					__p = __p.__right_;
		else if (__q.__left_ == __p)		__q.__left_ = __p.__right_;
		else								__q.__right_ = __p.__right_;
	}else {
		if (__q == nullptr)					__p = nullptr;
		else if (__q.__left_ == __p)		__q.__left_ = nullptr;
		else								__q.__right_ = nullptr;
	}

	return __p;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
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
	
	/*
	* 아래는 반드시 구현해야하는 부분입니다.
	* 위의 추천드리는 설계 방식을 이용하여 구현하는 것을 추천합니다.
	* 추전드리는 설계 방식을 이용하지 않고 새로 구현하셔도 무방합니다.
	*/

	public: // Capacity
		size_type height() const {
			return __height(__root_);
		}

		size_type size() const {
			return __size(__root_);
		}
	
	public: // Lookup
		void inorder() const {
			__inorder(__root_);
			cout << endl;
		}
		
	public: // Modifier
		pair<const_pointer, bool> insert(const key_type& key) {
			return __insertBST(__root_, key);
		}

		const_pointer erase(const key_type& key) {
			pointer __r = __eraseBST(__root_, key);

			delete __r;

			return __r; 
		}

		void clear() {
			__clear(__root_);
		}
};

/*
* 아래는 추천드리는 main 함수의 예시입니다.
* 반드시 아래의 main 함수를 사용해야할 필요는 없습니다.
* ❗️새로 구현하실 경우, 출력 형식에 주의하세요.❗️
*/
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
