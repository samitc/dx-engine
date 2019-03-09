#include "GObjectContainer.h"
#include <algorithm>
#include "GraphicState.h"
#include "DXMain.h"
#include "Device.h"
#include "DrawCommand.h"
#include "GObject.h"
#include "UObject.h"
#include "GameTime.h"
#include "RenderDevice.h"
template <int> bool update(UObject *obj, const GameTime &time)
{
	throw "not implement";
}
template <> bool update<1>(UObject *obj, const GameTime &time)
{
	return obj->update(time);
}
template <> bool update<2>(UObject *obj, const GameTime &time)
{
	return obj->beforeUpdate(time);
}
template <> bool update<3>(UObject *obj, const GameTime &time)
{
	return obj->afterUpdate(time);
}
template <int> void render(std::pair<GObject*, DrawCommand> &obj, const RenderDevice &renderDevice)
{
	throw "not implement";
}
template <> void render<1>(std::pair<GObject*, DrawCommand> &obj, const RenderDevice &renderDevice)
{
	renderDevice.getdx().getDevice().setDrawCommand(obj.second);
	obj.first->render(renderDevice);
}
template <> void render<2>(std::pair<GObject*, DrawCommand> &obj, const RenderDevice &renderDevice)
{
	obj.first->beforeRender(renderDevice);
}
template <> void render<3>(std::pair<GObject*, DrawCommand> &obj, const RenderDevice &renderDevice)
{
	obj.first->afterRender(renderDevice);
}
template <int N> bool updateP(std::vector<UObject*> &vec, const GameTime &time)
{
	auto s = vec.begin(), e = vec.end();
	while (s!=e)
	{
		if ((*s)->getShouldUpdate())
		{
			if (!update<N>(*s, time))
			{
				s = vec.erase(s);
			}
			else
			{
				++s;
			}
		}
	}
	return true;
}
template <int N> void renderP(std::vector<std::pair<GObject*, DrawCommand>> &vec, const RenderDevice &renderDevice)
{
	for (auto &obj : vec) 
	{
		if (obj.first->getShouldDrawen())
		{
			render<N>(obj, renderDevice);
		}
	}
}
GObjectContainer::GObjectContainer()
{
}
GObjectContainer::~GObjectContainer()
{
	for (const auto& g : gObjects)
	{
		delete g.first;
	}
	for (const auto& u : uObjects)
	{
		delete u;
	}
}
GObjectContainer::GObjectContainer(GObjectContainer &&m) :gObjects(std::move(m.gObjects)), uObjects(std::move(m.uObjects))
{
}
bool GObjectContainer::update(const GameTime &time)
{
	updateP<2>(uObjects, time);
	updateP<1>(uObjects, time);
	updateP<3>(uObjects, time);
	return true;
}
void GObjectContainer::render(const RenderDevice &renderDevice)
{
	renderP<2>(gObjects, renderDevice);
	renderP<1>(gObjects, renderDevice);
	renderP<3>(gObjects, renderDevice);
}
void GObjectContainer::addObject(GObject* ob, const DrawCommand&drawCommand)
{
#if CLDLEVEL >= 4
	if (ob != NULL)
	{
		if (std::find_if(this->gObjects.begin(), this->gObjects.end(), [ob, &drawCommand](const auto& obj)
		{
			return obj.first == ob && obj.second == drawCommand;
		}) == this->gObjects.end())
		{
#endif
			gObjects.push_back(std::pair<GObject*, DrawCommand>(ob, drawCommand));
#if CLDLEVEL >= 4
		}
	}
#endif
}
void GObjectContainer::addObject(UObject * ob)
{
#if CLDLEVEL >= 4
	if (ob != NULL)
	{
		if (std::find_if(this->uObjects.begin(), this->uObjects.end(), [ob](const auto& obj)
		{
			return obj == ob;
		}) == this->uObjects.end())
		{
#endif
			uObjects.push_back(ob);
#if CLDLEVEL >= 4
	}
}
#endif
}
const std::vector<Shader*> GObjectContainer::addObjectS(GObject * ob, const DrawCommand &drawCommand)
{
#if CLDLEVEL >= 4
	if (ob != NULL)
	{
		if (std::find_if(this->gObjects.begin(), this->gObjects.end(), [ob, &drawCommand](const auto& obj)
		{
			return obj.first == ob && obj.second == drawCommand;
		}) == this->gObjects.end())
		{
#endif
			gObjects.push_back(std::pair<GObject*, DrawCommand>(ob, drawCommand));
			return gObjects[gObjects.size() - 1].second.getpipe().getshaders();
#if CLDLEVEL >= 4
		}
	}
#endif
	return std::vector<Shader*>();
}
void GObjectContainer::removeObject(GObject* ob, const DrawCommand& drawCommand)
{
	gObjects.erase(std::remove_if(gObjects.begin(), gObjects.end(), [ob, &drawCommand](auto& obj)
	{
		return ob == obj.first && drawCommand == obj.second;
	}), gObjects.cend());
}
void GObjectContainer::removeObject(UObject * ob)
{
	uObjects.erase(std::remove_if(uObjects.begin(), uObjects.end(), [ob](auto& obj)
	{
		return ob == obj;
	}), uObjects.cend());
}
GObjectContainer & GObjectContainer::operator=(GObjectContainer &&m)
{
	if (this != &m)
	{
		gObjects = std::move(m.gObjects);
		uObjects = std::move(m.uObjects);
	}
	return *this;
}