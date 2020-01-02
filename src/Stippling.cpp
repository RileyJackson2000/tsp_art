#include <iostream>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "poisson.hpp"
#include "renderer.hpp"
#include <cairo/cairo.h>
#include "posn.hpp"
#include "image.hpp"
#include "graph.hpp"

int main(int argc, char ** argv){

	std::string outname = "test.png";
	image im{argv[1]};

	poisson pds(im);
	std::vector<posn<double>> points = pds.sample();

	renderer gr(im.width, im.height);
	gr.flushBackground();
	std::cout<<points.size()<<std::endl;
	for (auto &x : points) gr.drawCircle(x);
	gr.save(outname);
}
