#ifndef MODM_GES_Size_HPP
#define MODM_GES_Size_HPP

#include <xpcc/math/geometry/vector2.hpp>

namespace modm
{

namespace ges
{

class Size
{
	inline
	Size(xpcc::Vector2i vector) :
		vector(vector) {}

public:
	inline
	Size() :
		vector(-1, -1)
	{}

	inline
	Size(int16_t width, int16_t height) :
		vector(width, height)
	{}

	inline
	Size(const Size &size) :
		vector(size.vector)
	{}

	inline int16_t
	getWidth() const
	{ return vector.getX(); }

	inline int16_t
	getHeight() const
	{ return vector.getY(); }

	inline void
	setWidth(int16_t width)
	{ return vector.setX(width); }

	inline void
	setHeight(int16_t height)
	{ return vector.setY(height); }

	inline bool isEmpty() const
	{
		return (vector <= xpcc::Vector2i(0));
	}

	inline bool isValid() const
	{
		return (vector >= xpcc::Vector2i(0));
	}

	inline bool isNull() const
	{
		return (vector == xpcc::Vector2i(0));
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
	inline int16_t operator * (const Size &rhs) const
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
	xpcc::Vector2i vector;

	friend class Rect;
	friend class Size;
	friend class Line;
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_Size_HPP
