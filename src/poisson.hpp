#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include "posn.hpp"
#include <random>
#include "image.hpp"

class poisson 
{
	struct lattice_point {
		int id;
		double density;
		bool valid;
		lattice_point(double density);
	};

	image _im;
	std::vector<lattice_point> _grid;
	std::default_random_engine _generator;
	std::uniform_real_distribution<double> _dist;

public:
	poisson(image im);

	std::vector<posn<double>>	
	sample();

private:
	posn<double>
	getPoint(const posn<double> &point, double lower_r, double upper_r);

	bool
	hasNeighbors(posn<double> &point, double r, std::vector<posn<double>> &points);

	bool
	insertPoint(std::vector<posn<double>> &points, const posn<double> &point);
};
