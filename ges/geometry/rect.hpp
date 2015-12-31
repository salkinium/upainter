/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MODM_GES_RECT_HPP
#define MODM_GES_RECT_HPP

#include <xpcc/math/geometry/vector2.hpp>
#include "point.hpp"
#include "size.hpp"
#include <xpcc/math/utils/misc.hpp>

namespace modm
{

namespace ges
{

class Rect
{
public:
	inline Rect() = default;
	inline Rect(const Rect&) = default;

	inline explicit
	Rect(const Point &topLeft, const Point &bottomRight) :
		origin(topLeft), size((bottomRight - topLeft).vector) {}

	inline
	Rect(const Point &origin, const Size &size) :
		origin(origin), size(size) {}

	inline
	Rect(coord_t x, coord_t y, coord_t width, coord_t height) :
		origin(x, y), size(width, height) {}


	inline bool
	isEmpty() const
	{ return size.isEmpty(); }

	inline bool
	isNull() const
	{ return size.isNull(); }

	inline bool
	isValid() const
	{ return not isEmpty(); }


	inline void
	adjust(coord_t dx1, coord_t dy1, coord_t dx2, coord_t dy2)
	{ origin += Point(dx1, dy1); size += Size(dx2 - dx1, dy2 - dy1); }

	inline Rect
	adjusted(coord_t dx1, coord_t dy1, coord_t dx2, coord_t dy2) const
	{ return Rect(origin + Point(dx1, dy1), size + Size(dx2 - dx1, dy2 - dy1)); }

	inline void
	setRect(coord_t x, coord_t y, coord_t width, coord_t height)
	{ origin = Point(x, y); size = Size(width, height); }

	inline void
	setCoords(coord_t x1, coord_t y1, coord_t x2, coord_t y2)
	{ origin = Point(x1, y1); size = Size(x2 - x1, y2 - y1); }


	// getter origin
	inline Point
	getOrigin() const
	{ return origin; }

	inline coord_t
	getX() const
	{ return origin.getX(); }

	inline coord_t
	getY() const
	{ return origin.getY(); }

	// setter origin
	inline void
	setX(coord_t x)
	{ origin.setX(x); }

	inline void
	setY(coord_t y)
	{ origin.setY(y); }

	inline void
	moveTo(const Point &position)
	{ origin = position; }

	inline void
	moveTo(coord_t x, coord_t y)
	{ origin = Point(x, y); }


	// getter size
	inline Size
	getSize() const
	{ return size; }

	inline coord_t
	getWidth() const
	{ return size.getWidth(); }

	inline coord_t
	getHeight() const
	{ return size.getHeight(); }

	// setter size
	inline void
	setSize(const Size &size)
	{ this->size = size; }

	inline void
	setWidth(coord_t width)
	{ size.setWidth(width); }

	inline void
	setHeight(coord_t height)
	{ size.setHeight(height); }


	// getter lines
	inline coord_t
	getBottom() const
	{ return getY() + getHeight(); }

	inline coord_t
	getTop() const
	{ return getY(); }

	inline coord_t
	getLeft() const
	{ return getX(); }

	inline coord_t
	getRight() const
	{ return getX() + getWidth(); }

	// setter lines
	inline void
	setBottom(coord_t y)
	{ setHeight(y - getTop()); }

	inline void
	setTop(coord_t y)
	{ setHeight(getBottom() - y); setY(y); }

	inline void
	setLeft(coord_t x)
	{ setWidth(getRight() - x); setX(x); }

	inline void
	setRight(coord_t x)
	{ setWidth(x - getLeft()); }

	// move setter lines
	inline void
	moveBottom(coord_t y)
	{ setY(y - getHeight()); }

	inline void
	moveTop(coord_t y)
	{ setY(y); }

	inline void
	moveLeft(coord_t x)
	{ setX(x); }

	inline void
	moveRight(coord_t x)
	{ setX(x - getWidth()); }


	// getter points
	inline Point
	getTopLeft() const
	{ return origin; }

	inline Point
	getTopRight() const
	{ return Point(getTop(), getRight()); }

	inline Point
	getBottomLeft() const
	{ return Point(getLeft(), getBottom()); }

	inline Point
	getBottomRight() const
	{ return Point(origin.vector + size.vector); }

	inline Point
	getCenter() const
	{ return Point(origin + Point(size.getWidth() / 2, size.getHeight() / 2)); }

	// setter points
	inline void
	setTopLeft(const Point &position)
	{ size = (getBottomRight() - position).vector; origin = position; }

	inline void
	setTopRight(const Point &position)
	{ setTop(position.getY()); setRight(position.getX()); }

	inline void
	setBottomLeft(const Point &position)
	{ setBottom(position.getY()); setRight(position.getX()); }

	inline void
	setBottomRight(const Point &position)
	{ size = (position - origin).vector; }

	// move setter points
	inline void
	moveTopLeft(const Point &position)
	{ origin = position; }

	inline void
	moveTopRight(const Point &position)
	{ origin = Point(position.getX() - getWidth(), position.getY()); }

	inline void
	moveBottomLeft(const Point &position)
	{ origin = Point(position.getX(), position.getY() - getHeight()); }

	inline void
	moveBottomRight(const Point &position)
	{ origin = position.vector - size.vector; }

	inline void
	moveCenter(const Point &position)
	{ origin = position - Point(size.getWidth() / 2, size.getHeight() / 2); }


	inline void
	translate(coord_t dx, coord_t dy)
	{ origin += Point(dx, dy); }

	inline void
	translate(const Point &offset)
	{ origin += offset; }

	inline Rect
	translated(coord_t dx, coord_t dy) const
	{ return Rect(origin + Point(dx, dy), size); }

	inline Rect
	translated(const Point &offset) const
	{ return Rect(origin + offset, size); }


	// order
	inline Rect
	normalized() const
	{
		Rect norm = *this;
		if (unlikely(getWidth() < 0)) {
			// "swap" left and right corners
			norm.setX( getX() + getWidth() );
			norm.setWidth( -getWidth() );
		}
		if (unlikely(getHeight() < 0)) {
			norm.setY( getY() + getHeight() );
			norm.setHeight( -getHeight() );
		}
		return norm;
	}

	inline void
	normalize()
	{
		*this = normalized();
	}

	// contains
	inline bool
	contains(const Point &point) const
	{ return contains(point.getX(), point.getY()); }

	inline bool
	contains(coord_t x, coord_t y) const
	{
		// left <= x <= right and top <= y <= bottom
		// we have to check all four anyway
		// that's why we use OR here to abort early
		// when not in rect.
		return not (x < getLeft() or getRight() < x or
					y < getTop() or getBottom() < y);
	}

	inline bool
	contains(const Rect &r) const
	{
		// r.origin <= origin and r.size <= size
		return not (r.getTopLeft() < getTopLeft() or r.getSize() > getSize());
	}


	// intersections
	inline bool
	intersects(const Rect &r) const
	{
		return not (r.getLeft() > getRight() or
					r.getRight() < getLeft() or
					r.getTop() > getBottom() or
					r.getBottom() < getTop());
	}

	inline Rect
	intersected(const Rect &r) const
	{
		if (likely(intersects(r)))
		{
			return Rect(Point(xpcc::max(getLeft(),  r.getLeft()),  xpcc::max(getTop(),    r.getTop())),
						Point(xpcc::min(getRight(), r.getRight()), xpcc::min(getBottom(), r.getBottom())));
		}

		return Rect();
	}

	inline Rect
	united(const Rect &r) const
	{
		return Rect(Point(xpcc::min(getLeft(),  r.getLeft()),  xpcc::min(getTop(),    r.getTop())),
					Point(xpcc::max(getRight(), r.getRight()), xpcc::max(getBottom(), r.getBottom())));
	}


private:
	Point origin;
	Size size;
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_RECT_HPP
