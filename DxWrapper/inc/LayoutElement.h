#ifndef _BUFFERELEMENT_H_
#define _BUFFERELEMENT_H_
#include "Engine.h"
enum class DataFormat;
//this class represent one element for input layout
class LayoutElement
{
public://public for everyone
	//create layout element
	//parameters:
	//	semanticName:the semantic name in the shader
	//	format:the format type of the element
	//	index:the index of the element in the specific semantic
	//	inputSlot:the shader parameter index
	LayoutElement(const char * const semanticName, DataFormat format, int index, int bufferIndex = 0, bool isInstanceData = false, int offset = -1, int drawPerInstance = 1);
	LayoutElement(const LayoutElement &copy);
	~LayoutElement();
	LayoutElement &operator=(LayoutElement &ass);
public://public for engine
	int getSize() const;
	int getIndex() const;
	const char * const getSemanticName() const;
	DataFormat getFormat() const;
	bool getIsInstanceData() const;
	int getOffset() const;
	int getBufferIndex() const;
	int getDrawPerInstance() const;
private:
	char *semanticName;
	DataFormat format;
	int index;
	bool instanceData;
	int offset;
	int bufferIndex;
	int drawPerInstance;
};
#endif