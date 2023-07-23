///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: File.h
/// Date: 11/05/2007
///
/// Description:
///
/// File
///****************************************************************

#ifndef OPCPP_FILE
#define OPCPP_FILE

#include <stdio.h>

//#define BOOST_FILESYSTEM_NO_LIB
#include <boost/filesystem/operations.hpp>

class FileReadStream
{
public:
	FileReadStream(const opString& filename) : name(filename)
	{
		file = fopen(filename.GetCString(),"rb");
	}
	
	~FileReadStream()
	{
		fclose(file);
	}
	
	bool IsOpen()
	{
		return file != NULL;
	}

	template<class type>
	void ReadToContainer(type& c)
	{
		int size = (int)boost::filesystem::file_size(name.GetString());
		
		c.Resize(size);
		
		if(size > 0)
			fread(&c[0],sizeof(char),size,file);
	}
	
private:
	opString name;
	FILE* file;
};


class FileWriteStream
{
public:
	FileWriteStream(const opString& filename) : name(filename)
	{
		file = fopen(filename.GetCString(),"wb");
	}

	~FileWriteStream()
	{
		if(file)
			fclose(file);
	}

	bool IsOpen()
	{
		return file != NULL;
	}

	bool is_open()
	{
		return IsOpen();
	}

	template<class type>
	friend FileWriteStream& operator << (FileWriteStream& stream, const type& data)
	{
		stream.Write(data);

		return stream;
	}

	void Write(const opString& s)
	{
		fwrite(&s[0],1,s.Length(),file);
	}

	void Write(const char& s)
	{
		fwrite(&s,sizeof(char),1,file);
	}

private:
	opString name;
	FILE*    file;
};

#endif//OPCPP_FILE


