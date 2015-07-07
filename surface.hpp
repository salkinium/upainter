#ifndef MODM_GES_SURFACE_HPP
#define MODM_GES_SURFACE_HPP

#include <stdint.h>
#include "pixel_format.hpp"
#include "pixel_buffer.hpp"
#include "color.hpp"

namespace modm
{

namespace ges
{

template< uint16_t Width, uint16_t Height, PixelFormat Format = PixelFormat::ARGB8 >
class Surface;

} // namespace ges

} // namespace modm

#include "surface/surface_argb2.hpp"
#include "surface/surface_argb4.hpp"
#include "surface/surface_argb8.hpp"
#include "surface/surface_argb1555.hpp"
#include "surface/surface_l1.hpp"
#include "surface/surface_l4.hpp"
#include "surface/surface_l8.hpp"
#include "surface/surface_rgb1.hpp"
#include "surface/surface_rgb332.hpp"
#include "surface/surface_rgb565.hpp"

#endif // MODM_GES_SURFACE_HPP
