#ifndef _ENGINE_H_
#define _ENGINE_H_
using UTF8 = wchar_t;
#define allocStack(type,size)	((type*)alloca(sizeof(type)*(size)))
#define ALIGN(x)	__declspec(align(x))
#define CONSTEXPRN	constexpr
#ifdef CONSTEXPR
#define CONSTEXPR	constexpr
#else
#define CONSTEXPR
#endif // CONSTEXPR
#define GET_METHOD(varName,type,modifier)\
	modifier:\
	type get##varName() const;
#define SET_METHOD(varName,type,modifier)\
	modifier : \
	void set##varName(const type val);
#define BUILDER_SET_METHOD(varName,type,modifier,className)\
	modifier:\
	className& set##varName(const type val);
#define SAME_GET_METHOD(varName,type,modifier)\
	GET_METHOD(varName,type,modifier);
#define SAME_SET_METHOD(varName,type,modifier)\
	modifier : \
	void set##varName(type val);
#define SAMEC_GET_METHOD(varName,type,modifier)\
	modifier:\
	type get##varName();
#define GET_SET_METHOD(varName,type,modifier)\
	GET_METHOD(varName,type,modifier);\
	SET_METHOD(varName, type, modifier);
#define BUILDER_GET_SET_METHOD(varName,type,modifier,className)\
	GET_METHOD(varName,type,modifier);\
	BUILDER_SET_METHOD(varName, type, modifier,className);
#define SAME_GET_SET_METHOD(varName,type,modifier)\
	SAME_GET_METHOD(varName,type,modifier);\
	SAME_SET_METHOD(varName, type, modifier);
#define SAMEC_GET_SET_METHOD(varName,type,modifier)\
	SAMEC_GET_METHOD(varName,type,modifier);\
	SAME_SET_METHOD(varName, type, modifier);
#define GET_METHOD_IMPL(varName,type,className)\
	type className::get##varName() const { return (varName); }
#define SET_METHOD_IMPL(varName,type,className)\
	void className::set##varName(const type val){ varName = val; }
#define BUILDER_SET_METHOD_IMPL(varName,type,className)\
	className& className::set##varName(const type val){varName = val;return *this;}
#define SAME_GET_METHOD_IMPL(varName,type,className)\
	GET_METHOD_IMPL(varName,type,className);
#define SAMEC_GET_METHOD_IMPL(varName,type,className)\
	type className::get##varName(){ return (varName); }
#define SAME_SET_METHOD_IMPL(varName,type,className)\
	void className::set##varName(type val){ varName = val; }
#define GET_SET_METHOD_IMPL(varName,type,className)\
	GET_METHOD_IMPL(varName,type,className);\
	SET_METHOD_IMPL(varName, type, className);
#define BUILDER_GET_SET_METHOD_IMPL(varName,type,className)\
	GET_METHOD_IMPL(varName,type,className);\
	BUILDER_SET_METHOD_IMPL(varName, type, className);
#define SAME_GET_SET_METHOD_IMPL(varName,type,className)\
	SAME_GET_METHOD_IMPL(varName,type,className);\
	SAME_SET_METHOD_IMPL(varName, type, className);
#define SAMEC_GET_SET_METHOD_IMPL(varName,type,className)\
	SAMEC_GET_METHOD_IMPL(varName,type,className);\
	SAME_SET_METHOD_IMPL(varName, type, className);
#endif