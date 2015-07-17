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
		const Line ln = l.normalized();

		int16_t bX = ln.getX1();
		int16_t bY = ln.getY1();
		int16_t eX = ln.getX2();
		int16_t eY = ln.getY2();

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

		/*
		{
			// classic bresenham without any clipping during setup
			const int16_t dx =  abs(eX-bX);
			const int16_t dy = -abs(eY-bY);

			const int8_t sy = bY < eY ? 1 : -1;

			// error value e_xy
			int32_t err = dx + dy;
			int32_t e2;

			while (true)
			{
				// this should be solved with analytic line clipping, not bit masking
				if (clip.contains(bX, bY)) surface.setPixel(bX, bY, c);

				if (bX == eX and bY == eY) break;

				e2 = 2*err;
				if (e2 > dy) // e_xy + e_x > 0
				{
					err += dy;
					bX++;
				}
				if (e2 < dx) // e_xy + e_y < 0
				{
					err += dx;
					bY += sy;
				}
			}
		}
		/*/
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

			if (not setexit and bX < wbX)
			{
				uint32_t temp = dy2 * (wbX - bX);
				uint_fast16_t msd = temp / dx2;
				yd += msd;
				uint16_t rem = temp % dx2;

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
				surface.setPixel(*d0, *d1, c);

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
		//*/
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

