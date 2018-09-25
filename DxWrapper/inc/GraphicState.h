#ifndef _GRAPHICSTATE_H_
#define _GRAPHICSTATE_H_
#include "Engine.h"
class DXMain;
//this class represent how the gpu will draw objects
class GraphicState
{
public:
	virtual ~GraphicState();
	virtual GraphicState *clone() const = 0;
	virtual void apply(const DXMain &dx) const = 0;
	//for engine
	virtual int getId() const = 0;
private:

};
#endif