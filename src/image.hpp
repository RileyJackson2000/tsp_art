#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include "posn.hpp"

struct image {
	std::string filename;
	int mesh_diam, width, height;
	double min_thresh, max_thresh, min, max, min_thresh_p, max_thresh_p, alpha, beta;
	std::vector<int> initials;
	std::vector<double> densities;

	image(const std::string &filename, int mesh_diam = 10, double min = 0.5, double max = 16, double min_thresh_p = 0.1, double max_thresh_p = 0.5, double alpha = 1, double beta = 2);
};
