#pragma once
#include "GOEntity.h"
#include "GameTime.h"
#include "Buffer.h"
class GObjectMock :public GOEntity
{
public:
private:
	int *num;
	int b;
	int stati = 0;
    const RenderDevice *dev;
public:
	GObjectMock() :GObjectMock(INT_MAX, new int())
	{
		this->setIbuffer(nullptr);
		this->setVbuffer(nullptr);
	}
	GObjectMock(int a, int *num1)
	{
		num = num1;
		b = a;
		this->setIbuffer(nullptr);
		this->setVbuffer(nullptr);
	}
	~GObjectMock()
	{
		*num = 0;
	}
	void render(const RenderDevice &renderDevice) override
	{
		dev = &renderDevice;
		GOEntity::render(renderDevice);
	}
	const RenderDevice* getLastDevice()
	{
		return this->dev;
	}
private:
	GObjectMock(const GObjectMock& copy)
	{
	}
};