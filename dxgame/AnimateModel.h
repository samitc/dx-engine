#ifndef _ANIMATEMODEL_H_
#define _ANIMATEMODEL_H_
#include "Model.h"
class Matrix;
class AnimateModel:public Model
{
public:
	class AnimateModelBuilder
	{
	public:
		AnimateModelBuilder(const char* fileName, const ModelBuildBuilder &modelBuild);
		~AnimateModelBuilder() = default;
		void build(const DXMain &, AnimateModel *);
		AnimateModel *build(const DXMain &dx);
	private:
		ModelBuilderFromResource *builder;
	};
	AnimateModel();
	~AnimateModel();
	//need to change to more elegant way
	const ModelData& getData() const;
	//need to change to more elegant way
	int getNumOfBones() const;
	void getBonesMatrix(const char* clipName, float timeInSec, Matrix *transforms) const;
private:
	ModelBuilderFromResource *builder;
	mutable Matrix *tempMetrix;
	mutable Matrix *treeTempMatrix;
};
#endif