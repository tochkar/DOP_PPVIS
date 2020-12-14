#pragma once

#include "f_list.h"

template<class _Ty>
class _Node
{
public:
	using value_t = _Ty;
	using reference = _Ty&;

	_Node() :
		_Next(nullptr),
		_Prev(nullptr),
		_Value(value_t{})
	{}
	_Node(const reference _Val) :
		_Next(nullptr),
		_Prev(nullptr) {
		_Value = _Val;
	}
	~_Node() {}

	_Node* prev() {
		return _Prev.get();
	}
	_Node* next() {
		return _Next;
	}
	reference value() {
		return _Value;
	}

	value_t _Value;
	_Node * _Next, * _Prev;
};

template<class _Ty>
class list_iterator : public iterator_base<_Ty>
{
public:
	using _Mybase = iterator_base<_Ty>;
	
	list_iterator(_Ty* _Val) : _Mybase(_Val) {}

	list_iterator& operator--() {
		_Mybase::_Ptr = _Mybase::_Ptr->prev();
		return *this;
	}
};

template<class _Ty>
class list : public _list_base<_Ty, _Node<_Ty>>
{
	using _Mybase = _list_base<_Ty, _Node<_Ty>>;
	using node_t = typename _Mybase::node_t;
	using value_t = typename _Mybase::value_t;
public:
	using iterator = list_iterator<node_t>;

	list() : 
		_Mybase(),
		_Tail(new node_t) 
	{
		_Mybase::_Head->_Next = _Tail.get();
		_Mybase::_Head->_Prev = _Mybase::_Head.get();
		_Tail->_Next = _Tail.get();
		_Tail->_Prev = _Mybase::_Head.get();
	}

	~list() {
		for (auto ptr = this->_Head->_Next; ptr != _Tail.get();) {
			auto t = ptr->next();
			delete ptr;
			ptr = t;
		}
	}

	iterator begin() {
		return iterator(_Mybase::_Head->_Next);
	}
	
	iterator end() {
		return iterator(_Tail.get());
	}

	iterator insert(iterator _Iter, const value_t& _Val) {
		return _Insert(_Iter, _Val);
	}

	void push_front(const value_t& _Val) {
		_Insert(iterator(_Mybase::_Head.get()), _Val);
	}

	void push_back(const value_t& _Val) {
		_Insert(iterator(_Tail->_Prev), _Val);
	}
	iterator erase(iterator _Iter) {
		auto prev = _Iter._Ptr->_Prev,
			next = _Iter._Ptr->_Next;
		prev->_Next = next;
		next->_Prev = prev;
		delete _Iter._Ptr;
		return iterator(next);
	}
private:

	iterator _Insert(iterator _Iter, const value_t& _Val) {
		auto ptr = new node_t;
		ptr->_Value = _Val;

		ptr->_Next = _Iter._Ptr->_Next;
		ptr->_Next->_Prev = ptr;

		ptr->_Prev = _Iter._Ptr;
		ptr->_Prev->_Next = ptr;

		return iterator(ptr);
	}

	std::shared_ptr<node_t> _Tail;
};