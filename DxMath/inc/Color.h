#ifndef _COLOR_H_
#define _COLOR_H_
#include "Vector.h"
#include "Engine.h"
//this class represent color with 4 bytes(red,green,blue,alpha)
class Color :public Vector
{
public://public for everyone
	Color(float r, float g, float b);
	Color(unsigned char r, unsigned char g, unsigned char b);
	Color(const Color &copy);
	~Color();
protected:
private:
};
#endif