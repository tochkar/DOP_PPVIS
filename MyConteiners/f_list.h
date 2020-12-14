#pragma once

#include <memory>

template <class _Ty>
class _F_Node
{
public:

	using value_t	= _Ty;
	using reference = value_t&;
	using pointer	= value_t*;
	
	_F_Node() : _Value(value_t{}), _Next(nullptr) {}
	_F_Node(const reference _Val) : _Value(_Val), _Next(nullptr) {}
	~_F_Node() {}

	_F_Node* next() {
		return _Next;
	}
	reference value() {
		return _Value;
	}

	value_t _Value;
	_F_Node* _Next;/*не рентабельно использовать умные указатели. слишком часто приходится менять значения
				   * если плохо продумана концепция "перекидывания адресов", умные указатели освободят еще нужную память
				   * а на продумывание концепции нужно время!!!
				   */
};

template <class _Ty>
class iterator_base
{
public:
	using value_t	= typename _Ty::value_t;
	using reference = value_t&;
	using pointer	= value_t*;

	iterator_base(_Ty* __Ptr) : _Ptr(__Ptr) {}

	iterator_base& operator++() {
		_Ptr = _Ptr->next();
		return *this;
	}
	reference operator*() {
		return _Ptr->value();
	}
	pointer operator->() {
		return &_Ptr->value();
	}
	bool operator!=(const iterator_base& _Right) {
		return _Ptr != _Right._Ptr;
	}
	bool operator==(const iterator_base& _Right) {
		return _Ptr == _Right._Ptr;
	}
	_Ty* _Ptr;
};

template <class _Ty, class Node_Type>
class _list_base
{
public:

	using value_t = _Ty;
	using node_t = Node_Type;
	using iterator = iterator_base<node_t>;

	_list_base() :
		_Head(new node_t) {}

	iterator begin() {
		return iterator(_Head->_Next);
	}

	iterator end() {
		return iterator(_Head.get());
	}
	
	void push_back(const value_t& _Val)	{
		_Insert(_Last(), _Val);
	}
	
	void push_front(const value_t& _Val) {
		_Insert(iterator(_Head.get()), _Val);
	}

	iterator insert(iterator _Iter, const value_t& _Val) {
		return _Insert(_Iter, _Val);
	}
	
	iterator erase(iterator _Iter) {
		auto ptr = _Head->_Next;
		for (; ptr->_Next != _Iter._Ptr; ptr = ptr->_Next);
		ptr->_Next = _Iter._Ptr->_Next;
		delete _Iter._Ptr;
		return iterator(ptr->_Next);
	}

protected:
	
	iterator _Insert(iterator _Iter, const value_t& _Val) {
		node_t* ptr = new node_t;
		ptr->_Value = _Val;
		ptr->_Next = _Iter._Ptr->_Next;
		_Iter._Ptr->_Next = ptr;
		return iterator(ptr);
	}
	
	iterator _Last() {
		auto ptr = _Head->_Next;
		for (; ptr->_Next != _Head.get(); ptr = ptr->_Next);
		return iterator(ptr);
	}

	std::shared_ptr<node_t> _Head;
};

template <class _Ty>
class forward_list : public _list_base<_Ty, _F_Node<_Ty>>
{
public:
	using _Mybase = _list_base<_Ty, _F_Node<_Ty>>;
	
	forward_list() : _Mybase() {
		_Mybase::_Head->_Next = _Mybase::_Head.get();
	}

	~forward_list() {
		for (auto ptr = _Mybase::_Head->_Next; ptr != _Mybase::_Head.get();) {
			auto t = ptr->next();
			delete ptr;
			ptr = t;
		}
	}
};