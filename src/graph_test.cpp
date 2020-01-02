#include "graph.hpp"
#include <iostream>
#include "posn.hpp"

template<typename T>
struct e_weight {
	static double
	weight(size_t v, size_t w, posn<T> p, posn<T> q)
	{ return sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y)); }
};

int main() {
	std::vector<posn<double>> P{posn<double>(1, 2), posn<double>(3, 4), posn<double>(5,6)};
	auto g = ds::adj_graph<posn<double>, e_weight<double>>{P, true};
	auto e = g.e_circuit();
	for (auto &x : e) std::cout << x << " ";
	std::cout << std::endl;
}
