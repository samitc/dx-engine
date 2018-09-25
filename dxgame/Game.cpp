#include "Game.h"
#include <functional>
#include <algorithm>
#include "DXMain.h"
#include "RenderDevice.h"
#include "Device.h"
#include "Gametime.h"
#include "BaseLog.h"
#include "GObject.h"
#include "DrawCommand.h"
#include "GObjectContainer.h"
Game::Game(const RenderDevice &renderDevice) :renderDevice(renderDevice), startUpdateAutomatcly(true), startRenderAutomatcly(true), time(new GameTime()), gameObjects(1)
{
}
Game::Game(const Game & game) : renderDevice(game.renderDevice), time(new GameTime(*game.time)), startUpdateAutomatcly(game.startUpdateAutomatcly), startRenderAutomatcly(game.startRenderAutomatcly)
{
}
Game::~Game()
{
}
const Game::ObjectGroup Game::createGroup()
{
	int size = gameObjects.size();
	gameObjects.resize(size + 1);
	return gameObjects[size];
}
void Game::addObject(GObject* ob, const DrawCommand&drawCommand)
{
	addObject(gameObjects[0], ob, drawCommand);
}
void Game::addObject(UObject * ob)
{
	addObject(gameObjects[0], ob);
}
void Game::addObject(const Game::ObjectGroup & group, GObject * ob, const DrawCommand &drawCommand)
{
	group.addObject(ob, drawCommand);
}
void Game::addObject(const ObjectGroup & group, UObject * ob)
{
	group.addObject(ob);
}
const std::vector<Shader*> Game::addObjectS(GObject * ob, const DrawCommand &drawCommand)
{
	return gameObjects[0].addObjectS(ob, drawCommand);
}
void Game::removeObject(GObject* ob, const DrawCommand&drawCommand)
{
	removeObject(gameObjects[0], ob, drawCommand);
}
void Game::removeObject(const ObjectGroup & group, GObject * ob, const DrawCommand &drawCommand)
{
	group.removeObject(ob, drawCommand);
}
void Game::removeObject(UObject * ob)
{
	removeObject(gameObjects[0], ob);
}
void Game::removeObject(const ObjectGroup & group, UObject * ob)
{
	group.removeObject(ob);
}
void Game::startGame()
{
	time->GameStart();
}
void Game::endGame()
{
	time->GameEnd();
}
void Game::update()
{
	BaseLog::getInstance().trace("start update");
	if (startUpdateAutomatcly)
	{
		time->StartUpdate();
	}
	auto pointer = this->gameObjects.begin(), end = this->gameObjects.end();
	while (pointer != end)
	{
		if (!pointer->update(*time))
		{
			pointer = gameObjects.erase(pointer);
		}
		else
		{
			++pointer;
		}
	}
	if (startUpdateAutomatcly)
	{
		time->EndUpdate();
	}
	BaseLog::getInstance().trace("end update");
}
bool Game::render()
{
	BaseLog::getInstance().trace("start render");
	if (startRenderAutomatcly)
	{
		time->StartRender();
	}
	for (auto& obj : gameObjects)
	{
		obj.render(renderDevice);
	}
	if (startRenderAutomatcly)
	{
		time->EndRender();
	}
	BaseLog::getInstance().trace("end render");
return true;
}
unsigned __int64 Game::GetFrameNumber() const
{
	return time->GetFrameRNum();
}
long double Game::getSecondSinceLastUpdate() const
{
	return this->time->getSecondSinceLastUpdate();
}
long double Game::getSecondSinceLastRender() const
{
	return this->time->getSecondSinceLastRender();
}
void Game::startUpdateGame()
{
	if (!this->startUpdateAutomatcly)
	{
		this->time->StartUpdate();
	}
}
void Game::endUpdateGame()
{
	if (!this->startUpdateAutomatcly)
	{
		this->time->EndUpdate();
	}
}
void Game::setSholdStartUpdateManually(bool isManually)
{
	this->startUpdateAutomatcly = !isManually;
}
void Game::startRenderGame()
{
	if (!this->startRenderAutomatcly)
	{
		this->time->StartRender();
	}
}
void Game::endRenderGame()
{
	if (!this->startRenderAutomatcly)
	{
		this->time->EndRender();
	}
}
void Game::setSholdStartRenderManually(bool isManually)
{
	this->startRenderAutomatcly = !isManually;
}