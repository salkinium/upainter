#ifndef MODM_GES_PAINTER_HPP
#define MODM_GES_PAINTER_HPP

#include "surface.hpp"
#include "pixel_color.hpp"
#include <xpcc/ui/animation/interpolation.hpp>
#include <xpcc/math/utils/misc.hpp>

#include "geometry/line.hpp"
#include "geometry/line.hpp"
#include "geometry/rect.hpp"

namespace modm
{

namespace ges
{

template< PixelFormat Format >
class Painter
{
	typedef typename Surface<Format>::NativeColor NativeColor;
public:
	typedef Surface<Format> Surface;


	Painter(Surface &surface) :
		surface(surface) {}

	void
	drawLine(const Line &l, NativeColor c, Rect clip = Rect())
	{
		clip = surface.clip(clip);

		int16_t bX = l.getX1();
		int16_t bY = l.getY1();
		int16_t eX = l.getX2();
		int16_t eY = l.getY2();

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
			drawVerticalLine(c, bX, bY, eY);
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
			drawHorizontalLine(c, bY, bX, eX);
			return;
		}

		{
			int16_t dx =  abs(eX-bX);
			int8_t sx = bX < eX ? 1 : -1;

			int16_t dy = -abs(eY-bY);
			int8_t sy = bY < eY ? 1 : -1;

			// error value e_xy
			int32_t err = dx + dy;
			int32_t e2;

			for (;;)
			{
				// this should be solved with analytic line clipping, not bit masking
				if (clip.contains(bX, bY)) surface.setPixel(bX, bY, c);

				e2 = 2*err;
				if (e2 >= dy) // e_xy + e_x > 0
				{
					if (bX == eX) break;
					err += dy;
					bX += sx;
				}
				if (e2 <= dx) // e_xy + e_y < 0
				{
					if (bY == eY) break;
					err += dx;
					bY += sy;
				}
			}
		}
	}

	void
	drawRect(const Rect &r, NativeColor c, Rect clip = Rect())
	{
		clip = surface.clip(clip);
		// don't even bother if rect is not in clip area
		if (clip.isEmpty() or not clip.intersects(r)) return;

//		int16_t cl = clip.getLeft();	// save some stack
//		int16_t ct = clip.getTop();		// save some stack
		int16_t cr = clip.getRight();
		int16_t cb = clip.getBottom();

//		int16_t rl = r.getLeft();	// save some stack
//		int16_t rt = r.getTop();	// save some stack
		int16_t rr = r.getRight();
		int16_t rb = r.getBottom();

		int16_t ml = xpcc::max(r.getLeft(), clip.getLeft());
		int16_t mt = clip.getTop();	// decided at top line
		int16_t mr = xpcc::min(rr, cr);
		int16_t mb = cb;	// decided at bottom line

		if (clip.getTop() <= r.getTop()) {	// top line
			mt = r.getTop();
			drawHorizontalLine(c, r.getTop(), ml, mr);
		}

		if (rb <= cb) {	// bottom line
			mb = rb;
			drawHorizontalLine(c, rb, ml, mr);
		}

		if (clip.getLeft() <= r.getLeft()) {	// left line
			drawVerticalLine(c, r.getLeft(), mt, mb);
		}

		if (rr <= cr) {	// right line
			drawVerticalLine(c, rr, mt, mb);
		}
	}

	void
	fillRect(const Rect &r, NativeColor c, Rect clip = Rect())
	{
		clip = r.intersected(surface.clip(clip));
		// there is no need to test for isEmpty() !

		for (int16_t yy = clip.getTop(); yy <= clip.getBottom(); yy++)
		{
			drawHorizontalLine(c, yy, clip.getLeft(), clip.getRight());
		}
	}

protected:
	void
	drawHorizontalLine(NativeColor color, int16_t y,
					   int16_t beginX, int16_t endX)
	{
		for (int16_t xx = beginX; xx <= endX; xx++)
		{
			surface.setPixel(xx, y, color);
		}
	}

	void
	drawVerticalLine(NativeColor color, int16_t x,
					 int16_t beginY, int16_t endY)
	{
		for (int16_t yy = beginY; yy <= endY; yy++)
		{
			surface.setPixel(x, yy, color);
		}
	}

private:
	Surface &surface;
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_PAINTER_HPP

