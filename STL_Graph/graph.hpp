#pragma once

#include <memory>
#include <map>
#include <list>

template <class _Ty, class _Vty>
class _My_Iterator
{
	using value_type	= _Ty;
	using reference		= _Ty&;
	using pointer		= _Ty*;

public:
	_My_Iterator(pointer ptr):
		_Ptr(ptr)
	{}
	_My_Iterator(const _My_Iterator& _Right) :
		_Ptr(_Right._Ptr)
	{}
	bool operator!=(const _My_Iterator& _Right) const 
	{
		return _Ptr != _Right._Ptr;
	}
	bool operator==(const _My_Iterator& _Right) const 
	{
		return _Ptr == _Right._Ptr;
	}
	_My_Iterator& operator++() 
	{
		_Ptr = &++(*_Ptr);
		return *this;
	}
	_My_Iterator& operator--() 
	{
		--(*_Ptr);
		return *this;
	}
	_Vty& operator*() 
	{
		return const_cast<_Vty&>(_Ptr->operator*());
	}
	_Vty* operator->() 
	{
		return const_cast<_Vty*>(_Ptr->operator->());
	}
protected:
	pointer _Ptr;
};



template <class _Kty, class _Vty>
struct _MyEdge {
	const _Kty _From, _To;
	_Vty _Value;
	_MyEdge() {};
	_MyEdge(const _Kty from, const _Kty to, const _Vty curve) :
		_From(from),
		_To(to),
		_Value(curve)
	{}
	_MyEdge& operator=(const _MyEdge& _Right)
	{
		_From	= _Right._From;
		_To		= _Right._To;
		_Value	= _Right._Value;
		return *this;
	}
};

template <class _Ty, 
	class _Pr = std::less<_Ty>,
	class _Alloc = std::allocator<_Ty>
> class graph_node {
public:
	using allocator = typename _Alloc::value_type;

	graph_node() :
		left(nullptr),
		right(nullptr),
		right_thread(false)
		
	{}
	graph_node(const _Ty& value, graph_node* left = nullptr, graph_node* right = nullptr, bool rThread = false):
		value(value),
		left(left),
		right(right),
		right_thread(rThread)
	{}
	~graph_node() {}

	graph_node(const graph_node& _Right)			= delete;
	graph_node(graph_node&& _Right)					= delete;
	graph_node& operator=(const graph_node& _Right) = delete;
	graph_node& operator=(graph_node&& _Right)		= delete;

	graph_node& operator++()
	{
		if (left)
			return *left;
		else
			if (right_thread)
				return *(right->right);
			else
				return *right;
	}
	graph_node& operator--()
	{

	}
	_Ty* operator->()
	{
		return &value;
	}
	_Ty& operator*()
	{
		return value;
	}

	graph_node * left, * right;
	bool right_thread;
	_Ty value;
};

template<class _Kty, class _Vty, class _Pr = std::less<_Kty>, class _Alloc = std::allocator<_MyEdge<_Kty,_Vty>>>
class graph {
public:
	using node_type		= typename graph_node<_MyEdge<_Kty,_Vty>, _Pr, _Alloc>;
	using value_type	= _MyEdge<_Kty, _Vty>;
	using iterator		= _My_Iterator<node_type, value_type>;
	using compare_type	= _Pr;
#define IS_MT(a, b) (compare_type()(b,a))
#define IS_EQ(a, b) (!compare_type()(a,b)&&!compare_type()(b,a))
#define IS_LT(a, b) (compare_type()(a,b))
	
	graph() :_Root(nullptr) 
	{
		_Head = new node_type;
		_Head->left = _Root;
		_Head->right = _Head;
		_Head->right_thread = true;
	}
	graph(const graph& _Right) :_Root(nullptr)
	{
		_Head = new node_type;
		_Head->left = _Root;
		_Head->right = _Head;
		_Head->right_thread = true;
		for (auto& _Val : _Right)
			push(_Val);
	}
	~graph()
	{
		_Clear_Node(_Root);
		delete _Head;
	}
	void push(const value_type& _Val)
	{
		_Insert(_Val);
	}
	template <class ... Args>
	void push(Args... _Args)
	{
		_Insert(value_type(std::forward<Args>(_Args)...));
	}
	iterator find(const _Kty& _First_Key, const _Kty& _Second_Key)
	{
		return _Find(_Root, _First_Key, _Second_Key);
	}
	_Vty& operator()(const _Kty& _First_Key, const _Kty& _Second_Key)
	{
		auto res = find(_First_Key, _Second_Key);
		return res->_Value;
	}
	iterator begin() const
	{
		return iterator(_Root);
	}
	iterator end() const
	{
		return _Head;
	}
	void clear()
	{
		_Clear_Node(_Root);
		_Root = nullptr;
	}
protected:
	node_type* _RInsert(node_type* _Node, node_type* _Right_Thread, const value_type& _Edge)
	{
		if (_Node->right_thread) {
			_Node->right = new node_type(_Edge, nullptr, _Right_Thread, true);
			_Node->right_thread = false;
			return _Node->right;
		}
		else
			if (IS_LT(_Edge._From,_Node->right->value._From)&&
				IS_LT(_Edge._To, _Node->right->value._To))
				return _LInsert(_Node->right, _Node, _Edge);
			else
				if (!IS_EQ(_Edge._From, _Node->right->value._From) ||
					!IS_EQ(_Edge._To, _Node->right->value._To))
					_RInsert(_Node->right, _Right_Thread, _Edge);
	}
	node_type* _LInsert(node_type* _Node, node_type* _Right_Thread, const value_type& _Edge)
	{
		if (!_Node->left)
			return _Node->left = new node_type(_Edge, nullptr, _Node, true);
		else
			if (IS_LT(_Edge._From, _Node->left->value._From) &&
				IS_LT(_Edge._To, _Node->left->value._To))
				return _LInsert(_Node->left, _Node, _Edge);
			else
				if (!IS_EQ(_Edge._From, _Node->left->value._From) ||
					!IS_EQ(_Edge._To, _Node->left->value._To))
					return _RInsert(_Node->left, _Right_Thread, _Edge);
	}
	node_type* _Insert(const value_type& e)
	{
		if (_Root == nullptr)
			return _Root = new node_type(e, nullptr, _Head, true);
		
		if (!IS_LT(e._From, _Root->value._From) ||
			!IS_LT(e._To, _Root->value._To))
			return _RInsert(_Root, _Head, e);
		else
			if (!IS_EQ(e._From, _Root->value._From) &&
				!IS_EQ(e._To, _Root->value._To))
				return _LInsert(_Root, _Root, e);
	}

	iterator _Find(node_type* _Node, const _Kty& _First_Key, const _Kty& _Second_Key)
	{
		if (IS_EQ(_First_Key, _Node->value._From) &&
			IS_EQ(_Second_Key, _Node->value._To))
			return iterator(_Node);
		if (_Node == _Head)
			return iterator(_Head);

		if (IS_LT(_First_Key, _Node->value._From) &&
			IS_LT(_Second_Key, _Node->value._To))
			return _Find(_Node->left, _First_Key, _Second_Key);
		else
			return _Find(_Node->right, _First_Key, _Second_Key);
	}
	void _Clear_Node(node_type* _Node)
	{
		if (!_Node) return;
		_Clear_Node(_Node->left);
		if (!_Node->right_thread) _Clear_Node(_Node->right);
		delete _Node;
	}
	node_type* _Head;
	node_type* _Root;
};

template <class _HGKty, class _GKty, class _GVty, class _Pr = std::less<_HGKty>, class _Alloc = std::allocator<std::pair<const _HGKty, graph<_GKty, _GVty>>>>
class hipergraph : public std::map<_HGKty, graph<_GKty, _GVty>, _Pr, _Alloc>
{
public:
	using _Mybase		= std::map<_HGKty, graph<_GKty, _GVty>, _Pr, _Alloc>;
	using key_type		= _HGKty;
	using value_type	= _MyEdge<_GKty, _GVty>;
	using node_type		= graph_node<_MyEdge<_GKty, _GVty>>;
	using mapped_type	= std::list<node_type*>;
	using compare_type	= _Pr;

	hipergraph() :_Mybase() {}

	void push(const key_type& _Key, const value_type& _Val)
	{
		this->operator[](_Key).push_back(_Insert(_Val));
	}
};