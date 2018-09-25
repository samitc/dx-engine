#ifndef _UObject_H_
#define _UObject_H_
#include "EObject.h"
#include "Engine.h"
class GameTime;
class UObject:public virtual EObject
{
public:
	UObject() = default;
	virtual ~UObject() = default;
	//called before any object update
	virtual bool beforeUpdate(const GameTime &time);
	//update the object
	virtual bool update(const GameTime &time) = 0;
	//called after all objects update
	virtual bool afterUpdate(const GameTime &time);
	//set if this object should be update
	void setShouldUpdate(bool update);
	bool getShouldUpdate() const;
};
#endif