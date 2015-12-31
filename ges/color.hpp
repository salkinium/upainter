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

constexpr Color kColorBlack  {   0,    0,    0};
constexpr Color kColorGray   {0x80, 0x80, 0x80};
constexpr Color kColorSilver {0xc0, 0xc0, 0xc0};
constexpr Color kColorWhite  {0xff, 0xff, 0xff};

constexpr Color kColorMaroon {0x80,    0,    0};
constexpr Color kColorGreen  {   0, 0x80,    0};
constexpr Color kColorNavy   {   0,    0, 0x80};
constexpr Color kColorOlive  {0x80, 0x80,    0};
constexpr Color kColorTeal   {   0, 0x80, 0x80};
constexpr Color kColorPurple {0x80,    0, 0x80};

constexpr Color kColorRed    {0xff,    0,    0};
constexpr Color kColorLime   {   0, 0xff,    0};
constexpr Color kColorBlue   {   0,    0, 0xff};
constexpr Color kColorYellow {0xff, 0xff,    0};
constexpr Color kColorAqua   {   0, 0xff, 0xff};
constexpr Color kColorFuchsia{0xff,    0, 0xff};

// HINT: You may define your own custom colors for your project
// in the `modm::ges` namespace and they will behave the same.

} // namespace ges

} // namespace modm

#endif // MODM_GES_COLOR_HPP

