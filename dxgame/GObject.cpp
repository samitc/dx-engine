#include "GObject.h"
#define RENDER_BIT	0
bool GObject::getShouldDrawen() const
{
	return getBit(RENDER_BIT);
}
void GObject::setShouldDrawen(bool draw)
{
	if (draw)
		setBit(RENDER_BIT);
	else
		clearBit(RENDER_BIT);
}
void GObject::beforeRender(const RenderDevice & dx)
{
}
void GObject::afterRender(const RenderDevice & dx)
{
}