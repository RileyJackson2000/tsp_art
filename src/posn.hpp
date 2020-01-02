#ifndef __POSN
#define __POSN

template<typename T>
struct posn {
	T x, y;

	posn()
	: x{0}, y{0} { }

	posn(T x, T y)
	: x{x}, y{y} { }
};

#endif

