#pragma once
#include "UObject.h"
#include "GameTime.h"
#include "Buffer.h"
class GUbjectMock :public UObject
{
public:
private:
	int *num;
	int b;
	int stati = 0;
	GameTime time;
public:
    GUbjectMock() :GUbjectMock(INT_MAX, new int())
	{
	}
    GUbjectMock(int a, int *num1)
	{
		num = num1;
		b = a;
	}
	~GUbjectMock()
	{
		*num = 0;
	}
	bool update(const GameTime &time) override
	{
		this->time = time;
		if (++stati >= b)
		{
			return false;
		}
		return true;
	}
	GameTime getLastGameTime()
	{
		return this->time;
	}
};