#include "ResourceView.h"
#include "Resource.h"
#define viewsIter(command)\
for (int i = 0; i < viewsCount; i++)\
{\
	getView(i)->##command;\
}
Resource ResourceView::getResource(ResourceViewType view)
{
	ResourceType res;
	view->GetResource(&res);
	return Resource(res);
}
ResourceView::~ResourceView()
{
	viewsIter(Release());
}
ResourceView & ResourceView::operator=(const ResourceView &view)
{
	if (this!=&view)
	{
		viewsIter(Release());
		resourceView = view.resourceView;
		viewsCount = view.viewsCount;
		viewsIter(AddRef());
	}
	return *this;
}
ResourceViewsType ResourceView::getAllViews() const
{
	return getResourceView();
}
ResourceViewType ResourceView::getView(int i) const
{
	if (viewsCount==1)
	{
		return (ResourceViewType)getResourceView();
	}
	return getAllViews()[i];
}
ResourceViewType ResourceView::getView() const
{
	return getView(0);
}
int ResourceView::getViewsCount() const
{
	return viewsCount;
}
ResourceView::ResourceView(const ResourceView &copy) :ResourceView(copy.resourceView, copy.viewsCount)
{
	viewsIter(AddRef());
}
ResourceView::ResourceView(ResourceViewType resView) : ResourceView((ResourceViewsType)(resView), 1)
{
}
ResourceView::ResourceView(ResourceViewsType resView, int viewsCount) : resourceView(resView), viewsCount(viewsCount)
{
}
void ResourceView::setResView(const ResourceViewsType res, int viewsCount)
{
	this->resourceView = res;
	this->viewsCount = viewsCount;
}

void ResourceView::setResView(const ResourceViewType res)
{
	setResView((ResourceViewsType)res, 1);
}
ResourceViewsType ResourceView::getResourceView() const
{
	return resourceView;
}
