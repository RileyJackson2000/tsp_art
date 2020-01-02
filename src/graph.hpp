#ifndef _DS_GRAPH_
#define _DS_GRAPH_

/* #include "Eigen/Eigen" */
#include <list>
#include <stack>
#include <vector>
#include <map>
#include <set>
#include <cstddef>
#include <unordered_map>
/* #include "heap.hpp" */
#include "disjoint_set.hpp"
#include <algorithm>
#include <iostream>

/* Basic graph template originally written to use with tsp_art
 * Supports (/will support) templated node and weight types
 * Currently implemented with an adjacency list (adjacency matrix coming (maybe?) soon w/ Eigen)
 * Supports directed and undirected graphs
 * Will not support weighted multigraph where different edges from v->w have different weights
 *     as weight must be a function of the two nodes, and the indexes of the two nodes
 */

namespace ds {

class graph_error{};

template<typename T, typename U>
class graph {
	using vertex_type = T;

protected:
	graph()
	{ }

	graph
	MST()
	{ static_cast<U *>(this)->MST(); }

public:
	void
	add_edge(size_t v, size_t w, bool undirected = true)
	{ static_cast<U *>(this)->add_edge(v, w, undirected); }

	void
	add_mult_edge(size_t v, size_t w, bool undirected = true)
	{ static_cast<U *>(this)->add_edge(v, w, undirected); }

	void
	add_vertex(size_t key, vertex_type v)
	{ static_cast<U *>(this)->add_vertex(key, v); }

	size_t
	degree(size_t v)
	{ return static_cast<U *>(this)->degree(v); }

	vertex_type &
	operator[](size_t i)
	{ return static_cast<U *>(this)->operator[](i); }

	std::list<size_t>
	e_circuit()
	{ return static_cast<U *>(this)->e_circuit(); }

	std::list<size_t>
	short_e_circuit()
	{ return static_cast<U *>(this)->e_circuit(); }

	std::list<size_t>
	get_adjacent(size_t v)
	{ return static_cast<U *>(this)->get_adjacent(v); }

	size_t
	v_count()
	{ return static_cast<U *>(this)->v_count(); }

	size_t
	e_count()
	{ return static_cast<U *>(this)->e_count(); }

	std::list<size_t>
	v()
	{ return static_cast<U *>(this)->v(); }
};

template<typename T, typename U>
class adj_graph: public graph<T, adj_graph<T, U>> {
	using vertex_type = T;
	using base = graph<T, adj_graph<T, U>>;

	struct edge {
		size_t v, w;
		double weight; //@TODO this needs to be generic eventually
		//ie fix weight type problem before it gets used in too many places

		edge(size_t v, size_t w, double weight)
		: v{v}, w{w}, weight{weight} { }

		bool
		operator<(const edge &other)
		{ return weight < other.weight; }

		bool
		operator>(const edge &other)
		{ return weight > other.weight; }
	};
	
	/* Nodes are represented internally by a unique size_t identifier */
	std::map<size_t, std::list<size_t>> adj_list;
	std::map<size_t, vertex_type> lookup;

public:
	adj_graph()
	{ }

	adj_graph(std::map<size_t, std::list<vertex_type>> &adj_list)
	: adj_list{adj_list} { }

	//@TODO this has never actually been tested
	adj_graph(std::map<size_t, vertex_type> &V, bool complete)
	: lookup{V}
	{
		if (complete) {
			for (auto it1 = V.begin(); it1 != V.end(); ++it1) {
				adj_list[*it1] = std::list<size_t>{};
				for (auto it2 = V.begin(); it2 != V.end(); ++it2) {
					if (*it1 == *it2) continue;
					adj_list[*it1].push_back(*it2);
				}
			}
		} else {
			for (auto it = V.begin(); it != V.end(); ++it)
				adj_list[*it] = std::list<size_t>{};
		}
	}

	//constructs either a complete or sparse graph on a given set of vertices, keys are array posns
	adj_graph(std::vector<vertex_type> &V, bool complete)
	{
		for (size_t i = 0; i < V.size(); ++i) lookup[i] = V[i];
		if (complete) {
			for (size_t i = 0; i < V.size(); ++i) {
				adj_list[i] = std::list<size_t>{};	
				for (size_t j = 0; j < V.size(); ++j) {
					if (i == j) continue;
					adj_list[i].push_back(j);
				}
			}
		} else {
			for (size_t i = 0; i < V.size(); ++i)
				adj_list[i] = std::list<size_t>{};
		}
	}

	vertex_type &
	operator[](size_t i)
	{
		if (lookup.count(i) != 0) return lookup[i];
		else throw graph_error{};
	}

	size_t
	v_count()
	{ return adj_list.size(); }

	//@TODO should make this a field, O(1) vs O(|V|)
	size_t
	e_count()
	{
		size_t e_c = 0;
		for (auto &x : adj_list) e_c += x.second.size();
		return e_c;
	}

	//@TODO should switch to Prims cause use case is primarily dense graphs, also need to constant time optimize
	adj_graph<vertex_type, U>
	MST()
	{
		std::vector<edge> edges{};
		edges.reserve(e_count());
		/* ds::heap<edge, ds::min_heap<edge>> e_heap = ds::min_heap<edge>{}; */
		/* Used to be a heap but constant factor was too high */
		for (size_t i = 0; i < adj_list.size(); ++i) { // O(|E| * log|E|)
			for (auto it = adj_list[i].begin(); it != adj_list[i].end(); ++it) {
				edges.emplace_back(i, *it, U::weight(i, *it, lookup[i], lookup[*it]));
			}
		}
		ds::disjoint_set<size_t> A{v_count()};
		ds::adj_graph<vertex_type, U> mst{};
		for (auto &x : adj_list) A.make_set(x.first);
		std::sort(edges.begin(), edges.end());
		for (auto &&x : edges) {
			if (A.find(x.v) != A.find(x.w)) {
				mst.add_vertex(x.w, lookup[x.w]);
				mst.add_vertex(x.v, lookup[x.v]);
				mst.add_edge(x.v, x.w);
				A.union_set(x.v, x.w);
			}
		}
		return mst;
	}

	//@TODO ideally have a graph act like list which can be filtered, mapped, etc.
	//fine for now
	adj_graph<vertex_type, U>
	filter_odd()
	{
		adj_graph<vertex_type, U> g{};
		std::set<size_t> odds{};
		for (auto &x : adj_list)
			if (degree(x.first) % 2 != 0) odds.insert(x.first);
		for (auto v_it = odds.begin(); v_it != odds.end(); ++v_it) {
			g.add_vertex(*v_it, lookup[*v_it]);
			for (auto e_it = adj_list[*v_it].begin(); e_it != adj_list[*v_it].end(); ++e_it) {
				if (odds.count(*e_it)) {
					g.add_vertex(*e_it, lookup[*e_it]);
					g.add_edge(*v_it, *e_it);
				}
			}
		}
		return g;
	}

	void
	add_vertex(size_t key, vertex_type v)
	{
		if (adj_list.count(key) == 0) {
			adj_list[key] = std::list<size_t>{};
			lookup[key] = v;
		}
	}

	void
	add_edge(size_t v, size_t w, bool undirected = true)
	{
		if (adj_list.count(v) == 0 or adj_list.count(w) == 0) return;
		for (auto x : adj_list[v])
			if (x == w) return;
		adj_list[v].push_back(w);
		if (undirected) adj_list[w].push_back(v);	
	}

	void
	add_mult_edge(size_t v, size_t w, bool undirected = true)
	{
		adj_list[v].push_back(w);
		if (undirected) adj_list[w].push_back(v);	
	}


	std::list<size_t>
	get_adjacent(size_t v)
	{ return adj_list[v]; }

	/*@TODO Like MST above, this can still be optimized, however its not a bottleneck (currently)
	 * Also its asymptotically optimal (barring the use of map instead of unordered_map)
	 */
	std::list<size_t>
	e_circuit()
	{
		std::map<size_t, std::list<size_t>> e_copy{adj_list};
		std::unordered_map<size_t, size_t> e_count;

		for (auto &x : e_copy)
			e_count[x.first] = x.second.size();

		std::stack<size_t> cur_path;
		std::list<size_t> circuit;

		size_t cur_v = (e_copy.begin())->first;
		cur_path.push(cur_v);
		
		while (!cur_path.empty()) {
			if (e_count[cur_v]) {
				cur_path.push(cur_v);
				e_count[cur_v]--;
				size_t tmp = e_copy[cur_v].front();
				e_count[tmp]--;
				auto it = std::find(e_copy[tmp].begin(),e_copy[tmp].end(),cur_v);
				e_copy[tmp].erase(it);
				e_copy[cur_v].pop_front();
				cur_v = tmp;
			} else {
				circuit.push_back(cur_v);
				cur_v = cur_path.top();
				cur_path.pop();
			}
		}
		return circuit;
	}

	std::list<size_t>
	short_e_circuit()
	{
		std::list<size_t> euler = e_circuit();
		std::map<size_t, bool> seen;
		for (auto it = euler.begin(); it != euler.end(); ) {
			if (seen.count(*it) != 0) {
				it = euler.erase(it);
			} else {
				seen[*it];
				++it;
			}
		}
		euler.push_back(*(euler.begin()));
		return euler;
	}

	size_t
	degree(size_t v)
	{ return adj_list[v].size(); }

	std::list<size_t>
	v()
	{
		std::list<size_t> list{};
		for (auto &x : adj_list) list.push_back(x.first);
		return list;
	}
};

}

#endif

