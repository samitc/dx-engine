#include "BaseLog.h"
#include <XmlLogger.h>
CONSTEXPRN int CHACH_SIZE = 1024;
BaseLog & BaseLog::getInstance()
{
	static BaseLog baseLog;
	return baseLog;
}
void BaseLog::debug(const char * msg)
{
	log.debug(msg);
}
void BaseLog::fatal(const char * msg)
{
	log.fatal(msg);
}
void BaseLog::error(const char * msg)
{
	log.error(msg);
}
void BaseLog::info(const char * msg)
{
	log.info(msg);
}
void BaseLog::warn(const char * msg)
{
	log.warn(msg);
}
void BaseLog::trace(const char * msg)
{
	log.trace(msg);
}
Sys::Logging::XmlLogger getXml()
{
	Sys::Logging::XmlLogger xml;
	xml.load("dx.xml");
	return xml;
}
BaseLog::BaseLog() :log(getXml(), 0,Sys::Logging::ConcurrencyLevel::ONE)
{
}