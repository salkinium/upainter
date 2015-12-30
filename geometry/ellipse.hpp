/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MODM_GES_ELLIPSE_HPP
#define MODM_GES_ELLIPSE_HPP

#include "point.hpp"
#include "rect.hpp"

namespace modm
{

namespace ges
{

class Ellipse
{
public:
	inline
	Ellipse() :
		origin(0,0), size(-1,-1) {}

	inline
	Ellipse(int16_t x, int16_t y, int16_t xRadius, int16_t yRadius) :
		origin(x - xRadius, y - yRadius), size(xRadius*2, yRadius*2) {}

	inline
	Ellipse(const Point &topLeft, const Size &size) :
		origin(topLeft), size(size) {}

	inline
	Ellipse(const Rect &rect) :
		origin(rect.getTopLeft()), size(rect.getSize()) {}

	inline
	Ellipse(const Point &topLeft, const Point &bottomRight) :
		origin(topLeft), size(bottomRight.getX() - topLeft.getX(), bottomRight.getY() - topLeft.getY()) {}


	inline bool
	isEmpty() const
	{ return size.isEmpty(); }

	inline bool
	isNull() const
	{ return size.isNull(); }

	inline bool
	isValid() const
	{ return size.isValid(); }

	inline bool
	isEven() const
	{
		// yes this also works for negative numbers in 2s compliment.
		// although it makes no sense then.
		return not ((size.getHeight() & 0x1) or (size.getWidth() & 0x1));
	}


	inline Rect
	getBounds() const
	{
		return Rect(origin, size);
	}


	// getter origin
	inline Point
	getOrigin() const
	{ return origin; }

	inline int16_t
	getX() const
	{ return origin.getX(); }

	inline int16_t
	getY() const
	{ return origin.getY(); }

	// setter origin
	inline void
	setX(int16_t x)
	{ origin.setX(x); }

	inline void
	setY(int16_t y)
	{ origin.setY(y); }

	inline void
	moveTo(const Point &position)
	{ origin = position; }

	inline void
	moveTo(int16_t x, int16_t y)
	{ origin = Point(x, y); }


	// getter size
	inline Size
	getSize() const
	{ return size; }

	inline int16_t
	getHeight() const
	{ return size.getHeight(); }

	inline int16_t
	getWidth() const
	{ return size.getWidth(); }

	// setter radius
	inline void
	setSize(const Size &size)
	{ this->size = size; }

	inline void
	setHeight(uint16_t height)
	{ size.setHeight(height); }

	inline void
	setWidth(uint16_t width)
	{ size.setWidth(width); }


	// translate
	inline void
	translate(int16_t dx, int16_t dy)
	{ origin += Point(dx, dy); }

	inline void
	translate(const Point &offset)
	{ origin += offset; }

	inline Ellipse
	translated(int16_t dx, int16_t dy) const
	{ return Ellipse(origin + Point(dx, dy), size); }

	inline Ellipse
	translated(const Point &offset) const
	{ return Ellipse(origin + offset, size); }


	// contains
	inline bool
	contains(const Point &point) const
	{ return contains(point.getX(), point.getY()); }

	inline bool
	contains(int16_t x, int16_t y) const
	{
		/* _normalized_ point (x,y) is in ellipse (a,b) determined by following formula
		 *    ~>   x^2 / a^2  +  y^2 / b^2  <=  1.0
		 *
		 * to avoid integer division and the associated loss of precision we rewrite this to:
		 *    ~>   x^2 * b^2  +  y^2 * a^2  <=  a^2 * b^2
		 * since we use diameter instead of radius to avoid issues with uneven width and height, this is actually
		 *    ~>   4 * x^2 * b^2  +  4 * y^2 * a^2  <=  4 * 4 * a^2 * b^2
		 * so to make this equal, the vector between ellipse origin and the point needs to be doubled as well
		 *    ~>   4 * 4 * x^2 * b^2  +  4 * 4 * y^2 * a^2  <=  4 * 4 * a^2 * b^2
		 */
		// diameter squared = a^2 * 4
		int32_t a2 = int32_t(size.getWidth()) * size.getWidth();
		int32_t b2 = int32_t(size.getHeight()) * size.getHeight();

		// (x,y) = 2*P - (2*TL + S)
		int32_t px = int32_t(x) * 2;
		int32_t py = int32_t(y) * 2;

		px -= int32_t(origin.getX()) * 2;
		py -= int32_t(origin.getY()) * 2;

		px -= size.getWidth();
		py -= size.getHeight();

		// = p^2 * 4
		px *= px;
		py *= py;

		return ((px * b2 + py * a2)  <=  (a2 * b2));
	}

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
	intersects(const Ellipse &) const
	{
		return true;
	}

	inline bool
	intersects(const Line &) const
	{
		return true;
	}

	inline bool
	intersects(const Rect &r) const
	{
		{
			Rect margin(r.getTopLeft() - Point(size.getWidth(), size.getHeight()), r.getSize() + size);
			// if the Ellipse origin is outside rect with radius margin
			// then the Ellipse is definitivly NOT in the rect
			if (likely(not margin.contains(origin))) return false;
		}

		// so now the Ellipse might be in the rect.
		// We need to check the radius at all four rectangle corners

		{
			// origin = 2*TL + S
			int32_t ox = int32_t(origin.getX()) * 2 + size.getWidth();
			int32_t oy = int32_t(origin.getY()) * 2 + size.getHeight();

			// we check if the origin point is in any of the four corners of the rect
			uint8_t corner = 0;

			// getLeft() requires no computation, so it is cheap to compare
			if (likely(ox < int32_t(r.getLeft())*2)) corner = 0b0100;
			// otherwise check if origin is to the right of r
			else if (ox > int32_t(r.getRight())*2)   corner = 0b0001;

			// if corner is zero the origin is between the left and right points of the rectangle.
			// this means, that its definitely NOT in any of the corners, so it must intersect.
			if (likely(corner == 0)) return true;

			// corner not zero? then we need to know if origin is in the upper or lower corner

			// getTop() also required no computation, so this is checked first
			if (likely(oy < int32_t(r.getTop())*2)) corner |= 0b0010;
			// otherwise check if origin below bottom
			else if (oy > int32_t(r.getBottom())*2) corner |= 0b1000;

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

	inline bool operator == (const Ellipse &rhs) const
	{ return (origin == rhs.origin and size == rhs.size); }
	inline bool operator != (const Ellipse &rhs) const
	{ return (origin != rhs.origin or size != rhs.size); }

private:
	Point origin;
	Size size;
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_ELLIPSE_HPP
