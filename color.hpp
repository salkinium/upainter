/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MODM_GES_COLOR_HPP
#define MODM_GES_COLOR_HPP

#include "pixel_color.hpp"


namespace modm
{

namespace ges
{

using Color = ColorARGB8;

// These are the 16 named colors from HTML 3.2

constexpr Color ColorBlack  {   0,    0,    0};
constexpr Color ColorGray   {0x80, 0x80, 0x80};
constexpr Color ColorSilver {0xc0, 0xc0, 0xc0};
constexpr Color ColorWhite  {0xff, 0xff, 0xff};

constexpr Color ColorMaroon {0x80,    0,    0};
constexpr Color ColorGreen  {   0, 0x80,    0};
constexpr Color ColorNavy   {   0,    0, 0x80};
constexpr Color ColorOlive  {0x80, 0x80,    0};
constexpr Color ColorTeal   {   0, 0x80, 0x80};
constexpr Color ColorPurple {0x80,    0, 0x80};

constexpr Color ColorRed    {0xff,    0,    0};
constexpr Color ColorLime   {   0, 0xff,    0};
constexpr Color ColorBlue   {   0,    0, 0xff};
constexpr Color ColorYellow {0xff, 0xff,    0};
constexpr Color ColorAqua   {   0, 0xff, 0xff};
constexpr Color ColorFuchsia{0xff,    0, 0xff};

// HINT: You may define your own custom colors for your project
// in the `modm::ges` namespace and they will behave the same.

} // namespace ges

} // namespace modm

#endif // MODM_GES_COLOR_HPP

