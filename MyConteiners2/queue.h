#pragma once

#include "f_list.h"

template<class _Ty>
class queue //тот же декоратор списка... слишком хорошо ложиться концепция, чтобы заново переделывать)
{
public:
	queue() {}
	~queue() {}

	void push(const _Ty& _Val) {
		_Data.push_back(_Val);
	}
	_Ty front() {
		return *_Data.begin();
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