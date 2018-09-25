#ifndef _RESOURCEVIEW_H_
#define _RESOURCEVIEW_H_
#include "Engine.h"
#include "DXTypes.h"
class Resource;
class ResourceView
{
public:
	static Resource getResource(ResourceViewType view);
	~ResourceView();
	ResourceView& operator=(const ResourceView&);
	ResourceViewsType getAllViews() const;
	ResourceViewType getView(int i) const;
	ResourceViewType getView() const;
	int getViewsCount() const;
protected:
	ResourceView() = default;
	ResourceView(const ResourceView&);
	ResourceView(ResourceViewType resView);
	ResourceView(ResourceViewsType resView, int viewsCount);
	void setResView(const ResourceViewsType res, int viewsCount);
	void setResView(const ResourceViewType res);
	ResourceViewsType getResourceView() const;
private:
	ResourceViewsType resourceView;
	int viewsCount;
};
#endif