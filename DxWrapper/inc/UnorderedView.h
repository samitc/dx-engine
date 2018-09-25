#ifndef _UNORDEREDVIEW_H_
#define _UNORDEREDVIEW_H_
#include "ResourceView.h"
class UnorderedView :public ResourceView
{
public://public for everyone
	UnorderedView()=delete;
	UnorderedView& operator=(UnorderedView& obj);
	~UnorderedView() = default;
	UnorderedViewType getResView() const;
public://public for engine
	UnorderedView(const UnorderedViewType res);
};
#endif