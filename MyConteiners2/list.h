#pragma once

#include "f_list.h"

template<class _Ty>
class _Node
{
public:
	using value_t = _Ty;
	using reference = _Ty&;

	using node_ptr = std::shared_ptr<_Node>;

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

	node_ptr prev() {
		return _Prev;
	}
	node_ptr next() {
		return _Next;
	}
	reference value() {
		return _Value;
	}

	value_t _Value;
	node_ptr _Next, _Prev;
};

template<class _Ty>
class list_iterator : public iterator_base<_Ty>
{
public:
	using _Mybase = iterator_base<_Ty>;
	using ptr_t = typename _Mybase::ptr_t;

	list_iterator(ptr_t _Val) : _Mybase(_Val) {}

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
		_Mybase()
	{
		_Mybase::_Head->_Next = _Mybase::_Head;
		_Mybase::_Head->_Prev = _Mybase::_Head;
	}

	~list() {
		_Mybase::~_list_base();
	}

	iterator begin() {
		return iterator(_Mybase::_Head->_Next);
	}
	
	iterator end() {
		return iterator(_Mybase::_Head);
	}

	iterator insert(iterator _Iter, const value_t& _Val) {
		return _Insert(_Iter, _Val);
	}

	void push_front(const value_t& _Val) {
		_Insert(iterator(_Mybase::_Head), _Val);
	}

	void push_back(const value_t& _Val) {
		_Insert(iterator(_Last()), _Val);
	}
	iterator erase(iterator _Iter) {
		auto prev = _Iter._Ptr->_Prev,
			next = _Iter._Ptr->_Next;
		prev->_Next = next;
		next->_Prev = prev;
		return iterator(next);
	}
private:
	iterator _Last() {
		auto ptr = _Mybase::_Head->_Next;
		for (; ptr->_Next != _Mybase::_Head; ptr = ptr->_Next);
		return iterator(ptr);
	}
	iterator _Insert(iterator _Iter, const value_t& _Val) {
		std::shared_ptr<node_t> ptr(new node_t);
		ptr->_Value = _Val;

		ptr->_Next = _Iter._Ptr->_Next;
		ptr->_Next->_Prev = ptr;

		ptr->_Prev = _Iter._Ptr;
		ptr->_Prev->_Next = ptr;

		return iterator(ptr);
	}
};