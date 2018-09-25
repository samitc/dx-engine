#ifndef _GOENTITY_H_
#define _GOENTITY_H_
#include "GObject.h"
#include <memory>
#include "Engine.h"
#include "DXTypes.h"
class Buffer;
class Device;
//this class represent basic drawable engine object
class GOEntity :public GObject
{
public://public for everyone
	GOEntity(const Buffer& vertexBuffer, const Buffer& indexBuffer = *(Buffer*)NULL);
	virtual ~GOEntity();
	virtual void render(const RenderDevice &renderDevice);
public://public for engine
	unsigned int getBuffers(BufferType ** buffers, unsigned int ** size) const;
	GOEntity& operator=(GOEntity& obj);
	const Buffer * const getiBuffer() const;
	const Buffer * const getvBuffer() const;
protected://for engine
	void setVbuffer(Buffer *buf);
	void setIbuffer(Buffer *buf);
protected:
	GOEntity();
	const Buffer &getVbuffer() const;
	const Buffer &getIbuffer() const;
	virtual void prepareRendering(const RenderDevice &renderDevice);
	virtual void draw(const RenderDevice &renderDevice, int count, int startIndex, int startVertex);
private:
	Buffer* setIndexBufferCtor(const Buffer& indexBuffer);
	std::unique_ptr<Buffer> Vbuffer;
	std::unique_ptr<Buffer> Ibuffer;
};
#endif