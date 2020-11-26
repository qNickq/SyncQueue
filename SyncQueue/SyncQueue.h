#pragma once

#include <Windows.h>
#include <iostream>
#include <sstream>

#include "QueueNode.h"

template <typename T>
class SyncQueue
{
public:
	
	SyncQueue();

	T Pop();
	
	void Push(const T& item);

	
	~SyncQueue();
	
private:
	QueueNode<T>* head;
	QueueNode<T>* tail;
	
	CRITICAL_SECTION mutex;
	size_t size = 0;
};



template <typename T>
SyncQueue<T>::SyncQueue()
{
	head = nullptr;
	tail = nullptr;
	
	InitializeCriticalSection(&mutex);
}


template <typename T>
T SyncQueue<T>::Pop()
{
	EnterCriticalSection(&mutex);
	
	std::stringstream s;
	s << "Pop! " << " Size: " << size << std::endl;

	std::cout << s.str();
	
	if (!head)
	{
		LeaveCriticalSection(&mutex);
		throw std::exception("Empty queue");
	}
	
	QueueNode<T>* const tmp = head;
	
	if (head->next)
	{
		head = head->next;
		head->prev = nullptr;
	}
	else
	{
		head = nullptr;
		tail = nullptr;
	}
	
	--size;
	
	LeaveCriticalSection(&mutex);
	
	T ret = tmp->value;
	
	delete tmp;
	
	return ret;
}



template <typename T>
void SyncQueue<T>::Push(const T& item)
{

	QueueNode<T>* const tmp = new QueueNode<T>(item);
	
	EnterCriticalSection(&mutex);
	
	if (!head)
	{
		head = tmp;
	}
	else
	{
		tail->next = tmp;
		tmp->prev = tail;
	}
	
	tail = tmp;
	++size;
	
	std::stringstream s;
	s << "Push " << tmp->value << " size: " << size << std::endl;
	
	std::cout << s.str();
	
	LeaveCriticalSection(&mutex);
}

template <typename T>
SyncQueue<T>::~SyncQueue()
{
	
	QueueNode<T>* curr = tail;

	while (curr)
	{
		QueueNode<T>* tmp = curr->prev;
		delete curr;
		curr = tmp;
		--size;
	}
	
	DeleteCriticalSection(&mutex);
}
