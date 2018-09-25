#include "Color.h"
Color::Color(float r, float g, float b) :Vector(r, g, b)
{
}
Color::Color(unsigned char r, unsigned char g, unsigned char b):Color(r / 255.0f, g / 255.0f, b / 255.0f)
{
}
Color::Color(const Color &copy): Vector(copy)
{
}
Color::~Color()
{
}