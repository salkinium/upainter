#ifndef MODM_GES_SURFACE_HPP
#define MODM_GES_SURFACE_HPP

#include <stdint.h>
#include "pixel_format.hpp"
#include "pixel_buffer.hpp"
#include "color.hpp"
#include <xpcc/architecture/utils.hpp>

namespace modm
{

namespace ges
{

template< uint16_t Width, uint16_t Height, PixelFormat Format = PixelFormat::RGBA8888 >
class Surface;

} // namespace ges

} // namespace modm

#include "surface/surface_argb2.hpp"
#include "surface/surface_argb4.hpp"
#include "surface/surface_rgba8888.hpp"
#include "surface/surface_argb1555.hpp"
#include "surface/surface_l1.hpp"
#include "surface/surface_l4.hpp"
#include "surface/surface_l8.hpp"
#include "surface/surface_rgb1.hpp"
#include "surface/surface_rgb332.hpp"
#include "surface/surface_rgb565.hpp"
#include "surface/qsurface.hpp"

#endif // MODM_GES_SURFACE_HPP
