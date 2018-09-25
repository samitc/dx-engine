#pragma once
#include "GameTime.h"
class GameTimeMock:public GameTime
{
public:
	using TestClock = ClockType;
	GameTimeMock(const GameTime& time) :GameTime(time)
	{
	}
	auto getStartTime()
	{
		return this->tGameStart;
	}
	auto getEndTime()
	{
		return this->tGameEnd;
	}
private:

};