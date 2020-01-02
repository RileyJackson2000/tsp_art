#pragma once

#include <cairo/cairo.h>
#include <vector>
#include <array>
#include <string>
#include "posn.hpp"

class renderer
{
	int _width, _height;
	cairo_surface_t *_surface;
	cairo_t *_cr;

public:
	renderer(int width, int height);

	void
	flushBackground();

	void
	drawCircle(posn<double> p);

	void
	drawEdge(posn<double> p1, posn<double> p2);

	void
	save(std::string filename);
};
