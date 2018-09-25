#ifndef _INSTANCEMODEL_H_
#define _INSTANCEMODEL_H_
#include "Model.h"
#include "Buffer.h"
class InstanceModel:public Model
{
public:
	InstanceModel();
	~InstanceModel() = default;
	void setInstanceBuffer(const Buffer& instance);
protected:
	void prepareRendering(const RenderDevice &renderDevice) override;
	void draw(const RenderDevice &renderDevice, int count, int startIndex, int startVertex) override;
private:
	Buffer buf;
	int instanceCount;
	GET_SET_METHOD(instanceCount, int, public);
};
#endif