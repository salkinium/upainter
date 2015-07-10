#ifndef MODM_GES_SURFACE_HPP
#define MODM_GES_SURFACE_HPP

#include <stdint.h>
#include "pixel_format.hpp"
#include "pixel_buffer.hpp"
#include "color.hpp"
#include <cstring>
#include <algorithm>

namespace modm
{

namespace ges
{

template< PixelFormat Format >
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
#include "surface/qsurface.hpp"

#endif // MODM_GES_SURFACE_HPP
