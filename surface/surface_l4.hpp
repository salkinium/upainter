
#ifndef MODM_GES_SURFACE_HPP
#	error	"Don't include this file directly, use 'surface.hpp' instead!"
#endif

#include <xpcc/architecture/detect.hpp>

namespace modm
{

namespace ges
{

#ifndef XPCC__OS_HOSTED

template<>
class Surface<PixelFormat::L4>
{
	friend class QDisplay;
	static constexpr PixelFormat Format = PixelFormat::L4;
public:
	using NativeColor = PixelColor<PixelFormat::L4>;

	template< uint16_t Width, uint16_t Height >
	using Buffer = PixelBuffer<Width, Height, Format>;

public:
	Surface(uint8_t *const buffer, const uint16_t width, const uint16_t height) :
		width(width), height(height), buffer(buffer)
	{}

	Surface(uint8_t *const buffer, const Size size) :
		Surface(buffer, size.getWidth(), size.getHeight())
	{}

	template< uint16_t Width, uint16_t Height >
	Surface(PixelBuffer<Width, Height, Format> &buffer) :
		Surface(buffer.getData(), Width, Height)
	{}

	uint16_t
	getWidth() const
	{ return width; }

	uint16_t
	getHeight() const
	{ return height; }

	Size
	getSize() const
	{ return Size(width, height); }

	Rect
	getBounds() const
	{ return Rect(0,0, width-1, height-1); }

	static constexpr PixelFormat
	getPixelFormat()
	{ return Format; }

	Rect
	clip(Rect input = Rect()) const
	{
		if (input.isEmpty()) return getBounds();
		return input.intersected(getBounds());
	}

	void
	clear()
	{
		std::memset(buffer, 0, std::size_t(width) * height / 2);
	}

	void
	clear(NativeColor color)
	{
		std::memset(buffer, color.getValue() * 0x11, std::size_t(width) * height / 2);
	}

	void
	setPixel(uint16_t x, uint16_t y, NativeColor color)
	{
		if (x & 0x01) {
			buffer[(y * width + x) / 2] = (buffer[(y * width + x) / 2] & ~0xf0) | (color.getValue() << 4);
		} else {
			buffer[(y * width + x) / 2] = (buffer[(y * width + x) / 2] & ~0x0f) | color.getValue();
		}
	}

	inline void
	setPixel(Point p, NativeColor color)
	{
		setPixel(p.getX(), p.getY(), color);
	}

	void
	clearPixel(uint16_t x, uint16_t y)
	{
		setPixel(x, y, NativeColor(0));
	}

	inline void
	clearPixel(Point p)
	{
		setPixel(p.getX(), p.getY(), NativeColor(0));
	}

	NativeColor
	getPixel(uint16_t x, uint16_t y) const
	{
		if (x < width and y < height)
		{
			if (x & 0x01) {
				return NativeColor((buffer[(y * width + x) / 2] & 0xf0) >> 4);
			} else {
				return NativeColor( buffer[(y * width + x) / 2] & 0x0f);
			}
		}
		return NativeColor(0);
	}

	NativeColor
	getPixel(Point p) const
	{
		return getPixel(p.getX(), p.getY());
	}

protected:
	const uint16_t width;
	const uint16_t height;
	uint8_t *const buffer;

	template < PixelFormat F >
	friend class Painter;
};

#endif

} // namespace ges

} // namespace modm

