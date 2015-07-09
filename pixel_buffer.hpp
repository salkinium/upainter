#ifndef MODM_GES_PIXELBUFFER_HPP
#define MODM_GES_PIXELBUFFER_HPP

#include <cstddef>
#include <stdint.h>
#include <xpcc/architecture/utils.hpp>
#include <cstring>

namespace modm
{

namespace ges
{

class PixelBuffer
{
public:
	template< std::size_t Length >
	class Buffer
	{
		uint8_t data[Length] ATTRIBUTE_ALIGNED(4);
	public:
		Buffer() : data{0} {}

		inline const uint8_t *
		getData() const
		{ return data; }

		inline uint8_t *
		getData()
		{ return data; }

		inline std::size_t
		getLength() const
		{ return Length; }

		inline void
		clear(uint8_t value = 0)
		{ std::memset(data, value, Length); }
	};
};

template< uint8_t *const Address >
class StaticPixelBuffer
{
public:
	template< std::size_t Length >
	class Buffer
	{
	public:
		Buffer()
		{ clear(); }

		inline const uint8_t *
		getData() const
		{ return Address; }

		inline uint8_t *
		getData()
		{ return Address; }

		inline std::size_t
		getLength() const
		{ return Length; }

		inline void
		clear(uint8_t value = 0)
		{ std::memset(Address, value, Length); }
	};
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_PIXELBUFFER_HPP
