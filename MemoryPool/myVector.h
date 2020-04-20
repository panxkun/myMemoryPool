#pragma once

#include <memory>

template <class T,class Alloc=std::allocator<T>>
class myVector
{
	typedef size_t	size_type;
	typedef T		value_type;
	typedef T*		pointer;
	typedef T&		reference;

public:

	myVector():start(nullptr),finish(nullptr),end_of_stroage(nullptr)
	{

	};

	myVector(size_type n, T c=T())
	{

	}




	size_t size()
	{
		
	}

	void push_back()
	{

	}

	void pop()
	{

	}

	T back()
	{

	}

	T front()
	{

	}

	void resize(size_type sz, T c = T())
	{

	}

	void reserve(size_type n)
	{

	}

	size_type capacity()
	{

	}

	bool empty() const
	{

	}

	reference operator[](const size_t n)
	{

	}

	

private:
	typedef typename Alloc::template rebind<Node>::other allocator;
	pointer start;
	pointer finish;
	pointer end_of_stroage;
	
	
};
