#ifndef _DXEXCEPTION_H_
#define _DXEXCEPTION_H_
//represent exception of that occurs in the engine
class DXException
{
public:
	DXException() = default;
	DXException(bool isFatal);
	virtual ~DXException() = default;
private:
	bool isFatal;
};
#endif