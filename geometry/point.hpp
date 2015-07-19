#ifndef MODM_GES_POINT_HPP
#define MODM_GES_POINT_HPP

#include <xpcc/math/geometry/vector2.hpp>

namespace modm
{

namespace ges
{

class
Point
{
	inline
	Point(const xpcc::Vector2i &vector) :
		vector(vector)
	{}

public:
	inline
	Point() :
		vector(0, 0)
	{}

	inline
	Point(int16_t x, int16_t y) :
		vector(x, y)
	{}

	inline
	Point(const Point &point) :
		vector(point.vector)
	{}

	inline int16_t
	getX() const
	{
		return vector.getX();
	}

	inline int16_t
	getY() const
	{
		return vector.getY();
	}

	inline void
	setX(int16_t x)
	{
		return vector.setX(x);
	}

	inline void
	setY(int16_t y)
	{
		return vector.setY(y);
	}

	inline void
	set(int16_t x, int16_t y)
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
	{ return vector == xpcc::Vector2i(0); }


	inline Point operator - () const
	{ return -vector; }
	inline Point operator - (const Point &rhs) const
	{ return (vector - rhs.vector); }
	inline Point operator + (const Point &rhs) const
	{ return (vector + rhs.vector); }
	inline int16_t operator * (const Point &rhs) const
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
	xpcc::Vector2i vector;

	friend class Rect;
	friend class Point;
	friend class Line;
};


} // namespace ges

} // namespace modm

#endif // MODM_GES_POINT_HPP

