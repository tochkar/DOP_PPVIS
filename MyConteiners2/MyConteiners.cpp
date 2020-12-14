
#include "f_list.h"
#include "list.h"
#include "stack.h"
#include "queue.h"

#include <iostream>

int main()
{
	forward_list<int> flist;
	flist.push_back(10);
	flist.push_back(11);
	flist.push_back(20);
	flist.push_back(11);
	flist.push_back(21);

	for (auto it = flist.begin(); it != flist.end(); ++it) {
		if (*it == 11)
			it = flist.insert(it, 100);
	}

	for (auto it = flist.begin(); it != flist.end();) {
		if (*it == 20)
			it = flist.erase(it);
		else
			++it;
	}

	for (auto& val : flist)
		std::cout << val << std::endl;
	std::cout << std::endl;

	list<int> list;
	
	list.push_back(1);
	list.push_back(2);
	list.push_front(10);
	list.push_front(21);

	list.erase(list.begin());

	for (auto& val : list)
		std::cout << val << std::endl;

	std::cout << std::endl;

	stack<int> stack;
	stack.push(1);
	stack.push(2);
	stack.push(3);
	stack.push(4);
	stack.push(5);
	for (; !stack.empty(); stack.pop())
		std::cout << stack.top() << std::endl;

	std::cout << std::endl;

	queue<int> queue;
	queue.push(1);
	queue.push(2);
	queue.push(3);
	queue.push(4);
	queue.push(5);

	for (; !queue.empty(); queue.pop())
		std::cout << queue.front() << std::endl;

	std::cout << std::endl;
}