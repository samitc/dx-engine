#include "ModelData.h"
#include <algorithm>
#include <iterator>
#include "inc/SubSet.h"
template<class T> void writeData(char *& p, const T& t)
{
	memcpy(p, &t, sizeof(t));
	p += sizeof(t);
}
template<class T> void writeSize(char *& p, const std::vector<T>& t)
{
	*(ModelData::DATA_COUNT*)(p) = (ModelData::DATA_COUNT)t.size();
	p += sizeof ModelData::DATA_COUNT;
}
void writeStr(char*& p, const std::string &str)
{
	strcpy(p, str.c_str());
	p += str.size();
	++p;
}
template < class T, class S> inline void writeVec(char* &p, const std::vector<T> &vec, S defaultVal, std::false_type)
{
	ModelData::DATA_COUNT si = vec.size();
	writeSize(p, vec);
	if (si > 0)
	{
		memcpy(p, &vec[0], si*sizeof T);
		p += si*sizeof T;
	}
}
template < class T, class S> inline void writeVec(char* &p, const std::vector<T> &vec, S defaultVal, std::true_type)
{
	ModelData::DATA_COUNT si = vec.size();
	writeSize(p, vec);
	ModelData::DATA_COUNT countUntilDefault;
	countUntilDefault = 0;
	if (si > 0)
	{
		S* pd = (S*)&vec[0];
		while (countUntilDefault < sizeof T / sizeof S&&*pd != defaultVal)
		{
			++countUntilDefault;
			++pd;
		}
	}
	*(ModelData::DATA_COUNT*)(p) = countUntilDefault;
	p += sizeof ModelData::DATA_COUNT;
	if (si > 0)
	{
		if (countUntilDefault == sizeof T / sizeof S)
		{
			memcpy(p, &vec[0], si*sizeof T);
			p += si*sizeof T;
		}
		else
		{
			const T* cur = &vec[0];
			S* curD = (S*)p;
			for (size_t j = 0; j < si; j++)
			{
				S* curV = (S*)cur;
				for (size_t i = 0; i < countUntilDefault; i++)
				{
					*curD = *curV;
					++curD;
					++curV;
				}
				++cur;
			}
			p += si*countUntilDefault*sizeof(S);
		}
	}
}
template<class T> void readData(char *& p, T& t)
{
	memcpy(&t, p, sizeof(t));
	p += sizeof(t);
}
template<class T> void readSize(char *& p, std::vector<T>& t)
{
	ModelData::DATA_COUNT l = *(ModelData::DATA_COUNT*)(p);
	p += sizeof ModelData::DATA_COUNT;
	if (l>0)
	{
		t.resize(l);
	}
}
void readStr(char*& p, std::string &str)
{
	str = p;
	p += str.size() + 1;
}
template < class T, class S> inline void readVec(char* &p, std::vector<T> &vec, S defaultVal, std::false_type)
{
	readSize(p, vec);
	int l = vec.size();
	if (l > 0)
	{
		S* curVal = (S*)p;
		memcpy(&vec[0], p, l*sizeof T);
		p += l*sizeof T;
	}
}
template < class T, class S> inline void readVec(char* &p, std::vector<T> &vec, S defaultVal, std::true_type)
{
	readSize(p, vec);
	ModelData::DATA_COUNT defCount;
	defCount = *(ModelData::DATA_COUNT*)(p);
	p += sizeof ModelData::DATA_COUNT;
	unsigned int l = vec.size();
	if (l > 0)
	{
		S* curVal = (S*)p;
		if (defCount == sizeof T / sizeof S)
		{
			memcpy(&vec[0], p, l*sizeof T);
			p += l*sizeof T;
		}
		else
		{
			S* curInde = (S*)&vec[0];
			for (size_t i = 0; i < l; i++)
			{
				for (size_t j = 0; j < defCount; j++)
				{
					*curInde = *curVal;
					++curVal;
					++curInde;
				}
				for (size_t j = 0; j < sizeof T / sizeof S - defCount; j++)
				{
					*curInde = defaultVal;
					++curInde;
				}
			}
			p += l*defCount*sizeof(S);
		}
	}
}
#define CONST_DEF const
#include "ModelDataHelper.h"
#define writeVec readVec
#define writeData readData
#define writeSize readSize
#define writeStr readStr
#undef CONST_DEF
#include "ModelDataHelper.h"
#undef writeVec
#undef writeData
#undef writeSize
#undef writeStr
const std::vector<ModelDat>& ModelData::getModels() const
{
	return models;
}
const std::vector<Node>& ModelData::getNodes() const
{
	return nodes;
}
void ModelData::setNodes(std::vector<Node> &&n)
{
	nodes = std::move(n);
}
void ModelData::setModels(std::vector<ModelDat> &&m)
{
	models = std::move(m);
}
void ModelData::writeToBuffer(void *p) const
{
	char *c = (char*)p;
	writeVec(c, nodes);
	writeVec(c, models);
}
void ModelData::readFromBuffer(void *p)
{
	char *c = (char*)p;
	readVec(c, nodes);
	readVec(c, models);
}
TextData::TextData()
{
}
TextData::TextData(float u, float v, float w) :u(u), v(v), w(w)
{
}
VertexData::VertexData()
{
}
VertexData::VertexData(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}
NormalData::NormalData()
{
}
NormalData::NormalData(float x, float y, float z) : x(x), y(y), z(z)
{
}
Float4x4 Float4x4::operator*(const Float4x4 & f) const
{
	Float4x4 ret;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			float temp = 0;
			for (size_t q = 0; q < 4; q++)
			{
				temp += this->val[i][q] * +f.val[q][j];
			}
			ret.val[i][j] = temp;
		}
	}
	return ret;
}
Float4x4 Float4x4::transpose() const
{
	Float4x4 ret;
	ret.val[0][0] = val[0][0];
	ret.val[0][1] = val[1][0];
	ret.val[0][2] = val[2][0];
	ret.val[0][3] = val[3][0];
	ret.val[1][0] = val[0][1];
	ret.val[1][1] = val[1][1];
	ret.val[1][2] = val[2][1];
	ret.val[1][3] = val[3][1];
	ret.val[2][0] = val[0][2];
	ret.val[2][1] = val[1][2];
	ret.val[2][2] = val[2][2];
	ret.val[2][3] = val[3][2];
	ret.val[3][0] = val[0][3];
	ret.val[3][1] = val[1][3];
	ret.val[3][2] = val[2][3];
	ret.val[3][3] = val[3][3];
	return ret;
}