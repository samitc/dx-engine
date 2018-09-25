#ifndef _STATE_H_
#define _STATE_H_
#include "Engine.h"
class DXMain;
class State//for engine
{
public:
	CONSTEXPR State();
	virtual ~State() = default;
	void reset();
	virtual void apply(const DXMain &dx) = 0;
	virtual bool hasChanged() const = 0;
protected:
	CONSTEXPR void setChange(int pos);
	CONSTEXPR bool getChange(int pos) const;
	CONSTEXPR void clearChange(int pos);
	CONSTEXPR void clearChange();
	unsigned char hasChange;
private:
};
#endif