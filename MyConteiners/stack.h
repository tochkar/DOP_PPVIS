#pragma once

#include "f_list.h"

template<class _Ty>
class stack
{
public:
	stack() {}
	~stack() {}

	_Ty top() {
		return *_Data.begin();
	}
	void push(const _Ty& _Val) {
		_Data.push_front(_Val);
	}
	void pop() {
		_Data.erase(_Data.begin());
	}
	bool empty() {
		return _Data.begin() == _Data.end();
	}
private:
	forward_list<_Ty> _Data;
};