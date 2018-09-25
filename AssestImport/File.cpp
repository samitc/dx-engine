#include "File.h"
#include <sstream>
File::File(std::ifstream * file, const char * loc) :file(std::unique_ptr<std::ifstream>(file))
{
	register int l = strlen(loc) + 1;
	this->loc = new char[l];
	strncpy(this->loc, loc, l);
}
File::File()
{
}
File::File(const File & copy) :file(std::unique_ptr<std::ifstream>(new std::ifstream(copy.loc)))
{
	register int l = strlen(copy.loc) + 1;
	this->loc = new char[l];
	strncpy(this->loc, copy.loc, l);
}
File File::create(const char * loc)
{
	return File(new std::ifstream(loc), loc);
}
File::~File()
{
	delete[] loc;
}
void File::proccessTokens(void *data, tokenFunc func, endFunc end)
{
	const int SIZE = 1000;
	char* text = new char[SIZE];
	char* text1 = new char[SIZE];
	while (file->getline(text, SIZE))
	{
		std::istringstream iss(text);
		while (iss >> text1)
		{
			if (!func(data, text1, (void**)&func))
			{
				break;
			}
		}
		if (!iss.eof())
		{
			break;
		}
		if (!func(data, "\n", (void**)&func))
		{
			break;
		}
	}
	end(data, !file->eof());
	delete[]text;
	delete[]text1;
}
bool File::isOpen()
{
	return this->file->is_open();
}
File & File::operator=(const File & file)
{
	if (this != &file)
	{
		delete[] loc;
		register int l = strlen(file.loc) + 1;
		this->loc = new char[l];
		strncpy(this->loc, file.loc, l);
		this->file.reset(new std::ifstream(file.getloc()));
	}
	return *this;
}
GET_METHOD_IMPL(loc, char*, File);