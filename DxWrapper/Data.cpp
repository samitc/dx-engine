#include "Data.h"
Data::Data(Blob* data):data(data)
{
}
Data::Data(const Data &data):data(data.data)
{
	this->data->AddRef();
}
Data::~Data()
{
	this->data->Release();
}
LPVOID Data::getPointer() const
{
	return this->data->GetBufferPointer();
}
SIZE_T Data::getSize() const
{
	return this->data->GetBufferSize();
}

Data & Data::operator=(const Data & obj)
{
	if (this != &obj)
	{
		this->data->Release();
		this->data = obj.data;
		this->data->AddRef();
	}
	return *this;
}
