#ifndef _VIDEOADAPTER_H_
#define _VIDEOADAPTER_H_
#include <vector>
#include "Engine.h"
#include "DXTypes.h"
#include "DxEnums.h"
class VideoOutput;
//this class represent the available gpu interface
class VideoAdapter
{
public://public for everyone
	VideoAdapter(const VideoAdapter &copy);
	~VideoAdapter();
	WCHAR* getDescription() const;
	std::vector<VideoOutput> getAllOutput() const;
	//get all the available gpu interface in the system
	static std::vector<VideoAdapter> getAdapters();
public://public for engine
	VideoAdapter(VideoAdapterType adapter);
	const VideoAdapterType getAdapter() const;
protected:	

private:
	VideoAdapter& operator=(VideoAdapter& obj) = delete;
	VideoAdapterType adapter;
};
#endif