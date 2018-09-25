#include "State.h"
CONSTEXPR State::State() :hasChange(0)
{
}
void State::reset()
{
	hasChange = -1;
}
CONSTEXPR void State::setChange(int pos)
{
	hasChange |= 1 << pos;
}
CONSTEXPR bool State::getChange(int pos) const
{
	return (hasChange&(1 << pos)) != 0;
}
CONSTEXPR void State::clearChange(int pos)
{
	hasChange &= ~(1 << pos);
}
CONSTEXPR void State::clearChange()
{
	hasChange = 0;
}