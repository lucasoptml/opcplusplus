///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Globber.h
/// Date: 03/13/2007
///
/// Description:
///
/// Globbing.
///****************************************************************

using boost::filesystem::path;

class Globber
{
public:
	Globber();
	~Globber();

	bool Glob(const opParameters& p);

	//file utilities
	//finds all extension types in a directory and adds them to foundfiles
	static void FindFilesInDirectory(const opString& directory, const opString& extension, vector<opString>& foundfiles);
	static void FindFilesInDirectoryRecursive(const path& directory, const opString& extension, set<path>& foundfiles);
private:

	struct ohfileinfo
	{
		ohfileinfo(const path& inohfilepath, const path& inoohfilepath, const path& inocppfilepath)
			: 
			ohfilepath(inohfilepath),
			oohfilepath(inoohfilepath),
			ocppfilepath(inocppfilepath),
			ohfilename(inohfilepath.leaf().string())

		{
			//ohfilepath = inohfilepath;
			//oohfilepath = inoohfilepath;
			//ocppfilepath = inocppfilepath;
			//ohfilename = inohfilepath.leaf();
		}
		
		const path ohfilepath;
		const path oohfilepath;
		const path ocppfilepath;

		const opString ohfilename;
	};

	//try to update indexes potentially
	template<bool bheader>
	void UpdateIndex(const vector<ohfileinfo>& files, const path& headerindex, const opParameters& p );
	//void UpdateSourceIndex(const vector<ohfileinfo>& files, const path& indexpath, const opParameters& p );

	//actually create and write the indexes
	template<bool bheader>
	void WriteIndex(const vector<ohfileinfo>& files, const path& headerindex, const opParameters& p );

	//write the file list to a stream...
	template<bool bheader>
	void WriteIndexList(FileWriteStream& o, const vector<ohfileinfo>& files, const opString& macroname);

	//write the dialect file list to a stream...
	template<bool bheader>
	void WriteDialectList(FileWriteStream& o, const vector<opString>& files, const opString& macroname);
};


