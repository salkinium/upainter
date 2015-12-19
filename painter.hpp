#ifndef MODM_GES_PAINTER_HPP
#define MODM_GES_PAINTER_HPP

#include "surface.hpp"
#include "pixel_color.hpp"
#include <xpcc/ui/animation/interpolation.hpp>
#include <xpcc/math/utils/misc.hpp>

#include "geometry/point.hpp"
#include "geometry/line.hpp"
#include "geometry/rect.hpp"
#include "geometry/circle.hpp"

namespace modm
{

namespace ges
{

template< PixelFormat Format >
class Painter
{
public:
	using Surface = Surface<Format>;
	using NativeColor = typename Surface::NativeColor;
	using AlphaColor = typename NativeColor::AlphaColor;

	using CompositionOperator = void (NativeColor::*)(const AlphaColor);

public:
	// native composition operators
	static constexpr CompositionOperator Clear = &NativeColor::Clear;

	static constexpr CompositionOperator A = &NativeColor::A;
	static constexpr CompositionOperator B = &NativeColor::B;

	static constexpr CompositionOperator AoverB = &NativeColor::AoverB;
	static constexpr CompositionOperator BoverA = &NativeColor::BoverA;

	static constexpr CompositionOperator AinB = &NativeColor::AinB;
	static constexpr CompositionOperator BinA = &NativeColor::BinA;

	static constexpr CompositionOperator AoutB = &NativeColor::AoutB;
	static constexpr CompositionOperator BoutA = &NativeColor::BoutA;

	static constexpr CompositionOperator AatopB = &NativeColor::AatopB;
	static constexpr CompositionOperator BatopA = &NativeColor::BatopA;

	static constexpr CompositionOperator Xor = &NativeColor::Xor;
	static constexpr CompositionOperator Plus = &NativeColor::Plus;

public:
	Painter(Surface &surface) :
		surface(surface) {}


	inline void
	drawLine(const Line &line, const AlphaColor color, Rect clip)
	{ drawLine(line, color, A, clip); }

	void
	drawLine(const Line &line, const AlphaColor color, CompositionOperator composition = A, Rect clip = Rect());


	inline void
	drawRect(const Rect &rectangle, const AlphaColor color, Rect clip)
	{ drawRect(rectangle, color, A, clip); }

	void
	drawRect(const Rect &rectangle, const AlphaColor color, const CompositionOperator composition = A, Rect clip = Rect());


	inline void
	fillRect(const Rect &rectangle, const AlphaColor color, Rect clip)
	{ fillRect(rectangle, color, A, clip); }

	void
	fillRect(const Rect &rectangle, const AlphaColor color, const CompositionOperator composition = A, Rect clip = Rect());


	inline void
	drawCircle(const Circle &circle, const AlphaColor color, Rect clip)
	{ drawCircle(circle, color, A, clip); }

	void
	drawCircle(const Circle &circle, const AlphaColor color, const CompositionOperator composition = A, Rect clip = Rect());


	inline void
	fillCircle(const Circle &circle, const AlphaColor color, Rect clip)
	{ fillCircle(circle, color, A, clip); }

	void
	fillCircle(const Circle &circle, const AlphaColor color, const CompositionOperator composition = A, Rect clip = Rect());


	inline void
	drawEllipse(const Ellipse &ellipse, const AlphaColor color, Rect clip)
	{ drawEllipse(ellipse, color, A, clip); }

	void
	drawEllipse(const Ellipse &ellipse, const AlphaColor color, const CompositionOperator composition = A, Rect clip = Rect());


	inline void
	fillEllipse(const Ellipse &ellipse, const AlphaColor color, Rect clip)
	{ fillEllipse(ellipse, color, A, clip); }

	void
	fillEllipse(const Ellipse &ellipse, const AlphaColor color, const CompositionOperator composition = A, Rect clip = Rect());


protected:
	inline void
	drawEvenEllipse(const Ellipse &ellipse, const AlphaColor color, const CompositionOperator composition, const Rect &clip);

	inline void
	drawOddEllipse(const Ellipse &ellipse, const AlphaColor color, const CompositionOperator composition, const Rect &clip);

	inline void
	fillEvenEllipse(const Ellipse &ellipse, const AlphaColor color, const CompositionOperator composition, const Rect &clip);

	inline void
	fillOddEllipse(const Ellipse &ellipse, const AlphaColor color, const CompositionOperator composition, const Rect &clip);


protected:
	inline void
	drawHorizontalLineClipped(int16_t y, int16_t beginX, int16_t endX, const Rect &clip,
							  const AlphaColor color, const CompositionOperator composition);

	inline void
	drawVerticalLineClipped(int16_t x, int16_t beginY, int16_t endY, const Rect &clip,
							  const AlphaColor color, const CompositionOperator composition);


protected:
	inline void
	drawHorizontalLine(int16_t y, int16_t beginX, int16_t endX,
					   const AlphaColor color, const CompositionOperator composition);

	inline void
	drawVerticalLine(int16_t x, int16_t beginY, int16_t endY,
					 const AlphaColor color, const CompositionOperator composition);

private:
	Surface &surface;
};

} // namespace ges

} // namespace modm

#include "painter_impl.hpp"

#endif // MODM_GES_PAINTER_HPP

