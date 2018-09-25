#include "AnimateModel.h"
#include <algorithm>
#include "matrix.h"
#include "vector.h"
Vector f2v(const Float4 &f)
{
	return Vector(f.x, f.y, f.z, f.w);
}
Vector f2v(const Float3 &f)
{
	return Vector(f.x, f.y, f.z);
}
AnimateModel::AnimateModelBuilder::AnimateModelBuilder(const char * fileName, const ModelBuildBuilder &modelBuild) :builder(new ModelBuilderFromResource(fileName))
{
	builder->setModelBuildBuilder(ModelBuildBuilder(modelBuild));
}
void AnimateModel::AnimateModelBuilder::build(const DXMain &dx, AnimateModel *model)
{
	builder->build(dx, model);
	model->builder = this->builder;
	this->builder = nullptr;
}
AnimateModel * AnimateModel::AnimateModelBuilder::build(const DXMain & dx)
{
	AnimateModel* ret = new AnimateModel();
	build(dx, ret);
	return ret;
}
AnimateModel::AnimateModel():builder(nullptr),tempMetrix(nullptr)
{
}
AnimateModel::~AnimateModel()
{
	delete builder;
	delete[] tempMetrix;
	delete[] treeTempMatrix;
}
const ModelData & AnimateModel::getData() const
{
	waitForData();
	return *builder->getModelData();
}
int AnimateModel::getNumOfBones() const
{
	waitForData();
	return this->builder->getModelData()->getModels()[builder->getModelBuildBuilder().getmodelIndex()].bones.size();
}
template<class T, class U, class cmp> int findInVec(const std::vector<T> &vec, const U &dat,cmp &cmp)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (cmp(vec[i],dat))
		{
			return i;
		}
	}
	return -1;
}
template<class T, class U, class cmp> int findInSortVecPre(const std::vector<T> &vec, const U &dat, cmp &cmp)
{
	int min = 0;
	int max = vec.size() - 2;
	unsigned int curPos;
	while (min <= max)
	{
		curPos = (min + max) / 2;
		int prRes = cmp(vec[curPos], vec[curPos + 1], dat);
		if (prRes == 0)
		{
			return curPos;
		}
		else if (prRes < 0)
		{
			max = curPos - 1;
		}
		else
		{
			min = curPos + 1;
		}
	}
	return -1;
}
Vector affineZero()
{
	return Vector(0, 0, 0, 1);
}
Vector intr(const Vector& qs, const Vector& qe, float factor)
{
	float cosom = qs.getX() * qe.getX() + qs.getY()*qe.getY() + qs.getZ()*qe.getZ() + qs.getW()*qe.getW();
	Vector end = cosom > 0 ? qe : -qe;
	if (cosom<0)
	{
		cosom = -cosom;
	}
	float sclp, sclq;
	if ((1.0 - cosom) > 0.0001) // 0.0001 -> some epsillon
	{
		// Standard case (slerp)
		float omega, sinom;
		omega = std::acos(cosom);
		sinom = std::sin(omega);
		sclp = std::sin((1.0f - factor) * omega) / sinom;
		sclq = std::sin(factor * omega) / sinom;
	}
	else
	{
		// Very close, do linear interp (because it's faster)
		sclp = 1.0f - factor;
		sclq = factor;
	}
	return Vector(sclp*qs.getX() + sclq*end.getX(),
		sclp*qs.getY() + sclq*end.getY(),
		sclp*qs.getZ() + sclq*end.getZ(),
		sclp*qs.getW() + sclq*end.getW());
}
Float4x4 getRot(const Vector &rot)
{
	Float4x4 ret;
	float x = rot.getX();
	float y = rot.getY();
	float z = rot.getZ();
	float w = rot.getW();
	ret.val[0][0] = 1.0f - 2.0f * (y * y + z * z);
	ret.val[0][1] = 2.0f * (x * y - z * w);
	ret.val[0][2] = 2.0f * (x * z + y * w);
	ret.val[1][0] = 2.0f * (x * y + z * w);
	ret.val[1][1] = 1.0f - 2.0f * (x * x + z * z);
	ret.val[1][2] = 2.0f * (y * z - x * w);
	ret.val[2][0] = 2.0f * (x * z - y * w);
	ret.val[2][1] = 2.0f * (y * z + x * w);
	ret.val[2][2] = 1.0f - 2.0f * (x * x + y * y);
	for (size_t i = 0; i < 4; i++)
	{
		ret.val[3][i] = 0;
		ret.val[i][3] = 0;
	}
	ret.val[3][3] = 1;
	return ret;
}
/*Matrix interpolate(const std::vector<ModelDat::Bone::Frame> &frames,float time)
{
	if (time<=frames[0].time)
	{
		return Matrix::affineTransformation(f2v(frames[0].transform), f2v(frames[0].quat), affineZero(), f2v(frames[0].scale));
	}
	else if (time>=frames[frames.size()-1].time)
	{
		return Matrix::affineTransformation(f2v(frames[frames.size() - 1].transform), f2v(frames[frames.size() - 1].quat), affineZero(), f2v(frames[frames.size() - 1].scale));
	}
	else
	{
		int index = findInVecPre(frames, time, [](const auto& preFra, const auto& Fra, const auto time)
		{
			return time >= preFra.time&&time < Fra.time;
		});
		float lerpPrec = (time - frames[index].time) / (frames[index + 1].time - frames[index].time);
		return Matrix::affineTransformation(
			Vector::lerp(f2v(frames[index].transform), f2v(frames[index + 1].transform), lerpPrec),
			Vector::lerp(f2v(frames[index].quat), f2v(frames[index + 1].quat), lerpPrec),
			affineZero(),
			Vector::lerp(f2v(frames[index].scale), f2v(frames[index + 1].scale), lerpPrec));
	}
}*/
Matrix interpolate(const std::vector<ModelDat::Bone::Frame> &frames, float time)
{
	int index = findInSortVecPre(frames, time, [](const auto& preFra, const auto& Fra, const auto time)
	{
		if (time<preFra.time)
		{
			return -1;
		}
		else if (time>Fra.time)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	});
	if (index == -1)
	{
		if (time < 0)
		{
			index = 0;
		}
		else
		{
			index = frames.size() - 1;
		}
	}
	int nextIndex = (index + 1) % frames.size();
	auto trs = f2v(frames[index].transform);
	auto nextTrs = f2v(frames[nextIndex].transform);
	auto diffTime = frames[nextIndex].time - frames[index].time;
	float factor = float((time - frames[index].time) / diffTime);
	Vector pos = diffTime > 0 ? trs + (nextTrs - trs)*factor : trs;
	auto qu = f2v(frames[index].quat);
	auto nextqu = f2v(frames[nextIndex].quat);
	Vector rot = diffTime > 0 ? intr(qu, nextqu, factor) : qu;
	Vector sc = f2v(frames[index].scale);
	Float4x4 ff = getRot(rot);
	ff.val[0][0] *= sc.getX();
	ff.val[1][0] *= sc.getX();
	ff.val[2][0] *= sc.getX();
	ff.val[0][1] *= sc.getY();
	ff.val[1][1] *= sc.getY();
	ff.val[2][1] *= sc.getY();
	ff.val[0][2] *= sc.getZ();
	ff.val[1][2] *= sc.getZ();
	ff.val[2][2] *= sc.getZ();
	ff.val[0][3] = pos.getX();
	ff.val[1][3] = pos.getY();
	ff.val[2][3] = pos.getZ();
	return f2m(ff);
}
void calculateGlobalTransform(const std::vector<ModelDat::Bone>&bones,Matrix *rettrans,Matrix *trans,Matrix *tempTree,int boneIndex)
{
	int parentBone = bones[boneIndex].parentBone;
	if (parentBone==-1)
	{
		rettrans[boneIndex] = trans[boneIndex];
		tempTree[boneIndex] = trans[boneIndex];
	}
	else
	{
		rettrans[boneIndex] = tempTree[parentBone] * trans[boneIndex];
		tempTree[boneIndex] = tempTree[parentBone] * trans[boneIndex];
	}
}
void AnimateModel::getBonesMatrix(const char * clipName, float timeInSec, Matrix *transforms) const
{
	waitForData();
	int modelIndex = builder->getModelBuildBuilder().getmodelIndex();
	auto &modelDat = *builder->getModelData();
	auto &model = modelDat.getModels()[builder->getModelBuildBuilder().getmodelIndex()];
	int clipIndex = findInVec(model.animationsNames, clipName, [](const auto& str, const auto &clip)
	{
		return strcmp(str.c_str(), clip) == 0;
	});
	if (clipIndex != -1)
	{
		auto & nodes = modelDat.getNodes();
		Matrix modelInverse = f2m((*std::find_if(nodes.cbegin(), nodes.cend(), [modelIndex](auto &node)
		{
			return node.modelIndex == modelIndex;
		})).transformation).inverse();//need to translate to global matrix
		Matrix bonesNodeMatrix = f2m((*std::find_if(nodes.cbegin(), nodes.cend(), [modelIndex](auto &node)
		{
			return node.boneModelIndex == modelIndex;
		})).transformation);//need to translate to global matrix
		auto &bones = model.bones;
		auto bonesSize = bones.size();
		if (tempMetrix == nullptr)
		{
			tempMetrix = new Matrix[bonesSize];
			treeTempMatrix = new Matrix[bonesSize];
		}
		for (size_t i = 0; i < bonesSize; i++)
		{
			if (bones[i].frames.size() > 0)
			{
				transforms[i] = interpolate(bones[i].frames[clipIndex], timeInSec);
			}
			else
			{
				throw "Not implement";
			}
		}
		for (size_t i = 0; i < bonesSize; i++)
		{
			calculateGlobalTransform(bones, tempMetrix, transforms,treeTempMatrix, i);
			tempMetrix[i] = bonesNodeMatrix*tempMetrix[i];
		}
		for (size_t i = 0; i < bonesSize; i++)
		{
			transforms[i] = tempMetrix[i];
		}
		for (size_t i = 0; i < bonesSize; i++)
		{
			transforms[i] = modelInverse*transforms[i] * f2m(bones[i].offsetMatrix);
		}
	}
}