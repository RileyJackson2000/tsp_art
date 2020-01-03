#include <iostream>
#include "heap.hpp"
#include "graph.hpp"
#include <map>
#include <fstream>
#include <math.h>
#include <string>
#include <iostream>
#include <random>
#include "posn.hpp"
#include "image.hpp"
#include "renderer.hpp"
#include "poisson.hpp"

template<typename T>
struct e_weight {
	static double
	weight(size_t v, size_t w, posn<T> p, posn<T> q)
	{ return sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y)); }
};

void
getLine(std::ifstream &in, std::string &s)
{
	std::getline(in, s);
	while (s[0] == '#') std::getline(in, s);
}

int main(int argc, char ** argv) {
	int mesh_diam;
	double min, max, min_thresh_p, max_thresh_p, alpha, beta;

	std::string val;
	std::ifstream config{"config.tsp_art"};

	getLine(config, val);
	mesh_diam = std::stoi(val);
	getLine(config, val);
	min = std::stod(val);
	getLine(config, val);
	max = std::stod(val);
	getLine(config, val);
	min_thresh_p = std::stod(val);
	getLine(config, val);
	max_thresh_p = std::stod(val);
	getLine(config, val);
	alpha = std::stod(val);
	getLine(config, val);
	beta = std::stod(val);
	config.close();

	image im{argv[1], mesh_diam, min, max, min_thresh_p, max_thresh_p, alpha, beta};
	std::string outname{argv[2]};
	
	poisson pds{im};
	auto points = pds.sample();

	renderer gr{im.width, im.height};
	gr.flushBackground();

	if (argc == 4) {
		for (auto &x : points) gr.drawCircle(x);
		gr.save(argv[3]);
		gr.flushBackground();
	}

	auto g = ds::adj_graph<posn<double>, e_weight<double>>{points, true};
	auto mst = g.MST();
	auto odds = mst.filter_odd();

	std::unordered_map<int, size_t> inv{};

	std::string tmp = "graph.txt";
	std::map<std::pair<size_t, size_t>, bool> seen{};
	std::ofstream file;
	file.open(tmp.c_str());
	std::list<size_t> V = odds.v();
	file << "NAME: " << tmp << std::endl;
	file << "TYPE: TSP" << std::endl;
	file << "DIMENSION: " << odds.v_count() << std::endl;
	file << "EDGE_WEIGHT_TYPE: EUC_2D" << std::endl;
	file << "NODE_COORD_SECTION" << std::endl;
	
	int c = 1;
	for (auto &p : V) {
		inv[c - 1] = p;
		size_t x = odds[p].x, y = g[p].y;
		file << c++ << " " << x << " " << y << "\n";
	}
	file.close();

	system("../../c++libraries/blossom5-v2.05.src/blossom5 -g ./graph.txt -w ./out.txt -D -V");

	std::ifstream match{"out.txt"};
	std::string s;
	std::getline(match, s);
	size_t num_edges = std::stoi(s.substr(0, s.find(" ")));
	for (size_t i = 0; i < num_edges / 2; ++i) {
		std::getline(match, s);
		size_t v = std::stoi(s.substr(0, s.find(" ")));
		size_t w = std::stoi(s.substr(s.find(" ") + 1, s.find(" ", s.find(" ") + 1)));
		mst.add_mult_edge(inv[v], inv[w]);
	}

	auto e_circuit = mst.short_e_circuit();

	int prev = -1;
	for (auto &x : e_circuit) {
		gr.drawCircle(g[x]);
		if (prev != -1) gr.drawEdge(g[x], g[prev]);
		prev = x;
	}

	gr.save(outname);
	remove("graph.txt");
	remove("out.txt");
}

