#include "color.hpp"

namespace modm
{

namespace ges
{

constexpr Color Color::Black   = Color(0, 0, 0);
constexpr Color Color::White   = Color(0xff, 0xff, 0xff);

constexpr Color Color::Red     = Color(0xff, 0, 0);
constexpr Color Color::Green   = Color(0, 0xff, 0);
constexpr Color Color::Blue    = Color(0, 0, 0xff);

constexpr Color Color::Yellow  = Color(0xff, 0xff, 0);
constexpr Color Color::Cyan    = Color(0, 0xff, 0xff);
constexpr Color Color::Magenta = Color(0xff, 0, 0xff);

// ...

} // namespace ges

} // namespace modm
