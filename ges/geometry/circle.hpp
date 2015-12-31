/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MODM_GES_CIRCLE_HPP
#define MODM_GES_CIRCLE_HPP

#include "point.hpp"
#include "rect.hpp"
#include "ellipse.hpp"

namespace modm
{

namespace ges
{

class Circle
{
public:
	inline Circle() = default;
	inline Circle(const Circle&) = default;

	inline
	Circle(coord_t x, coord_t y, coord_t radius) :
		origin(x, y), radius(radius) {}

	inline
	Circle(const Point &origin, coord_t radius) :
		origin(origin), radius(radius) {}


	inline bool
	isEmpty() const
	{ return radius <= 0; }

	inline bool
	isNull() const
	{ return radius == 0; }

	inline bool
	isValid() const
	{ return radius > 0; }


	inline Rect
	getBounds() const
	{
		return Rect(origin - Point(radius, radius), Size(radius*2, radius*2));
	}

	inline Ellipse
	getEllipse() const
	{
		return Ellipse(origin.getX() - radius, origin.getY() - radius, radius*2, radius*2);
	}


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


	// getter radius
	inline coord_t
	getRadius() const
	{ return radius; }

	// setter radius
	inline void
	setRadius(coord_t radius)
	{ this->radius = radius; }


	// translate
	inline void
	translate(coord_t dx, coord_t dy)
	{ origin += Point(dx, dy); }

	inline void
	translate(const Point &offset)
	{ origin += offset; }

	inline Circle
	translated(coord_t dx, coord_t dy) const
	{ return Circle(origin + Point(dx, dy), radius); }

	inline Circle
	translated(const Point &offset) const
	{ return Circle(origin + offset, radius); }


	// contains
	inline bool
	contains(const Point &point) const
	{ return wide_coord_t(origin.distanceSquared(point)) <= (wide_coord_t(radius) * wide_coord_t(radius)); }

	inline bool
	contains(coord_t x, coord_t y) const
	{ return contains(Point(x, y)); }

	inline bool
	contains(const Line &l) const
	{ return contains(l.getP1()) and contains(l.getP2()); }

	inline bool
	contains(const Rect &r) const
	{
		return	contains(r.getTopLeft()) and contains(r.getTopRight()) and
				contains(r.getBottomLeft()) and contains(r.getBottomRight());
	}


	// intersections
	inline bool
	intersects(const Circle &c) const
	{
		coord_t r = radius + c.radius;
		return (origin.distanceSquared(c.origin) <= uint16_t(r * r));
		// well... that was simple.
	}

	inline bool
	intersects(const Line &l) const
	{
		// http://math.stackexchange.com/questions/275529/check-if-line-intersects-with-circles-perimeter
		wide_coord_t a = l.getX2() - l.getX1();
		wide_coord_t b = l.getY1() - l.getY2();
		wide_coord_t c = (-a) * l.getY1() + (-b) * l.getX1();

		wide_coord_t d = a * origin.getX() + b * origin.getY() + c;
		d *= d;	// square it, now definitely positive

		wide_coord_t r = radius;
		r *= a * a + b * b;

		return d <= r;
	}

	inline bool
	intersects(const Rect &r) const
	{
		{
			Rect margin(r.getTopLeft() - Point(radius, radius), r.getSize() + Size(2*radius, 2*radius));
			// if the circle origin is outside rect with radius margin
			// then the circle is definitivly NOT in the rect
			if (unlikely(not margin.contains(origin))) return false;
		}

		// so now the circle might be in the rect.
		// We need to check the radius at all four rectangle corners

		{
			// we check if the origin point is in any of the four corners of the rect
			uint8_t corner = 0;

			// getLeft() requires no computation, so it is cheap to compare
			if (likely(origin.getX() < r.getLeft())) corner = 0b0100;
			// otherwise check if origin is to the right of r
			else if (origin.getX() > r.getRight())   corner = 0b0001;

			// if corner is zero the origin is between the left and right points of the rectangle.
			// this means, that its definitely NOT in any of the corners, so it must intersect.
			if (likely(corner == 0)) return true;

			// corner not zero? then we need to know if origin is in the upper or lower corner

			// getTop() also required no computation, so this is checked first
			if (likely(origin.getY() < r.getTop())) corner |= 0b0010;
			// otherwise check if origin below bottom
			else if (origin.getY() > r.getBottom()) corner |= 0b1000;

			// now simply check the corners
			switch(corner)
			{
				case 0b0011:	// top right corner
					return contains(r.getTopRight());
				case 0b0110:	// top left corner
					return contains(r.getTopLeft());
				case 0b1100:	// bottom left corner
					return contains(r.getBottomLeft());
				case 0b1001:	// bottom right corner
					return contains(r.getBottomRight());
				default:		// origin is to the left or right but vertically in the middle
					break;
			}
		}
		return true;
	}

	inline bool operator == (const Circle &rhs) const
	{ return (origin == rhs.origin and radius == rhs.radius); }
	inline bool operator != (const Circle &rhs) const
	{ return (origin != rhs.origin or radius != rhs.radius); }

private:
	Point origin;
	coord_t radius{0};
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_CIRCLE_HPP
