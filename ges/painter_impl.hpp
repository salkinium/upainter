/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MODM_GES_PAINTER_HPP
#	error	"Don't include this file directly, use 'painter.hpp' instead!"
#endif

#include <xpcc/architecture/utils.hpp>

namespace modm { namespace ges {

template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::Clear  {&Painter<Format>::NativeColor::Clear};
template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::A      {&Painter<Format>::NativeColor::A};
template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::B      {&Painter<Format>::NativeColor::B};
template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::AoverB {&Painter<Format>::NativeColor::AoverB};
template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::BoverA {&Painter<Format>::NativeColor::BoverA};
template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::AinB   {&Painter<Format>::NativeColor::AinB};
template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::BinA   {&Painter<Format>::NativeColor::BinA};
template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::AoutB  {&Painter<Format>::NativeColor::AoutB};
template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::BoutA  {&Painter<Format>::NativeColor::BoutA};
template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::AatopB {&Painter<Format>::NativeColor::AatopB};
template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::BatopA {&Painter<Format>::NativeColor::BatopA};
template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::Xor    {&Painter<Format>::NativeColor::Xor};
template< PixelFormat Format >
const typename Painter<Format>::CompositionOperator Painter<Format>::Plus   {&Painter<Format>::NativeColor::Plus};

}}	// oooooh dear cthulhu, C++ is so frickin annoying sometimes.


template< modm::ges::PixelFormat Format >
modm::ges::Painter<Format>::Painter(NativeSurface &surface) : surface(surface), clipRect(surface.getBounds()) {}

template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::setClipArea(const Rect &clip)
{
	clipRect = surface.clip(clip);
}

template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::resetClipArea()
{
	clipRect = surface.getBounds();
}

template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::drawLine(const Line &line, const AlphaColor color, CompositionOperator composition)
{
	if (line.isNull()) return;

	int16_t bX, bY, eX, eY;
	{
		const Line l = line.normalized();

		bX = l.getX1();
		bY = l.getY1();
		eX = l.getX2();
		eY = l.getY2();
	}

	// check if line is vertical
	if (unlikely(bX == eX))
	{
		// check if line is vertically within clipping
		if (bX < clipRect.getLeft() or bX > clipRect.getRight()) return;
		// order correctly
		if (eY < bY) std::swap(bY, eY);
		// check if line is horizintally within clipping
		if (eY < clipRect.getTop() or bY > clipRect.getBottom()) return;

		// clip vertical length
		bY = xpcc::max(bY, int16_t(clipRect.getTop()));
		eY = xpcc::min(eY, int16_t(clipRect.getBottom()));
		// draw visible and clipped line
		drawVerticalLine(bX, bY, eY, color, composition);
		return;
	}

	// check if line is horizontal
	if (unlikely(bY == eY))
	{
		// check if line is horizontally within clipping
		if (bY < clipRect.getTop() or bY > clipRect.getBottom()) return;
		// order correctly
		if (eX < bX) std::swap(bX, eX);
		// check if line is vertically within clipping
		if (eX < clipRect.getLeft() or bX > clipRect.getRight()) return;

		// clip horizontal length
		bX = xpcc::max(bX, int16_t(clipRect.getLeft()));
		eX = xpcc::min(eX, int16_t(clipRect.getRight()));
		// draw visible and clipped line
		drawHorizontalLine(bY, bX, eX, color, composition);
		return;
	}

	{
		// code below is based directly on the paper:
		// Yevgeny P. Kuzmin. Bresenham's Line Generation Algorithm with
		// Built-in Clipping. Computer Graphics Forum, 14(5):275--280, 2005.

		int16_t wbX = clipRect.getLeft();
		int16_t wbY = clipRect.getTop();
		int16_t weX = clipRect.getRight();
		int16_t weY = clipRect.getBottom();

		int16_t xd;
		int16_t yd;
		int16_t* d0 = &xd;
		int16_t* d1 = &yd;


		// is line even visible?
		if (bX > weX or eX < wbX) return;

		int8_t stx = 1;
		int8_t sty = 1;

		// check if points are in Y order <
		if (likely(bY < eY))
		{
			// is line even visible?
			if (bY > weY or eY < wbY) return;
			//sty = 1;	// line goes down (y-axis does down, remember?)
		}
		else
		{
			// is line even visible?
			if (eY > weY or bY < wbY) return;
			sty = -1;	// line goes up (y-axis does down, remember?)

			// invert signs
			bY = -bY; eY = -eY;
			wbY = -wbY; weY = -weY;
			std::swap(wbY, weY);
		}

		// compute absolute x and y deltas
		uint16_t dsx = eX - bX;
		uint16_t dsy = eY - bY;

		// if line is steep, i.e. more y steps than x steps like /
		if (dsx < dsy)
		{
			// use y and x
			d0 = &yd; d1 = &xd;

			// mirror everything on the first quadrant axis
			std::swap(bX,bY);   std::swap(eX,eY);
			std::swap(wbX,wbY); std::swap(weX,weY);
			std::swap(dsx,dsy); std::swap(stx,sty);
		}
		//else {	// already done at beginning!
			// use x and y
		//	d0 = &xd; d1 = &yd;
		//}

		// clipping
		uint_fast16_t dx2 = 2 * dsx;
		const uint_fast16_t dy2 = 2 * dsy;
		xd = bX;
		yd = bY;

		int32_t e = dy2 - dsx;
		bool setexit = false;

		// line starts above the clip window
		if (unlikely(bY < wbY))
		{
			//uint32_t temp = dx2 * (wbY - bY) - dsx;
			uint32_t temp = (2 * (wbY - bY) - 1) * dsx;
			uint_fast16_t msd = temp / dy2;
			xd += msd;

			// line misses the clip window entirely
			if (xd > weX) return;

			// line starts
			if (xd >= wbX)
			{
				uint16_t rem = temp - msd * dy2;

				yd = wbY;
				e -= rem + dsx;

				if (rem > 0)
				{
					xd++;
					e += dy2;
				}
				setexit = true;
			}
		}

		// line starts left of the clip window
		if (unlikely(not setexit and bX < wbX))
		{
			uint32_t temp = dy2 * (wbX - bX);
			uint_fast16_t msd = temp / dx2;
			yd += msd;
			uint16_t rem = temp % dx2;

			// line misses clip window entirely
			if (yd > weY or (yd == weY and rem >= dsx)) return;

			xd = wbX;
			e += rem;

			if (rem >= dsx)
			{
				yd++;
				e -= dx2;
			}
		}

		int16_t term = eX;
		if (eY > weY)
		{
			uint32_t temp = dx2 * (weY - bY) + dsx;
			uint_fast16_t msd = temp / dy2;
			term = bX + msd;

			if ((temp - msd * dy2) == 0) term--;
		}

		if (term > weX) term = weX;
		term++;

		if (sty == -1) yd = -yd;

		if (stx == -1) {
			xd = -xd;
			term = -term;
		}
		dx2 -= dy2;

		// bresenham line drawing
		while (xd != term)
		{
			// if you get a crash here, check your bX,bY,eX,eY coordinates to see
			// they are in a reasonable range -- it could cause integer overflow
			// in this function
			surface.compositePixel(*d0, *d1, color, composition);

			if (e >= 0)
			{
				yd += sty;
				e -= dx2;
			}
			else {
				e += dy2;
			}

			xd += stx;
		}
	}
}


template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::drawRect(const Rect &rectangle, const AlphaColor color, const CompositionOperator composition)
{
	// don't even bother if rect is not in clip area
	if (unlikely(clipRect.isEmpty() or not clipRect.intersects(rectangle))) return;

//		int16_t cl = clipRect.getLeft();	// save some stack
//		int16_t ct = clipRect.getTop();		// save some stack
	int16_t cr = clipRect.getRight();
	int16_t cb = clipRect.getBottom();

//		int16_t rl = rectangle.getLeft();	// save some stack
//		int16_t rt = rectangle.getTop();	// save some stack
	int16_t rr = rectangle.getRight();
	int16_t rb = rectangle.getBottom();

	int16_t ml = xpcc::max(rectangle.getLeft(), clipRect.getLeft());
	int16_t mt = clipRect.getTop();	// decided at top line
	int16_t mr = xpcc::min(rr, cr);
	int16_t mb = cb;			// decided at bottom line

	if (likely(clipRect.getTop() <= rectangle.getTop())) {	// top line
		mt = rectangle.getTop();
		drawHorizontalLine(rectangle.getTop(), ml+1, mr-1, color, composition);
	}

	if (likely(rb <= cb)) {	// bottom line
		mb = rb;
		drawHorizontalLine(rb, ml+1, mr-1, color, composition);
	}

	if (likely(clipRect.getLeft() <= rectangle.getLeft())) {	// left line
		drawVerticalLine(rectangle.getLeft(), mt, mb, color, composition);
	}

	if (likely(rr <= cr)) {	// right line
		drawVerticalLine(rr, mt, mb, color, composition);
	}
}

template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::fillRect(const Rect &rectangle, const AlphaColor color, const CompositionOperator composition)
{
	Rect clip = rectangle.intersected(clipRect);
	// there is no need to test for isEmpty() !

	for (int16_t yy = clip.getTop(); yy <= clip.getBottom(); yy++)
	{
		drawHorizontalLine(yy, clip.getLeft(), clip.getRight(), color, composition);
	}
}


template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::drawCircle(const Circle &circle, const AlphaColor color, const CompositionOperator composition)
{
	// we don't draw empty circles
	if (unlikely(circle.isEmpty())) return;

	// only draw intersecting circles
	if (unlikely(not circle.intersects(clipRect))) return;

	// only draw a pixel if radius is zero
	if (unlikely(circle.isNull())) {
		surface.compositePixel(circle.getX(), circle.getY(), color, composition);
		return;
	}

	// start the drawing
	int16_t r = circle.getRadius();
	int16_t x = -r;
	int16_t y = 0;
	int16_t err = 2 - 2 * r;

	do {
		/* clipping circles seems difficult, because:
		 *  1. the clipping rect can create 4 arcs that need to be drawn
		 *  2. we need to be able to draw arbitrary arcs with bresenham
		 *
		 * Typical clipping strategies subdivide the circle into its 8 octants and
		 * then only render those that are (partially) visible.
		 * The individual octants are clipped using guard band clipping.
		 *
		 * I haven't found a good arc rasterization algorithm based on this one yet,
		 * so this uses the cheesy guard band clipping way for every pixel.
		 */
		if (clipRect.contains(circle.getX() - x, circle.getY() + y)) surface.compositePixel(circle.getX() - x, circle.getY() + y, color, composition);	//   I. Quadrant +x +y
		if (clipRect.contains(circle.getX() - y, circle.getY() - x)) surface.compositePixel(circle.getX() - y, circle.getY() - x, color, composition);	//  II. Quadrant -x +y
		if (clipRect.contains(circle.getX() + x, circle.getY() - y)) surface.compositePixel(circle.getX() + x, circle.getY() - y, color, composition);	// III. Quadrant -x -y
		if (clipRect.contains(circle.getX() + y, circle.getY() + x)) surface.compositePixel(circle.getX() + y, circle.getY() + x, color, composition);	//  IV. Quadrant +x -y

		r = err;

		if (r <= y) err += ++y * 2 + 1;		// e_xy+e_y < 0

		if (r > x or err > y) {			// e_xy+e_x > 0 or no 2nd y-step
			err += ++x * 2 + 1;				// -> x-step now
		}
	}
	while (x < 0);
}

template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::fillCircle(const Circle &circle, const AlphaColor color, const CompositionOperator composition)
{
	// we don't draw empty circles
	if (unlikely(circle.isEmpty())) return;

	// only draw intersecting circles
	if (unlikely(not circle.intersects(clipRect))) return;

	// only draw a pixel if radius is zero
	if (unlikely(circle.isNull())) {
		surface.compositePixel(circle.getX(), circle.getY(), color, composition);
		return;
	}

	// start the drawing
	int16_t r = circle.getRadius();
	int16_t x = -r;
	int16_t y = 1;
	int16_t err = 2 - 2 * r;
	int16_t prevY = 0;

	drawHorizontalLineClipped(circle.getY(), circle.getX() - r, circle.getX() + r, color, composition);

	do
	{
		if (y != prevY)
		{
			// above origin, left to right
			drawHorizontalLineClipped(circle.getY() + y, circle.getX() + x, circle.getX() - x, color, composition);
			// below left to right
			drawHorizontalLineClipped(circle.getY() - y, circle.getX() + x, circle.getX() - x, color, composition);
			prevY = y;
		}

		r = err;

		if (r <= y) err += ++y * 2 + 1;		// e_xy + e_y < 0

		if (r > x or err > y)
		{			// e_xy + e_x > 0 or no 2nd y-step
			err += ++x * 2 + 1;				// -> x-step now
		}
	}
	while (x < 0);
}


template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::drawEllipse(const Ellipse &ellipse, const AlphaColor color, const CompositionOperator composition)
{
	// we don't draw empty circles
	if (unlikely(ellipse.isEmpty())) return;

	// only draw intersecting circles
	if (unlikely(not ellipse.intersects(clipRect))) return;

	// only draw a pixel if radius is zero
	if (unlikely(ellipse.isNull())) {
		surface.compositePixel(ellipse.getX(), ellipse.getY(), color, composition);
		return;
	}

	if (ellipse.isEven())
	{
		drawEvenEllipse(ellipse, color, composition);
	}
	else
	{
		drawOddEllipse(ellipse, color, composition);
	}
}

template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::drawEvenEllipse(const Ellipse &ellipse, const AlphaColor color, const CompositionOperator composition)
{
	int16_t a = ellipse.getWidth() / 2;
	int16_t b = ellipse.getHeight() / 2;

	int32_t x = -a, y = 0;					// II. quadrant from bottom left to top right
	int32_t e2 = b, dx = (1+2*x)*e2*e2;	// error increment
	int32_t dy = x*x, err = dx+dy;			// error of 1.step
	int16_t xm = ellipse.getX() + a;
	int16_t ym = ellipse.getY() + b;

	do {
		if (clipRect.contains(xm-x, ym+y)) surface.compositePixel(xm-x, ym+y, color, composition);	//   I. Quadrant
		if (clipRect.contains(xm+x, ym+y)) surface.compositePixel(xm+x, ym+y, color, composition);	//  II. Quadrant
		if (clipRect.contains(xm+x, ym-y)) surface.compositePixel(xm+x, ym-y, color, composition);	// III. Quadrant
		if (clipRect.contains(xm-x, ym-y)) surface.compositePixel(xm-x, ym-y, color, composition);	//  IV. Quadrant

		e2 = 2*err;
		if (e2 >= dx) { x++; err += dx += 2*int32_t(b)*b; }	// x step
		if (e2 <= dy) { y++; err += dy += 2*int32_t(a)*a; }	// y step
	}
	while (x <= 0);

	// too early stop for flat ellipses with a=1
	if (unlikely(xm >= clipRect.getLeft() and xm <= clipRect.getRight()))
	{
		while (y++ < b)
		{
			// -> finish tip of ellipse
			if (ym+y >= clipRect.getTop() and ym+y <= clipRect.getBottom()) surface.compositePixel(xm, ym+y, color, composition);
			if (ym-y >= clipRect.getTop() and ym-y <= clipRect.getBottom()) surface.compositePixel(xm, ym-y, color, composition);
		}
	}
}

template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::drawOddEllipse(const Ellipse &ellipse, const AlphaColor color, const CompositionOperator composition)
{
	int16_t x0 = ellipse.getX();
	int16_t y0 = ellipse.getY();
	int16_t x1 = x0 + ellipse.getWidth();
	int16_t y1 = y0 + ellipse.getHeight();

	int32_t a = abs(x1-x0), b = abs(y1-y0), b1 = b & 1;           /* diameter */
	int64_t dx = 4*(1.0-a)*b*b, dy = 4*(b1+1)*a*a;         /* error increment */
	int64_t err = dx+dy+b1*a*a, e2;                        /* error of 1.step */

	y0 += (b+1)/2; y1 = y0-b1;                              /* starting pixel */
	a = 8*a*a; b1 = 8*b*b;

	do
	{
		if (clipRect.contains(x1, y0)) surface.compositePixel(x1, y0, color, composition);	//   I. Quadrant
		if (clipRect.contains(x0, y0)) surface.compositePixel(x0, y0, color, composition);	//  II. Quadrant
		if (clipRect.contains(x0, y1)) surface.compositePixel(x0, y1, color, composition);	// III. Quadrant
		if (clipRect.contains(x1, y1)) surface.compositePixel(x1, y1, color, composition);	//  IV. Quadrant
		e2 = 2*err;
		if (e2 <= dy) { ++y0; --y1; err += dy += a; }                 /* y step */
		if (e2 >= dx || 2*err > dy) { ++x0; --x1; err += dx += b1; }  /* x step */
	}
	while (x0 <= x1);

	// too early stop of flat ellipses a=1
	while (y0-y1 <= b)
	{
		// -> finish tip of ellipse
		if (clipRect.contains(x0-1, y0)) surface.compositePixel(x0-1, y0, color, composition);
		if (clipRect.contains(x1+1, y0)) surface.compositePixel(x1+1, y0, color, composition);
		if (clipRect.contains(x0-1, y1)) surface.compositePixel(x0-1, y1, color, composition);
		if (clipRect.contains(x1+1, y1)) surface.compositePixel(x1+1, y1, color, composition);
		++y0; --y1;
	}
}

template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::fillEllipse(const Ellipse &ellipse, const AlphaColor color, const CompositionOperator composition)
{
	// we don't draw empty circles
	if (unlikely(ellipse.isEmpty())) return;

	// only draw intersecting circles
	if (unlikely(not ellipse.intersects(clipRect))) return;

	// only draw a pixel if radius is zero
	if (unlikely(ellipse.isNull())) {
		surface.compositePixel(ellipse.getX(), ellipse.getY(), color, composition);
		return;
	}

	if (ellipse.isEven())
	{
		fillEvenEllipse(ellipse, color, composition);
	}
	else
	{
		fillOddEllipse(ellipse, color, composition);
	}
}

template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::fillEvenEllipse(const Ellipse &ellipse, const AlphaColor color, const CompositionOperator composition)
{
	int16_t a = ellipse.getWidth() / 2;
	int16_t b = ellipse.getHeight() / 2;

	int32_t x = -a, y = 0;					// II. quadrant from bottom left to top right
	int32_t e2 = b, dx = (1+2*x)*e2*e2;	// error increment
	int32_t dy = x*x, err = dx+dy;			// error of 1.step
	int16_t xm = ellipse.getX() + a;
	int16_t ym = ellipse.getY() + b;
	int16_t yPrev = 0;

	drawHorizontalLineClipped(ym-y, xm+x, xm-x, color, composition);

	do
	{
		if (yPrev != y)
		{
			drawHorizontalLineClipped(ym+y, xm+x, xm-x, color, composition);
			drawHorizontalLineClipped(ym-y, xm+x, xm-x, color, composition);
			yPrev = y;
		}

		e2 = 2*err;
		if (e2 >= dx) { x++; err += dx += 2*int32_t(b)*b; }	// x step
		if (e2 <= dy) { y++; err += dy += 2*int32_t(a)*a; }	// y step
	}
	while (x <= 0);

	// too early stop for flat ellipses with a=1
	// -> finish tip of ellipse
	if (unlikely(y < b))
	{
		drawVerticalLineClipped(xm, ym+y, ym+b, color, composition);
		drawVerticalLineClipped(xm, ym-y, ym-b, color, composition);
	}
}

template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::fillOddEllipse(const Ellipse &ellipse, const AlphaColor color, const CompositionOperator composition)
{
	int16_t x0 = ellipse.getX();
	int16_t y0 = ellipse.getY();
	int16_t x1 = x0 + ellipse.getWidth();
	int16_t y1 = y0 + ellipse.getHeight();

	int32_t a = abs(x1-x0), b = abs(y1-y0), b1 = b & 1;           /* diameter */
	int64_t dx = 4*(1.0-a)*b*b, dy = 4*(b1+1)*a*a;         /* error increment */
	int64_t err = dx+dy+b1*a*a, e2;                        /* error of 1.step */

	y0 += (b+1)/2; y1 = y0-b1;                              /* starting pixel */
	a = 8*a*a; b1 = 8*b*b;

	int16_t yPrev = y0;
	drawHorizontalLineClipped(y0, x0, x1, color, composition);
	if (unlikely(y0 != y1)) drawHorizontalLineClipped(y1, x0, x1, color, composition);

	do
	{
		if (yPrev != y0)
		{
			drawHorizontalLineClipped(y0, x0, x1, color, composition);
			drawHorizontalLineClipped(y1, x0, x1, color, composition);
			yPrev = y0;
		}

		e2 = 2*err;
		if (e2 <= dy) { ++y0; --y1; err += dy += a; }                 /* y step */
		if (e2 >= dx || 2*err > dy) { ++x0; --x1; err += dx += b1; }  /* x step */
	}
	while (x0 <= x1);

	// too early stop of flat ellipses a=1
	while (y0-y1 < b)
	{
		// -> finish tip of ellipse
		++y0; --y1;
		drawHorizontalLineClipped(y0, x0-1, x1+1, color, composition);
		drawHorizontalLineClipped(y1, x0-1, x1+1, color, composition);
	}
}


template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::drawHorizontalLineClipped(int16_t y, int16_t beginX, int16_t endX,
						  const AlphaColor color, const CompositionOperator composition)
{
	if (y >= clipRect.getTop() and y <= clipRect.getBottom())
	{
		// Y is in clip window
		int16_t cR = clipRect.getRight();
		if (likely(beginX <= cR))
		{
			// line starts before right of clip window
			drawHorizontalLine(y, std::max(beginX, int16_t(clipRect.getLeft())), std::min(endX, cR), color, composition);
		}
	}
}

template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::drawVerticalLineClipped(int16_t x, int16_t beginY, int16_t endY,
						  const AlphaColor color, const CompositionOperator composition)
{
	if (x >= clipRect.getLeft() and x <= clipRect.getRight())
	{
		// Y is in clip window
		int16_t cB = clipRect.getBottom();
		if (likely(beginY <= cB))
		{
			// line starts before right of clip window
			drawVerticalLine(x, std::max(beginY, int16_t(clipRect.getTop())), std::min(endY, cB), color, composition);
		}
	}
}


template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::drawHorizontalLine(int16_t y, int16_t beginX, int16_t endX,
				   const AlphaColor color, const CompositionOperator composition)
{
	for (int16_t xx = beginX; xx <= endX; xx++)
	{
		surface.compositePixel(xx, y, color, composition);
	}
}

template< modm::ges::PixelFormat Format >
void
modm::ges::Painter<Format>::drawVerticalLine(int16_t x, int16_t beginY, int16_t endY,
				 const AlphaColor color, const CompositionOperator composition)
{
	for (int16_t yy = beginY; yy <= endY; yy++)
	{
		surface.compositePixel(x, yy, color, composition);
	}
}

