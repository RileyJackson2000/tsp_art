#include "image.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include "posn.hpp"
#include <iostream>

image::image(const std::string &filename, int mesh_diam, double min, double max, double min_thresh_p, double max_thresh_p, double alpha, double beta)
: filename{filename},
  mesh_diam{mesh_diam},
  min{min},
  max{max},
  min_thresh_p{min_thresh_p},
  max_thresh_p{max_thresh_p},
  alpha{alpha},
  beta{beta}
{
	cv::Mat im = cv::imread(filename, 0);
	width = im.cols;
	height = im.rows;
	densities.reserve(width * height);

	double png_max = 0, png_min = 255;
	
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			double val = static_cast<double>(im.at<unsigned char>(j, i));
			densities.push_back(val);
			if (val > png_max) png_max = val;
			if (val < png_min) png_min = val;
		}
	}

	std::vector<double> percentile_vec{densities};
	std::sort(percentile_vec.begin(), percentile_vec.end());

	size_t size = percentile_vec.size();

	min_thresh = (percentile_vec[min_thresh_p * size] / (png_max - png_min)) * (max - min) + min;
	max_thresh = (std::min(percentile_vec[max_thresh_p * size], 200.0) / (png_max - png_min)) * (max - min) + min;

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j ) {
			double val = densities[i + j * width];
			val = ((val - png_min) / (png_max - png_min)) * (max - min) + min;
			densities[i + j * width] = val;
			if (i % mesh_diam == 0 and j % mesh_diam == 0 and val <= min_thresh)
				initials.push_back(i + j * width);
		}
	}
}
