#pragma once

template<typename T>
struct QueueNode
{
	QueueNode();

	QueueNode(const T& val);
	~QueueNode();

	T value;
	
	QueueNode* next;
	QueueNode* prev;

};


template <typename T>
QueueNode<T>::QueueNode()
{
	next = nullptr;
	prev = nullptr;
}

template <typename T>
QueueNode<T>::QueueNode(const T& val)
{
	next = nullptr;
	prev = nullptr;
	
	value = val;
}

template <typename T>
QueueNode<T>::~QueueNode()
{
	
}




