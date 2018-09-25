#include "GameTime.h"
#include "GeneralException.h"
#include "BaseLog.h"
#define TIME_INIT_VALUE 0
void GameTime::GameStart()
{
	this->FramesR = 0;
	this->FramesU = 0;
	this->timeSinceLastRender = ClockType::time_point();
	this->timeSinceLastUpdate = ClockType::time_point();
	this->totalTimeRender = ClockType::time_point();
	this->totalTimeUpdate = ClockType::time_point();
	tGameStart = ClockType::now();
	tEndRender = tGameStart;
	tEndUpdate = tGameStart;
}
void GameTime::GameEnd()
{
	tGameEnd = ClockType::now();
}
void GameTime::StartUpdate()
{
	tStartUpdate = ClockType::now();
	timeSinceLastUpdate = ClockType::time_point(tStartUpdate - tEndUpdate.time_since_epoch());
}
void GameTime::EndUpdate()
{
	this->FramesU++;
	tEndUpdate = ClockType::now();
	totalTimeUpdate += tEndUpdate - tStartUpdate;
}
void GameTime::StartRender()
{
	tStartRender = ClockType::now();
	timeSinceLastRender = ClockType::time_point(tStartRender - tEndRender);
}
void GameTime::EndRender()
{
	this->FramesR++;
	tEndRender = ClockType::now();
	totalTimeRender += tEndRender - tStartRender;
}
unsigned __int64 GameTime::GetFrameRNum() const
{
	return FramesR;
}
unsigned __int64 GameTime::GetFrameUNum() const
{
	return FramesU;
}
void GameTime::stop()
{
	throw "Not implement";
}
void GameTime::resume()
{
	throw "Not implement";
}
long double GameTime::getSecondSinceLastUpdate() const
{
	return std::chrono::duration_cast<std::chrono::duration<long double, std::ratio<1, 1>>>(timeSinceLastUpdate.time_since_epoch()).count();
}
long double GameTime::getSecondSinceLastRender() const
{
	return std::chrono::duration_cast<std::chrono::duration<long double, std::ratio<1, 1>>>(timeSinceLastRender.time_since_epoch()).count();
}
long double GameTime::gettotalTimeUpdate() const
{
	return std::chrono::duration_cast<std::chrono::duration<long double, std::ratio<1, 1>>>(totalTimeUpdate.time_since_epoch()).count();
}
long double GameTime::gettotalTimeRender() const
{
	return std::chrono::duration_cast<std::chrono::duration<long double, std::ratio<1, 1>>>(totalTimeRender.time_since_epoch()).count();
}

long double GameTime::getTotalGameTime() const
{
	return std::chrono::duration_cast<std::chrono::duration<long double, std::ratio<1, 1>>>(ClockType::now() - tGameStart).count();
}
