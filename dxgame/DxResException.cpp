#include "DxResException.h"
#include <string>
#include "BaseLog.h"
DxResException::DxResException(bool isFatal, int line, const char * file, const char * error,HRESULT res) :DXException(isFatal), line(line), file(file), error(error)
{
	std::string strLog("dx resoult error:file=");
	strLog.append(file);
	strLog.append(",line=");
	strLog.append(std::to_string(line));
	strLog.append(",res=");
	strLog.append(std::to_string(res));
	strLog.append(",error=");
	strLog.append(error);
	if (!isFatal)
	{
		BaseLog::getInstance().error(strLog.c_str());
	}
	else
	{
		BaseLog::getInstance().fatal(strLog.c_str());
		exit(1);
	}
}