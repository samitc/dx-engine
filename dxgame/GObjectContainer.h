#ifndef _GOBJECTCONTAINER_H_
#define _GOBJECTCONTAINER_H_
#include <memory>
#include <vector>
#include "Engine.h"
class DrawCommand;
class GameTime;
class GObject;
class UObject;
class RenderDevice;
class Shader;//TODO: remove when delete spesific shader func
enum PrimitiveTopology;
class GObjectContainer//for engine
{
public://public for everyone
	GObjectContainer();
	~GObjectContainer();
	GObjectContainer(GObjectContainer&&);
	bool update(const GameTime &time);
	void render(const RenderDevice &renderDevice);
	void addObject(GObject* ob, const DrawCommand&);
	void addObject(UObject* ob);
	const std::vector<Shader*> addObjectS(GObject* ob, const DrawCommand&);//TODO: remove when user can get effect
	void removeObject(GObject* ob, const DrawCommand&);
	void removeObject(UObject* ob);
	GObjectContainer& operator=(GObjectContainer&&);
private:
	std::vector<std::pair<GObject*, DrawCommand>> gObjects;
	std::vector<UObject*> uObjects;
};
#endif