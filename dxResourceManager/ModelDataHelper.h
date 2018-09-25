#ifndef CONST_DEF
#define CONST_DEF
#define CONST_DEF_WAS_NOT_DEFINED
#endif
template < class T> inline void writeVec(char* &p, CONST_DEF std::vector<T> &vec)
{
	writeVec<T, T>(p, vec, T(), std::false_type{});
}
template <> inline void writeVec<std::string>(char* &p, CONST_DEF std::vector<std::string> &vec)
{
	writeSize(p, vec);
	for (size_t i = 0; i < vec.size(); i++)
	{
		writeStr(p, vec[i]);
	}
}
template <> inline void writeVec<ModelDat::BoneDat>(char* &p, CONST_DEF std::vector<ModelDat::BoneDat> &vec)
{
	writeSize(p, vec);
	for (size_t i = 0; i < vec.size(); i++)
	{
		writeVec(p, vec[i].weights);
	}
}
template <> inline void writeVec<ModelDat::MaterialDat>(char* &p, CONST_DEF std::vector<ModelDat::MaterialDat> &vec)
{
	writeSize(p, vec);
	for (size_t i = 0; i < vec.size(); i++)
	{
		writeVec(p, vec[i].diffTextName);
	}
}
template <> inline void writeVec<std::vector<ModelDat::Bone::Frame>>(char* &p, CONST_DEF std::vector<std::vector<ModelDat::Bone::Frame>> &vec)
{
	writeSize(p, vec);
	for (size_t i = 0; i < vec.size(); i++)
	{
		writeVec(p, vec[i]);
	}
}
template <> inline void writeVec<ModelDat::Bone>(char* &p, CONST_DEF std::vector<ModelDat::Bone> &vec)
{
	writeSize(p, vec);
	for (size_t i = 0; i < vec.size(); i++)
	{
		writeData(p, vec[i].parentBone);
		writeData(p, vec[i].boneTransformation);
		writeData(p, vec[i].offsetMatrix);
		writeVec(p, vec[i].frames);
	}
}
template <> inline void writeVec<ModelDat>(char* &p, CONST_DEF std::vector<ModelDat> &vec)
{
	writeSize(p, vec);
	for (size_t i = 0; i < vec.size(); i++)
	{
		writeVec(p, vec[i].vertex, std::numeric_limits<float>::infinity(), std::true_type{});
		writeVec(p, vec[i].index);
		writeVec(p, vec[i].texture, std::numeric_limits<float>::infinity(), std::true_type{});
		writeVec(p, vec[i].normal, std::numeric_limits<float>::infinity(), std::true_type{});
		writeVec(p, vec[i].bonesDat);
		writeVec(p, vec[i].matirialIndexPerSubset);
		writeVec(p, vec[i].materials);
		writeVec(p, vec[i].subset);
		writeVec(p, vec[i].bones);
		writeVec(p, vec[i].nodes);
		writeVec(p, vec[i].animationsNames);
	}
}
#ifdef CONST_DEF_WAS_NOT_DEFINED
#undef CONST_DEF
#endif