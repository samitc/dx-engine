#include "UnorderedView.h"

UnorderedView & UnorderedView::operator=(UnorderedView & obj)
{
	this->ResourceView::operator=(obj);
	return *this;
}
UnorderedViewType UnorderedView::getResView() const
{
	return (UnorderedViewType)this->getResourceView();
}
UnorderedView::UnorderedView(const UnorderedViewType res)
{
	this->setResView(res);
#if CLDLEVEL >= 4
	static int number = 0;
	char name[] = "Unordered resource view:   ";
	char *p = name + strlen(name) - 1;
	int num = number++;
	while (num>0)
	{
		*p = (num % 10) + 48;
		num /= 10;
		p--;
	}
	res->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
#endif
}