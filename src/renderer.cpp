#include "renderer.hpp"
#include <cairo/cairo.h>

#define pi 3.141592653

renderer::renderer(int width, int height)
: _width{width}, _height{height}
{
	_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, _width, _height);
	_cr = cairo_create(_surface);
}

void
renderer::flushBackground()
{
	cairo_set_source_rgb(_cr, 1.0, 1.0, 1.0);
	cairo_rectangle(_cr, 0, 0, _width, _height);
	cairo_fill(_cr);
}

void
renderer::drawCircle(posn<double> p)
{
	cairo_set_source_rgb(_cr, 0, 0, 0);
  	cairo_arc (_cr, p.x, p.y, 2.0, 0, 2 * pi);
	cairo_fill(_cr);
}

void
renderer::drawEdge(posn<double> p1, posn<double> p2)
{
	cairo_set_source_rgb(_cr, 0, 0, 0);
	cairo_move_to(_cr, p1.x, p1.y);
	cairo_line_to(_cr, p2.x, p2.y);
	cairo_stroke(_cr);
}

void
renderer::save(std::string filename)
{ cairo_surface_write_to_png (_surface, filename.c_str()); }

#undef pi
