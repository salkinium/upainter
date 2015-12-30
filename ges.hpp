/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MODM_GES_HPP
#define MODM_GES_HPP

#include <xpcc/utils/arithmetic_traits.hpp>
#include <xpcc/math/geometry/vector.hpp>
#include <fixed_point.hpp>

namespace modm
{

namespace ges
{

//using coord_t = int16_t;			// for      aliased lines
using coord_t =  modm::fix16_t<4>;	// for anti-aliased lines

using wide_coord_t = typename xpcc::ArithmeticTraits<coord_t>::WideType;
using Vector  = xpcc::Vector<coord_t, 2>;

}

}

#endif // MODM_GES_HPP

