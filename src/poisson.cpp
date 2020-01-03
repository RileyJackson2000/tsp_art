#include "poisson.hpp"
#include <algorithm>
#include <vector>
#include <deque>
#include "image.hpp"
#include <random>
#include <math.h>

using std::vector;

poisson::lattice_point::lattice_point(double density)
:id{-1}, density{density}, valid{true}
{ }

poisson::poisson(image im)
: _im{im}, _grid{}, _generator{}, _dist{-1.0, 1.0}
{
	_grid.reserve(_im.width * _im.height);
	for (int j = 0; j < _im.height; ++j)
		for (int i = 0; i < _im.width; ++i)
			_grid.emplace_back(im.densities[i + j * im.width]);
}

vector<posn<double>>
poisson::sample()
{
	vector<posn<double>> points{};
	vector<int> neighbors;
	const int k = 50;
	std::deque<posn<double>> active_list{};
	for (auto &x : _im.initials)
		active_list.push_back(posn<double>(x % _im.width, x / _im.width));

	while(active_list.size() > 0) {
		posn<double> candidate = active_list[0];
		int n_id = static_cast<int>(candidate.x) + static_cast<int>(candidate.y) * _im.width;	
		double r = (_grid.begin() + n_id)->density;

		for(int i = 0; i < k; ++i) {
			posn<double> p = getPoint(candidate, r, 2 * r);
			double R = (_grid.begin() + (static_cast<int>(p.x) + static_cast<int>(p.y)*_im.width))->density;
			if(!hasNeighbors(p, R, points) and insertPoint(points, p)) {
				active_list.push_back(p);	
			}
		}

		active_list.pop_front();
	}

	return points;
}

bool
poisson::insertPoint(vector<posn<double>> &points, const posn<double> &point)
{
	int x = static_cast<int>(point.x);
	int y = static_cast<int>(point.y);
	int n = x + y * _im.width;
	if (_grid[n].density >= _im.max_thresh or not _grid[n].valid) return false;
	_grid[n].valid = false;
	_grid[n].id = points.size();
	points.push_back(point);
	double r = _grid[n].density;
	int min_x = std::max(0, static_cast<int>(point.x - r));
	int max_x = std::min(_im.width - 1, static_cast<int>(point.x + r));
	int min_y = std::max(0, static_cast<int>(point.y - r));
	int max_y = std::min(_im.height - 1, static_cast<int>(point.y + r));

	for (int j = min_y; j <= max_y; ++j)
		for (int i = min_x; i <= max_x; ++i)
			if ((x - i) * (x - i) + (y - j) * (y - j) < _im.alpha * pow(r, _im.beta))
				_grid[i+j*_im.width].valid = false;

	return true;
}

posn<double>
poisson::getPoint(const posn<double> &point, double r, double R)
{
	while(1) {
		/* double x = _dist(_generator) * R; */
		/* double y = _dist(_generator) * R; */
		double rad = _dist(_generator) * (R - r) + r;
		double angle = _dist(_generator) * 2 * 3.1415926;

		/* double h = x*x+y*y; */
		/* if (h < r_sqr or h > R_sqr) continue; */
		double x = point.x + rad * cos(angle);
		double y = point.y + rad * sin(angle);

		if (x < 0 or x >= _im.width or y < 0 or y >= _im.height) continue;

		return posn<double>(x, y);
	}
}

bool
poisson::hasNeighbors(posn<double> &point, double r, vector<posn<double>> &points)
{
	vector<int> neighbors;

	int min_x = std::max(0, static_cast<int>(point.x - r));
	int max_x = std::min(_im.width - 1, static_cast<int>(point.x + r));
	int min_y = std::max(0, static_cast<int>(point.y - r));
	int max_y = std::min(_im.height - 1, static_cast<int>(point.y + r));

	for (int j = min_y; j <= max_y; ++j)
		for (int i = min_x; i <= max_x; ++i)
			neighbors.push_back(i+j*_im.width);

	for (auto id_it = neighbors.begin(); id_it != neighbors.end(); ++id_it) {
		lattice_point cur = _grid[*id_it];
		if (cur.valid and cur.id != -1) {
			double x = points[cur.id].x - point.x;
			double y = points[cur.id].y - point.y;
			double d = x * x + y * y;
			if (d < r * r) return true;
		}
	}

	return false;
}

