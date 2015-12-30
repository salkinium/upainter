/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MODM_GES_POINT_HPP
#define MODM_GES_POINT_HPP

#include <xpcc/math/geometry/vector2.hpp>
#include <xpcc/architecture/utils.hpp>
#include "../ges.hpp"

namespace modm
{

namespace ges
{

class
Point
{
	inline
	Point(const Vector &vector) :
		vector(vector)
	{}

public:
	inline
	Point() :
		vector(0, 0)
	{}

	inline
	Point(coord_t x, coord_t y) :
		vector(x, y)
	{}

	inline
	Point(const Point &point) :
		vector(point.vector)
	{}

	inline coord_t
	getX() const
	{
		return vector.getX();
	}

	inline coord_t
	getY() const
	{
		return vector.getY();
	}

	inline void
	setX(coord_t x)
	{
		return vector.setX(x);
	}

	inline void
	setY(coord_t y)
	{
		return vector.setY(y);
	}

	inline void
	set(coord_t x, coord_t y)
	{
		return vector.set(x, y);
	}


	static inline float
	distance(const Point &point1, const Point &point2)
	{
		float squared = (point2.vector - point1.vector).getLengthSquared();
		return std::sqrt(squared);
	}

	inline float
	distance(const Point &point) const
	{
		return Point::distance(*this, point);
	}

	static inline uint32_t
	distanceSquared(const Point &point1, const Point &point2)
	{
		// not using `xpcc::Vector::getLengthSquared()` to avoid float type
		Point d = point2 - point1;
		return (int32_t(d.getX()) * d.getX() + int32_t(d.getY()) * d.getY());
	}

	inline uint16_t
	distanceSquared(const Point &point) const
	{
		return Point::distanceSquared(*this, point);
	}


	inline bool
	isNull() const
	{ return vector == Vector(0,0); }


	inline Point operator - () const
	{ return -vector; }
	inline Point operator - (const Point &rhs) const
	{ return (vector - rhs.vector); }
	inline Point operator + (const Point &rhs) const
	{ return (vector + rhs.vector); }
	inline coord_t operator * (const Point &rhs) const
	{ return (vector * rhs.vector); }
	inline Point operator * (const float scale) const
	{ return (vector * scale); }
	inline Point operator / (const float scale) const
	{ return (vector / scale); }

	inline Point& operator += (const Point &rhs)
	{ vector += rhs.vector; return *this; }
	inline Point& operator -= (const Point &rhs)
	{ vector -= rhs.vector; return *this; }
	inline Point& operator *= (const float scale)
	{ vector = (vector * scale); return *this; }
	inline Point& operator /= (const float scale)
	{ vector = (vector / scale); return *this; }

	// comparisons
	inline bool operator == (const Point &rhs) const
	{ return vector == rhs.vector; }
	inline bool operator != (const Point &rhs) const
	{ return vector != rhs.vector; }
	inline bool operator < (const Point &rhs) const
	{ return vector < rhs.vector; }
	inline bool operator <= (const Point &rhs) const
	{ return vector <= rhs.vector; }
	inline bool operator > (const Point &rhs) const
	{ return vector > rhs.vector; }
	inline bool operator >= (const Point &rhs) const
	{ return vector >= rhs.vector; }

private:
	Vector vector;

	friend class Rect;
	friend class Point;
	friend class Line;
};


} // namespace ges

} // namespace modm

#endif // MODM_GES_POINT_HPP

