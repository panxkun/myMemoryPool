#pragma once

#include <climits>
#include <cstddef>

template <typename T,size_t BlockSize=4096>
class myAllocator
{
public:
	typedef T			value_type;
	typedef T*			pointer;
	typedef T&			reference;
	typedef const T*	const_pointer;
	typedef const T&	const_reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

	template <typename U> struct rebind
	{
		typedef myAllocator<U> other;
	};

	myAllocator() noexcept
	{
		currentBlock_ = nullptr;
		currentSlot_ = nullptr;
		lastSlot_ = nullptr;
		freeSlots_ = nullptr;
	}

	myAllocator(const myAllocator& _myAllocator) noexcept :myAllocator()
	{
	}


	template <class U> myAllocator(const myAllocator<U>& _myAllocator) noexcept;

	~myAllocator() noexcept
	{
		slot_pointer_ curr = currentBlock_;
		while (curr != nullptr)
		{
			slot_pointer_ prev = curr->next;
			operator delete(reinterpret_cast<void*>(curr));
			curr = prev;
		}
	};
	
	myAllocator& operator=(const myAllocator& _myAllocator) = delete;

	myAllocator& operator=(myAllocator&& _myAllocator) noexcept
	{
		if (this != &_myAllocator)
		{
			std::swap(currentBlock_, _myAllocator.currentBlock_);
			currentSlot_ = _myAllocator.currentSlot_;
			lastSlot_ = _myAllocator.lastSlot_;
			freeSlots_ = _myAllocator.freeSlots_;
		}
		return *this;
	}

	pointer address(reference x) const noexcept
	{
		return &x;
	}
	const_pointer address(const_reference x)const noexcept
	{
		return &x;
	}

	pointer allocate(size_type n = 1, const_pointer hint = 0)
	{
		if (freeSlots_ != nullptr)
		{
			pointer result = reinterpret_cast<pointer>(freeSlots_);
			freeSlots_ = freeSlots_->next;
			return result;
		}
		else
		{
			if (currentSlot_ >= lastSlot_)
			{
				allocateBolck();
			}
			return reinterpret_cast<pointer>(currentSlot_++);
		}
	}
	void deallocate(pointer p, size_type n = 1)
	{
		if (p != nullptr)
		{
			reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
			freeSlots_ = reinterpret_cast<slot_pointer_>(p);
		}
	}

	size_type max_size() const noexcept
	{
		size_type maxBlocks = -1 / BlockSize;
		return (BlockSize - sizeof(data_pointer_)) / sizeof(slot_type_)*maxBlocks;

	}

	template <class U, class T> void construct(U* p,const T& value)
	{
		new(p) U(value);
	}

	template <class U> void destroy(U* p)
	{
		p->~U();
	}

	template <class... Args> pointer newElement(Args&&... args)
	{
		pointer result = allocate();
		construct<value_type>(result, std::forward<Args>(args)...);
		return result;
	}

	void deleteElement(pointer p)
	{
		if (p != nullptr)
		{
			p->~value_type();
			deallocate(p);
		}
	}

private:
	union  Slot_
	{
		value_type element;
		Slot_* next;
	};

	typedef char* data_pointer_;
	typedef Slot_ slot_type_;
	typedef Slot_* slot_pointer_;

	slot_pointer_ currentBlock_;
	slot_pointer_ currentSlot_;
	slot_pointer_ lastSlot_;
	slot_pointer_ freeSlots_;

	size_type padPointer(data_pointer_ p, size_type align) const noexcept
	{
		uintptr_t result = reinterpret_cast<uintptr_t>(p);
		return ((align - result) % align);

	}
	void allocateBolck()
	{
		data_pointer_ newBolck = reinterpret_cast<data_pointer_>(operator new(BlockSize));
		reinterpret_cast<slot_pointer_>(newBolck)->next = currentBlock_;
		currentBlock_ = reinterpret_cast<slot_pointer_>(newBolck);
		data_pointer_ body = newBolck + sizeof(slot_pointer_);
		size_type bodyPadding = padPointer(body, alignof(slot_type_));
		currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
		lastSlot_ = reinterpret_cast<slot_pointer_>(newBolck + BlockSize - sizeof(slot_type_) + 1);
	}

	static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize too small." );
};
