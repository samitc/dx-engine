#ifndef _VIDEOOUTPUT_H_
#define _VIDEOOUTPUT_H_
#include <vector>
#include "Engine.h"
#include "DXTypes.h"
enum class DataFormat;
class VideoMode;
class VideoAdapter;
//this class represent all the available output for specific adapter
class VideoOutput
{
public://public for everyone
	VideoOutput(const VideoOutput &copy);
	~VideoOutput();
	std::vector<DataFormat> getAllSupportedFormat() const;
	std::vector<VideoMode> getVideosMode(DataFormat format)  const;
public://public for engine
	VideoOutput(VideoOutputType adapter);
protected:
private:
	VideoOutput& operator=(VideoOutput& obj) = delete;
	VideoOutputType adapter;
};
#endif