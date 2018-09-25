#ifndef _GAME_H_
#define _GAME_H_
#include "Engine.h"
#include <vector>
#include <utility>
#include <memory>
class RenderDevice;
class GObject;
class UObject;
class GameTime;
class GObjectContainer;
class DrawCommand;
class Shader;//TODO: remove when delete spesific shader func
class Game
{
public:
	using ObjectGroup = GObjectContainer&;
	Game(const RenderDevice& renderDevice);
	Game(const Game& game);
	virtual ~Game();
	const ObjectGroup createGroup();
	//add game object to the game
	//parameters:
	//	ob:the game object to add
	void addObject(GObject* ob, const DrawCommand&);
	void addObject(UObject* ob);
	void addObject(const ObjectGroup& group, GObject* ob, const DrawCommand&);
	void addObject(const ObjectGroup& group, UObject* ob);
	const std::vector<Shader*> addObjectS(GObject* ob, const DrawCommand&);//TODO: remove when user can get effect
	//remove game object from the game
	//parameters:
	//	ob:the game object to remove
	void removeObject(GObject* ob, const DrawCommand&);
	void removeObject(const ObjectGroup& group, GObject* ob, const DrawCommand&);
	void removeObject(UObject* ob);
	void removeObject(const ObjectGroup& group, UObject* ob);
	//start the game
	void startGame();
	//finish the game
	void endGame();
	//return the number of frames
	unsigned __int64 GetFrameNumber() const;
	//render the whole game
	bool render();
	//start frame
	void startRenderGame();
	//end frame
	void endRenderGame();
	//determinate if start new frame automatically
	//parameters:
	//	isManually:false if start manually
	void setSholdStartRenderManually(bool isManually);
	//update the whole game
	void update();
	//start frame
	void startUpdateGame();
	//end frame
	void endUpdateGame();
	//determinate if start new frame automatically
	//parameters:
	//	isManually:false if start manually
	void setSholdStartUpdateManually(bool isManually);
	//return the number of second since the last update
	long double getSecondSinceLastUpdate() const;
	//return the number of second since the last render
	long double getSecondSinceLastRender() const;
private:
	Game& operator=(const Game& g) = delete;
	Game();
	std::vector<GObjectContainer> gameObjects;
	std::unique_ptr<GameTime> time;
	const RenderDevice& renderDevice;
	bool startUpdateAutomatcly;
	bool startRenderAutomatcly;
};
#endif