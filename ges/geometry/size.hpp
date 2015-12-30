/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MODM_GES_SIZE_HPP
#define MODM_GES_SIZE_HPP

#include <xpcc/math/geometry/vector2.hpp>
#include "../ges.hpp"

namespace modm
{

namespace ges
{

class Size
{
	inline
	Size(Vector vector) :
		vector(vector) {}

public:
	inline
	Size() :
		vector(-1, -1)
	{}

	inline
	Size(coord_t width, coord_t height) :
		vector(width, height)
	{}

	inline
	Size(const Size &size) :
		vector(size.vector)
	{}

	inline coord_t
	getWidth() const
	{ return vector.getX(); }

	inline coord_t
	getHeight() const
	{ return vector.getY(); }

	inline void
	setWidth(coord_t width)
	{ return vector.setX(width); }

	inline void
	setHeight(coord_t height)
	{ return vector.setY(height); }

	inline bool isEmpty() const
	{
		return (vector <= Vector(0,0));
	}

	inline bool isValid() const
	{
		return (vector >= Vector(0,0));
	}

	inline bool isNull() const
	{
		return (vector == Vector(0,0));
	}

	inline void transpose()
	{
		vector.set(vector.getY(), vector.getX());
	}

	inline Size transposed() const
	{
		return Size(vector.getY(), vector.getX());
	}

	inline Size operator - (const Size &rhs) const
	{ return (vector - rhs.vector); }
	inline Size operator + (const Size &rhs) const
	{ return (vector + rhs.vector); }
	inline coord_t operator * (const Size &rhs) const
	{ return (vector * rhs.vector); }
	inline Size operator * (const float scale) const
	{ return (vector * scale); }
	inline Size operator / (const float scale) const
	{ return (vector / scale); }

	inline Size& operator += (const Size &rhs)
	{ vector += rhs.vector; return *this; }
	inline Size& operator -= (const Size &rhs)
	{ vector -= rhs.vector; return *this; }
	inline Size& operator *= (const float scale)
	{ vector = (vector * scale); return *this; }
	inline Size& operator /= (const float scale)
	{ vector = (vector / scale); return *this; }

	inline bool operator == (const Size &rhs) const
	{ return vector == rhs.vector; }
	inline bool operator != (const Size &rhs) const
	{ return vector != rhs.vector; }
	inline bool operator < (const Size &rhs) const
	{ return vector < rhs.vector; }
	inline bool operator <= (const Size &rhs) const
	{ return vector <= rhs.vector; }
	inline bool operator > (const Size &rhs) const
	{ return vector > rhs.vector; }
	inline bool operator >= (const Size &rhs) const
	{ return vector >= rhs.vector; }

private:
	Vector vector;

	friend class Rect;
	friend class Size;
	friend class Line;
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_SIZE_HPP
