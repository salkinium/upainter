/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MODM_GES_PAINTER_HPP
#define MODM_GES_PAINTER_HPP

#include "surface.hpp"
#include "color.hpp"
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
	static const CompositionOperator Clear;

	static const CompositionOperator A;
	static const CompositionOperator B;

	static const CompositionOperator AoverB;
	static const CompositionOperator BoverA;

	static const CompositionOperator AinB;
	static const CompositionOperator BinA;

	static const CompositionOperator AoutB;
	static const CompositionOperator BoutA;

	static const CompositionOperator AatopB;
	static const CompositionOperator BatopA;

	static const CompositionOperator Xor;
	static const CompositionOperator Plus;

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
