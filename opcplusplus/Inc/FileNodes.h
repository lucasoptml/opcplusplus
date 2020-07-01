///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: FileNodes.h
/// Date: 12/04/2006
///
/// Description:
///
/// This file contains nodes for parsing files.
///****************************************************************

namespace nodes
{

class FileNode;
class OPFileNode;

typedef Blocks< Preprocessors< opNode > > FileNodeParent;

// This is an abstract base class.
class FileNode : public FileNodeParent
{
public:
	DECLARE_NODE(FileNode,opNode,T_UNKNOWN);
	
	void Init()
	{
		scanMs = 0;
		parseMs = 0;
		totalMs = 0;
		SetLine(0);
		SetFile(this);
		bAbsolutePath = false;
	}

	~FileNode();
	
	const opString& GetInputName()
	{
		return InputName;
	}

	const opString& GetAbsoluteFileName()
	{
		return AbsoluteFileName;
	}
	
	bool IsAbsolutePath()
	{
		return bAbsolutePath;
	}

	/**** utility ****/

	virtual FileNode* ToFileNode()
	{
		return this;
	}
	
	//load this file to this node, should we scan/identify test mode tokens?
	//should always return a file, but do check the errors
	template<class T>
	static T* Load(const opString& file, opScanner::ScanMode scanmode, bool bIncluded = false);
	
	// Process this node, inner nodes
	// we have to catch file errors in this
	bool PreProcess();	
	bool Process();

	// happens after PreProcess, before anything else
	virtual bool PreOperations();

	// happens after Process
	virtual bool Operations();

	// happens after PostProcess
	virtual bool PostOperations();
	
	// copy errors from an inner file
	// void MergeErrors(FileNode* file);
	
	// utility
	virtual bool Preprocessor();
	bool CheckBlockCorrectness();

	void PrintIncluded(opDialectStream& stream);
	void PrintIncluded(opFileStream& stream);
	void PrintIncluded(opXmlStream& stream);

protected:
	opString WriteFileHeader(stringstream& o, opString filename);
	
private:
	//input filename
	opString InputName;
	
	//absolute path for the file
	opString AbsoluteFileName;

	//is the path absolute or relative?
	bool bAbsolutePath;

	// timing
	double scanMs;
	double parseMs;
	double totalMs;

public:
	//get timing
	double GetScanMs()
	{
		return scanMs;
	}

	double GetParseMs()
	{
		return parseMs;
	}

	double GetTotalMs()
	{
		return totalMs;
	}

	//file dependencies
	void AddDependency(const opString& filepath);
	void SaveDependencies(const opString& filepath);
	bool LoadDependencies(const opString& filepath);

	bool IsDependencyNewer(time_t timestamp);

private:
	opSet<opString> Dependencies;


private:
	//internal file tables
	static opArray<FileNode*> FileTable;
	
	static FileNode* GetLoadedFile(const opString& filename);
	
	static void		 UnRegisterLoadedFiles();
	static void      DeleteLoadedFiles();
	
	friend opDriver;
	friend opMemoryTracker;
};

///==========================================
/// OPFileNode
///==========================================

//defines the normal file context (should really be moved to a context)
typedef context::Global< ExpandCalls< OPMacros< FileNode > > > OPFileNodeParent;

// This represents an .oh file to be parsed.
class OPFileNode : public OPFileNodeParent
{
public:
	DECLARE_NODE(OPFileNode,OPFileNodeParent,G_OPFILE);

	//these functions handle the preparse pass
	//which handle opmacro expansions
	virtual bool PreParse();
	virtual bool PostParse();

	virtual void PrintNode(opFileStream& stream);
	virtual void PrintXml(opXmlStream& stream);

	void SetFiles(const opString& headerfile, const opString& sourcefile)
	{
		HeaderFile = headerfile;
		SourceFile = sourcefile;
	}
	
	opString ErrorName() { return ""; }
	
private:
	
	//output filenames
	opString SourceFile;
	opString HeaderFile;
	
};

//This represents a cpp file
class CppFileNode : public FileNode
{
public:
	DECLARE_NODE(CppFileNode,FileNode,G_CPPFILE);

	void PrintNode(opFileStream& stream)
	{
		PrintNodeChildren(stream);
	}

	void PrintOriginal(opSectionStream& stream )
	{
		PrintOriginalUnspacedChildren(stream);
	}

	opString ErrorName() { return ""; }

};

} // end namespace nodes

