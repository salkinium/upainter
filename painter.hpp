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

	using CompositionOperator = void (NativeColor::*)(const AlphaColor &color);

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



public:
	inline void
	drawLine(const Line &line, const AlphaColor &color, Rect clip)
	{
		drawLine(line, color, A, clip);
	}

	void
	drawLine(const Line &line, const AlphaColor &color, CompositionOperator composition = A, Rect clip = Rect())
	{
		if (line.isNull()) return;

		clip = surface.clip(clip);

		int16_t bX, bY, eX, eY;
		{
			const Line l = line.normalized();

			bX = l.getX1();
			bY = l.getY1();
			eX = l.getX2();
			eY = l.getY2();
		}

		// check if line is vertical
		if (bX == eX)
		{
			// check if line is vertically within clipping
			if (bX < clip.getLeft() or bX > clip.getRight()) return;
			// order correctly
			if (eY < bY) std::swap(bY, eY);
			// check if line is horizintally within clipping
			if (eY < clip.getTop() or bY > clip.getBottom()) return;

			// clip vertical length
			bY = xpcc::max(bY, clip.getTop());
			eY = xpcc::min(eY, clip.getBottom());
			// draw visible and clipped line
			drawVerticalLine(bX, bY, eY, color, composition);
			return;
		}

		// check if line is horizontal
		if (bY == eY)
		{
			// check if line is horizontally within clipping
			if (bY < clip.getTop() or bY > clip.getBottom()) return;
			// order correctly
			if (eX < bX) std::swap(bX, eX);
			// check if line is vertically within clipping
			if (eX < clip.getLeft() or bX > clip.getRight()) return;

			// clip horizontal length
			bX = xpcc::max(bX, clip.getLeft());
			eX = xpcc::min(eX, clip.getRight());
			// draw visible and clipped line
			drawHorizontalLine(bY, bX, eX, color, composition);
			return;
		}

		{
			// code below is based directly on the paper:
			// Yevgeny P. Kuzmin. Bresenham's Line Generation Algorithm with
			// Built-in Clipping. Computer Graphics Forum, 14(5):275--280, 2005.

			int16_t wbX = clip.getLeft();
			int16_t wbY = clip.getTop();
			int16_t weX = clip.getRight();
			int16_t weY = clip.getBottom();

			int16_t xd;
			int16_t yd;
			int16_t* d0 = &xd;
			int16_t* d1 = &yd;


			// is line even visible?
			if (bX > weX or eX < wbX) return;

			int8_t stx = 1;
			int8_t sty = 1;

			// check if points are in Y order <
			if (bY < eY)
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
			if (bY < wbY)
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
			if (not setexit and bX < wbX)
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


//public:
//	void drawEllipse(int16_t xm, int16_t ym, int16_t a, int16_t b,
//					 NativeColor c)
//	{
//		uint32_t x = -a, y = 0;					// II. quadrant from bottom left to top right
//		uint32_t e2 = b, dx = (1+2*x)*e2*e2;	// error increment
//		uint32_t dy = x*x, err = dx+dy;			// error of 1.step

//		do {
//			surface.setPixel(xm-x, ym+y, c);	//   I. Quadrant
//			surface.setPixel(xm+x, ym+y, c);	//  II. Quadrant
//			surface.setPixel(xm+x, ym-y, c);	// III. Quadrant
//			surface.setPixel(xm-x, ym-y, c);	//  IV. Quadrant
//			e2 = 2*err;
//			if (e2 >= dx) { x++; err += dx += 2*int32_t(b)*b; }	// x step
//			if (e2 <= dy) { y++; err += dy += 2*int32_t(a)*a; }	// y step
//		}
//		while (x <= 0);

//		// too early stop for flat ellipses with a=1
//		while (y++ < b)
//		{
//			// -> finish tip of ellipse
//			surface.setPixel(xm, ym+y, c);
//			surface.setPixel(xm, ym-y, c);
//		}
//	}

public:
	void
	drawCircle(const Circle &circle, const AlphaColor &color, Rect clip)
	{ drawCircle(circle, color, A, clip); }

	void
	drawCircle(const Circle &circle, const AlphaColor &color, const CompositionOperator composition = A, Rect clip = Rect())
	{
		// we don't draw empty circles
		if (circle.isEmpty()) return;

		// clip the clipping to the surface
		clip = surface.clip(clip);

		// only draw intersecting circles
		if (not circle.intersects(clip)) return;

		// only draw a pixel if radius is zero
		if (circle.isNull()) {
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
			if (clip.contains(circle.getX() - x, circle.getY() + y))
				surface.compositePixel(circle.getX() - x, circle.getY() + y, color, composition);	//   I. Quadrant +x +y
			if (clip.contains(circle.getX() - y, circle.getY() - x))
				surface.compositePixel(circle.getX() - y, circle.getY() - x, color, composition);	//  II. Quadrant -x +y
			if (clip.contains(circle.getX() + x, circle.getY() - y))
				surface.compositePixel(circle.getX() + x, circle.getY() - y, color, composition);	// III. Quadrant -x -y
			if (clip.contains(circle.getX() + y, circle.getY() + x))
				surface.compositePixel(circle.getX() + y, circle.getY() + x, color, composition);	//  IV. Quadrant +x -y

			r = err;

			if (r <= y) err += ++y * 2 + 1;		// e_xy+e_y < 0

			if (r > x or err > y) {			// e_xy+e_x > 0 or no 2nd y-step
				err += ++x * 2 + 1;				// -> x-step now
			}
		}
		while (x < 0);
	}

public:
	inline void
	fillCircle(const Circle &circle, const AlphaColor &color, Rect clip)
	{ fillCircle(circle, color, A, clip); }

	void
	fillCircle(const Circle &circle, const AlphaColor &color, const CompositionOperator composition = A, Rect clip = Rect())
	{
		// we don't draw empty circles
		if (circle.isEmpty()) return;

		// clip the clipping to the surface
		clip = surface.clip(clip);

		// only draw intersecting circles
		if (not circle.intersects(clip)) return;

		// only draw a pixel if radius is zero
		if (circle.isNull()) {
			surface.compositePixel(circle.getX(), circle.getY(), color, composition);
			return;
		}

		// start the drawing
		int16_t r = circle.getRadius();
		int16_t x = -r;
		int16_t y = 1;
		int16_t err = 2 - 2 * r;
		int16_t prevY = 0;

		drawHorizontalLineClipped(circle.getY(), circle.getX() - r, circle.getX() + r, clip, color, composition);

		do
		{
			if (y != prevY)
			{
				// above origin, left to right
				drawHorizontalLineClipped(circle.getY() + y, circle.getX() + x, circle.getX() - x, clip, color, composition);
				// below left to right
				drawHorizontalLineClipped(circle.getY() - y, circle.getX() + x, circle.getX() - x, clip, color, composition);
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

public:
	inline void
	drawRect(const Rect &rectangle, const AlphaColor &color, Rect clip)
	{ drawRect(rectangle, color, A, clip); }

	void
	drawRect(const Rect &rectangle, const AlphaColor &color, const CompositionOperator composition = A, Rect clip = Rect())
	{
		clip = surface.clip(clip);
		// don't even bother if rect is not in clip area
		if (clip.isEmpty() or not clip.intersects(rectangle)) return;

//		int16_t cl = clip.getLeft();	// save some stack
//		int16_t ct = clip.getTop();		// save some stack
		int16_t cr = clip.getRight();
		int16_t cb = clip.getBottom();

//		int16_t rl = rectangle.getLeft();	// save some stack
//		int16_t rt = rectangle.getTop();	// save some stack
		int16_t rr = rectangle.getRight();
		int16_t rb = rectangle.getBottom();

		int16_t ml = xpcc::max(rectangle.getLeft(), clip.getLeft());
		int16_t mt = clip.getTop();	// decided at top line
		int16_t mr = xpcc::min(rr, cr);
		int16_t mb = cb;			// decided at bottom line

		if (clip.getTop() <= rectangle.getTop()) {	// top line
			mt = rectangle.getTop();
			drawHorizontalLine(rectangle.getTop(), ml+1, mr-1, color, composition);
		}

		if (rb <= cb) {	// bottom line
			mb = rb;
			drawHorizontalLine(rb, ml+1, mr-1, color, composition);
		}

		if (clip.getLeft() <= rectangle.getLeft()) {	// left line
			drawVerticalLine(rectangle.getLeft(), mt, mb, color, composition);
		}

		if (rr <= cr) {	// right line
			drawVerticalLine(rr, mt, mb, color, composition);
		}
	}

public:
	inline void
	fillRect(const Rect &rectangle, const AlphaColor &color, Rect clip)
	{ fillRect(rectangle, color, A, clip); }

	inline void
	fillRect(const Rect &rectangle, const AlphaColor &color, const CompositionOperator composition = A, Rect clip = Rect())
	{
		clip = rectangle.intersected(surface.clip(clip));
		// there is no need to test for isEmpty() !

		for (int16_t yy = clip.getTop(); yy <= clip.getBottom(); yy++)
		{
			drawHorizontalLine(yy, clip.getLeft(), clip.getRight(), color, composition);
		}
	}

protected:
	inline void
	drawHorizontalLineClipped(int16_t y, int16_t beginX, int16_t endX, const Rect &clip,
							  const AlphaColor &color, const CompositionOperator composition)
	{
		if (y >= clip.getTop() and y <= clip.getBottom())
		{
			// Y is in clip window
			int16_t cR = clip.getRight();
			if (beginX <= cR)
			{
				// line starts before right of clip window
				drawHorizontalLine(y, std::max(beginX, clip.getLeft()), std::min(endX, cR), color, composition);
			}
		}
	}


	inline void
	drawHorizontalLine(int16_t y, int16_t beginX, int16_t endX,
					   const AlphaColor &color, const CompositionOperator composition)
	{
		for (int16_t xx = beginX; xx <= endX; xx++)
		{
			surface.compositePixel(xx, y, color, composition);
		}
	}

	inline void
	drawVerticalLine(int16_t x, int16_t beginY, int16_t endY,
					 const AlphaColor &color, const CompositionOperator composition)
	{
		for (int16_t yy = beginY; yy <= endY; yy++)
		{
			surface.compositePixel(x, yy, color, composition);
		}
	}

private:
	Surface &surface;
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_PAINTER_HPP

