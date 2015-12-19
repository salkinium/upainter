
#ifndef MODM_GES_SURFACE_HPP
#	error	"Don't include this file directly, use 'surface.hpp' instead!"
#endif

namespace modm
{

namespace ges
{

template<>
class Surface<PixelFormat::L1>
{
	friend class QDisplay;
	static constexpr PixelFormat Format = PixelFormat::L1;
public:
	using NativeColor = PixelColor<PixelFormat::L1>;

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
		std::memset(buffer, 0, std::size_t(width) * height / 8);
	}

	void
	clear(NativeColor color)
	{
		std::memset(buffer, color.getValue() * 0xff, std::size_t(width) * height / 8);
	}

	inline void
	setPixel(uint16_t x, uint16_t y, NativeColor color)
	{
		if (color.getValue() == 0) {
			buffer[(y * width + x) / 8] &= ~(1 << (x & 0x07));
		} else {
			buffer[(y * width + x) / 8] |=  (1 << (x & 0x07));
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
		setPixel(p, NativeColor(0));
	}

	inline NativeColor
	getPixel(uint16_t x, uint16_t y) const
	{
		if (buffer[(y * width + x) / 8] & (1 << (x & 0x07)))
			return NativeColor(1);
		return NativeColor(0);
	}

	inline NativeColor
	getPixel(Point p) const
	{
		return getPixel(p.getX(), p.getY());
	}

	template <PixelFormat CompositeFormat>
	inline void
	compositePixel(uint16_t x, uint16_t y, const PixelColor<CompositeFormat> color, void (NativeColor::*composition)(const PixelColor<CompositeFormat>) = &NativeColor::A)
	{
		NativeColor pixel(getPixel(x, y));
		(pixel.*composition)(color);
		setPixel(x, y, pixel);
	}

	template <PixelFormat CompositeFormat>
	inline void
	compositePixel(const Point p, const PixelColor<CompositeFormat> color, void (NativeColor::*composition)(const PixelColor<CompositeFormat>) = &NativeColor::A)
	{
		NativeColor pixel(getPixel(p));
		(pixel.*composition)(color);
		setPixel(p, pixel);
	}

protected:
	const uint16_t width;
	const uint16_t height;
	uint8_t *const buffer;

	template < PixelFormat F >
	friend class Painter;
};

} // namespace ges

} // namespace modm

