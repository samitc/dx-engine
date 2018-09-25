#include "VideoMode.h"
VideoMode::VideoMode(const VideoModeType adapter):adapter(adapter)
{
}
VideoModeType VideoMode::getVideoModeType(UINT width, UINT height, DataFormat format, UINT RefreshRateNumerator, UINT RefreshRateDenominator)
{
	VideoModeType ret;
	ret.Format = (DXGI_FORMAT)((int)(format));
	ret.Height = height;
	ret.Width = width;
	ret.RefreshRate.Numerator = RefreshRateNumerator;
	ret.RefreshRate.Denominator = RefreshRateDenominator;
	return ret;
}
VideoMode::VideoMode(const VideoMode &copy):adapter(copy.adapter)
{
}
VideoMode::VideoMode(DataFormat format, UINT RefreshRateNumerator, UINT RefreshRateDenominator) : VideoMode(defaultWidth, defaultHeight, format, RefreshRateNumerator, RefreshRateDenominator)
{
}
VideoMode::VideoMode(UINT width, UINT height, DataFormat format, UINT RefreshRateNumerator, UINT RefreshRateDenominator) :adapter(getVideoModeType(width, height, format, RefreshRateNumerator, RefreshRateDenominator))
{
}
UINT VideoMode::getWidth() const
{
	return this->adapter.Width;
}
UINT VideoMode::getHeight() const
{
	return this->adapter.Height;
}
UINT VideoMode::getNumerator() const
{
	return this->adapter.RefreshRate.Numerator;
}
UINT VideoMode::getDenominator() const
{
	return this->adapter.RefreshRate.Denominator;
}
DataFormat VideoMode::getFormat() const
{
	return (DataFormat)((int)this->adapter.Format);
}