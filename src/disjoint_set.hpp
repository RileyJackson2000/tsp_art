#ifndef _DS_DISJOINT_SET_
#define _DS_DISJOINT_SET_

#include <unordered_map>
#include <vector>

namespace ds {
	
template<typename T>
class disjoint_set {
	std::unordered_map<T, size_t> id;
	std::vector<size_t> p;
	std::vector<size_t> r;
public:
	disjoint_set()
	{ }

	disjoint_set(size_t size)
	{ p.reserve(size); r.reserve(size); }

	void
	make_set(const T &t)
	{
		if (id.find(t) != id.end()) return;
		size_t s = id.size();
		id[t] = s;
		p.push_back(s);
		r.push_back(0);
	}

	size_t
	find(const T &t)
	{
		auto key = id.find(t);
		if (key == id.end()) return -1;
		return _find(key->second);
	}

	void
	union_set(const T &a, const T &b)
	{
		auto a_it = id.find(a), b_it = id.find(b);
		if (a_it == id.end() or b_it == id.end()) return;
		auto p_a = _find(a_it->second), p_b = _find(b_it->second);
		_link(p_a, p_b);
	}
private:
	size_t
	_find(size_t i)
	{
		if (p[i] != i) p[i] = _find(p[i]);
		return p[i];
	}

	void
	_link(size_t a, size_t b)
	{
		if (r[a] > r[b]) p[b] = a;
		else {
			p[a] = b;
			if (r[a] == r[b]) ++r[b];
		}
	}
};

}

#endif

