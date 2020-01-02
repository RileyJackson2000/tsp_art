#ifndef _DS_HEAP_
#define _DS_HEAP_

#include <vector>
#include <utility>
#include <iostream>

/* Templated heap data structure originally written to implement prim's the semi-smart way
 * Saw the idea of passing the specification (min/max) as a template parameter to the heap on
 *     a CS exam and thought it was cool so decided to do it that way as a POC
 *     (similar to CRTP but not quite)
 */

namespace ds {

class heap_error{};

template<typename T>
class min_heap;

template<typename T>
class max_heap;

template<typename T, typename U>
class heap {
	using value_type = T;
public:
	std::vector<value_type> the_heap;
protected:

	heap()
	{ }

	heap(std::vector<value_type> arr)
	: the_heap{arr}
	{ for (int i = the_heap.size() / 2; i >= 0; --i) heapify(the_heap, i); }

	void
	heapify(std::vector<value_type> &h, size_t i)
	{ static_cast<U *>(this)->heapify(h, i); }

	inline size_t
	left(size_t i) const
	{ return 2 * i + 1; }

	inline size_t
	right(size_t i) const
	{ return 2 * i + 2; }

	inline size_t
	parent(size_t i) const
	{ return (i - 1) / 2; }

public:
	value_type &
	peek()
	{ return the_heap[0]; }

	value_type
	pop()
	{
		value_type max = the_heap[0];
		std::swap(the_heap[0], the_heap[size() - 1]);
		the_heap.pop_back();
		heapify(the_heap, 0);
		return max;
	}

	bool
	empty()
	{ return the_heap.empty(); }

	size_t
	size()
	{ return the_heap.size(); }

	const value_type &
	operator[](size_t i) const
	{ return the_heap[i]; }

	value_type &
	operator[](size_t i)
	{ return the_heap[i]; }

	void
	insert(value_type val)
	{ static_cast<U *>(this)->insert(val); }

	friend class min_heap<T>;
	friend class max_heap<T>;
};

template<typename T>
class max_heap: public heap<T, max_heap<T>> {
	using value_type = T;
	using base = heap<T, max_heap<T>>;
	using base::left;
	using base::right;
	using base::parent;
	using base::the_heap;
public:
	using base::size;

	void
	heapify(std::vector<value_type> &h, size_t i)
	{
		size_t l = left(i);
		size_t r = right(i);
		size_t largest = (l < size() and h[l] > h[i]) ? l : i;
		if (r < size() and h[r] > h[largest]) largest = r;
		if (largest != i) {
			std::swap(h[i], h[largest]);
			heapify(h, largest);
		}
	}

	void
	insert(value_type val)
	{
		size_t i = the_heap.size();
		the_heap.push_back(val);
		while (i > 0 and the_heap[parent(i)] < the_heap[i]) {
			std::swap(the_heap[parent(i)], the_heap[i]);
			i = parent(i);
		}
	}

	max_heap()
	{ }

	max_heap(std::vector<value_type> arr)
	: base{arr} { }
};

template<typename T>
class min_heap: public heap<T, min_heap<T>> {
	using value_type = T;
	using base = heap<T, min_heap<T>>;
	using base::left;
	using base::right;
	using base::parent;
	using base::the_heap;
public:
	using base::size;

	void
	heapify(std::vector<value_type> &h, size_t i)
	{
		size_t l = left(i);
		size_t r = right(i);
		size_t smallest = (l < size() and h[l] < h[i]) ? l : i;
		if (r < size() and h[r] < h[smallest]) smallest = r;
		if (smallest != i) {
			std::swap(h[i], h[smallest]);
			heapify(h, smallest);
		}
	}

	void
	insert(value_type val)
	{
		size_t i = the_heap.size();
		the_heap.push_back(val);
		while (i > 0 and the_heap[parent(i)] > the_heap[i]) {
			std::swap(the_heap[parent(i)], the_heap[i]);
			i = parent(i);
		}
	}

	min_heap()
	{ }

	min_heap(std::vector<value_type> arr)
	: base{arr} { }
};

}

#endif

