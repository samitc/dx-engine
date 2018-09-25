#ifndef _VIDEOMODE_H_
#define _VIDEOMODE_H_
#include "Engine.h"
#include "DXTypes.h"
enum class DataFormat;
//this class represent the mode for specific output
class VideoMode
{
public://public for everyone
	VideoMode(const VideoMode &copy);
	VideoMode(DataFormat format, UINT RefreshRateNumerator = 60, UINT RefreshRateDenominator = 1);
	VideoMode(UINT width, UINT height, DataFormat format, UINT RefreshRateNumerator = 60, UINT RefreshRateDenominator = 1);
	DataFormat getFormat() const;
	UINT getWidth() const;
	UINT getHeight() const;
	UINT getNumerator() const;
	UINT getDenominator() const;
public://public for engine
	VideoMode(const VideoModeType adapter);
protected:	
private:
	CONSTEXPR UINT defaultWidth = -1;
	CONSTEXPR UINT defaultHeight = -1;
	static CONSTEXPR VideoModeType getVideoModeType(UINT width, UINT height, DataFormat format, UINT RefreshRateNumerator, UINT RefreshRateDenominator);
	VideoModeType adapter;
};
#endif