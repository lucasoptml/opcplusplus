///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Driver.h
/// Date: 11/26/2006
///
/// Description:
///
/// opDriver class declaration.
///****************************************************************

class opDriver
{
public:
	// construction / destruction
	opDriver() {}
	virtual ~opDriver() {}

	// utility
	bool Convert(const opParameters &p);

	//only to be called once
	void Initialize();

	// find all files of extension 'ext' in a directory
	static void FindFilesInDirectory(const opString& directory, const string& ext, opSet<path>& foundfiles);

	// get all oh files listed in parameters
	static opSet<path> GetFiles() { return OhFiles; }

	// get all doh files listed in parameters
	static opSet<path> GetDialectFiles() { return DohFiles; }

	// returns true if the file exists in the files specified on
	// the command line arguments
	static bool FileExists(path file);

	// adds 'num' errors to the number of total driver errors
	static void AddErrors(int num) { NumErrors += num; }

private:	
	// validate all oh files listed in parameters
	static bool ValidateFiles(const opParameters& p);
	
	// validate all dialect files listed in parameters
	static bool ValidateDialectFiles(const opParameters& p);

	// get the latest timestamp of all dialects used
	static time_t GetDialectTimestamp(const opParameters& p);

	// get the latest timestamp of all generated dialects used
	static time_t GetGeneratedDialectTimestamp(const opParameters& p);

	// is an additional dependency out of date?
	// if so update it and return true
	static bool CheckDependencies();

	// clean mode
	bool CleanMode(const opParameters& p);

	// normal opcpp mode
	bool NormalMode(const opParameters &p);

	// operate on a single file
	bool NormalModeFile(const opParameters& p, const path& filename);
	
	// dialect reading mode
	bool DialectMode(const opParameters& p);
	
	// operate on a single file
	bool DialectModeFile(const opParameters& p, const path& filename);
	
	// glob mode
	bool GlobMode(const opParameters& p);
	
public:
	// timestamp checking

	//are all dialects up to date?
	bool DialectsCurrent();

	//are all code files up to date?
	bool CodeCurrent();

	// utility functions
public:
	
	static string GetOutputPath(const opParameters& p, const path& inputpath);
	
	// get path relative to another path
	static string GetRelativePath(const opString& targetpath, const opString& basepath);
	
	// convert to and from generated paths
	static string ToGeneratedPath(const opString& inputpath);
	static string FromGeneratedPath(const opString& inputpath);
	
private:
	// private control
	void ForceCompile();
	
	friend Globber;

	static opSet<path> OhFiles;
	static opSet<path> DohFiles;
	static int         NumErrors;
};

