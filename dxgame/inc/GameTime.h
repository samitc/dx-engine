#ifndef _GAMETIME_H_
#define _GAMETIME_H_
#include <chrono>
#include "Engine.h"
class GameTime
{
public:
	GameTime() = default;
	//start the game
	void GameStart();
	//end the game
	void GameEnd();
	//start update frame
	void StartUpdate();
	//stop update frame
	void EndUpdate();
	//start render frame
	void StartRender();
	//stop render frame
	void EndRender();
	//get the number of renders frame
	unsigned __int64 GetFrameRNum() const;
	//get the number of updates frame
	unsigned __int64 GetFrameUNum() const;
	//stop the game
	void stop();
	//continue the game
	void resume();
	//get the number of seconds since the last update
	long double getSecondSinceLastUpdate() const;
	//get the number of seconds since the last render
	long double getSecondSinceLastRender() const;
	//get the total time the engine was busy in updating the game
	long double gettotalTimeUpdate() const;
	//get the total time the engine was busy in rendering the game
	long double gettotalTimeRender() const;
	//get the total time that the game is running
	long double getTotalGameTime() const;
protected:
	using ClockType = std::chrono::high_resolution_clock;
	ClockType::time_point tStartRender;
	ClockType::time_point tEndRender;
	ClockType::time_point tStartUpdate;
	ClockType::time_point tEndUpdate;
	ClockType::time_point timeSinceLastUpdate;
	ClockType::time_point timeSinceLastRender;
	unsigned __int64 FramesR, FramesU;
	ClockType::time_point totalTimeUpdate;
	ClockType::time_point totalTimeRender;
	ClockType::time_point tGameStart;
	ClockType::time_point tGameEnd;
};
#endif