#ifndef _GOBJECT_H_
#define _GOBJECT_H_
#include "EObject.h"
#include "Engine.h"
class RenderDevice;
class GObject :public virtual EObject
{
public:
	GObject() = default;
	virtual ~GObject() = default;
	//called before any object render
	virtual void beforeRender(const RenderDevice &dx);
	//render the object
	virtual void render(const RenderDevice &device) = 0;
	//called after all objects render
	virtual void afterRender(const RenderDevice &device);
	//set if this object should be render
	void setShouldDrawen(bool draw);
	bool getShouldDrawen() const;
};
#endif