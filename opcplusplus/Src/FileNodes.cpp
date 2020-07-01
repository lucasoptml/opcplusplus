///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: FileNodes.cpp
/// Date: 12/04/2006
///
/// Description:
///
/// This file contains the source code for parsing files.
///****************************************************************

#include "opCPP.h"

bool opStringStream::bLineDirectives = true;//true - should be true, just for testing!

//
// FileNode
//

opArray<FileNode*> FileNode::FileTable;

FileNode* FileNode::GetLoadedFile(const opString& filename)
{
	int size = FileTable.Size();
	for(int i = 0; i < size; i++)
	{
		FileNode* node = FileTable[i];
		if(node->GetInputName() == filename)
			return node;
	}

	return NULL;
}

FileNode::~FileNode()
{
	if(GetParent())
	{
		opArray<FileNode*>::iterator end = FileTable.end();
		opArray<FileNode*>::iterator item = FileTable.Find(this);
		if(item != end)
			*item = NULL;
	}
}

void FileNode::UnRegisterLoadedFiles()
{
	for(int i = 0; i < FileTable.Size(); i++)
	{
		if(FileTable[i])
			FileTable[i]->UnRegisterReachable();
	}
}

void FileNode::DeleteLoadedFiles()
{
	for(int i = 0; i < FileTable.Size(); i++)
	{
		if(FileTable[i])
			delete FileTable[i];
	}

	FileTable.clear();
}

//
// OPIncludeNode
//

path FindIncludedFile(const path& filepath)
{
	const opParameters& p = opParameters::Get();
	
	//if this exists, use it
	if(exists(filepath))
		return filepath;
	
	typedef vector<opString>::const_iterator dequeit;
	
	//else we'll look into 
	dequeit dend = p.Directories.end();
	for(dequeit it = p.Directories.begin(); it != dend; ++it)
	{
		path combined = (*it).GetString() / filepath;
		
		if(exists(combined))
			return combined;
	}

	dequeit fdend = p.FileDirectories.end();
	for(dequeit it = p.FileDirectories.begin(); it != fdend; ++it)
	{
		path combined = (*it).GetString() / filepath;

		if(exists(combined))
			return combined;
	}

	return filepath;
}

opString OPIncludeNode::GetFullFileString()
{
	opString filestring = GetFileString();

	path filepath = filestring.GetString();

	//attempt to use the current file path
	path currentpath = GetFile()->GetInputName().GetString();
	currentpath = FindIncludedFile(currentpath);
	currentpath = currentpath.branch_path();

	path currentcombined = currentpath / filepath;
	if(exists(currentcombined))
	{
		filepath = currentcombined;
	}
	else
	{
		filepath = FindIncludedFile(filepath);
	}

	return filepath.string();
}

FileNode* OPIncludeNode::IncludeFile()
{
	opString actualfile = GetFullFileString();
	
	//add this file to the dependency list
	RegisterDependency(actualfile);
	
	//NOTE: I don't think this process is very memory safe, should utilize stacked more.

	//include and parse based on inclusion context
	if(IsInside(G_CODE))
	{
		//code node (including cpp source)
		IncludedFile = FileNode::Load<CppFileNode>( actualfile, opScanner::SM_BasicMode, true );
	}
	else if(IsInside(G_OPFILE))
	{
		IncludedFile = FileNode::Load<OPFileNode>( actualfile, opScanner::SM_NormalMode, true );
	}
	else if(IsInside(G_DIALECTFILE))
	{
		IncludedFile = FileNode::Load<DialectFileNode>( actualfile, opScanner::SM_DialectMode, true );
	}
	else
		assert(0);
	
	stacked<FileNode> included = stacked<FileNode>::buildstacked(IncludedFile);
	AppendNode(included);
	
	return IncludedFile;

	//TODO: figure out how erroring works for this...
	//		I dont think we should be tracking 'types' of errors in here though (or in filenode even)

// 	validpath = filepath;
// 	
// 	//pre-pend generated path, append .ooh
// 	generatedpath = path(opParameters::Get().GeneratedDirectory.GetString()) / (validpath.string() + ".ooh");
// 	
// 	//nice error printout
// 	if(IncludedFile->AnyErrors())
// 	{
// 		//TODO: this should really be handled by post-message contexts set by objects
// 		if(IncludedFile->FileNotFoundError())
// 			opError::MessageError(this,opString("opinclude file not found (\"") + filestring + "\")", false);
// 		else if(IncludedFile->ScanError())
// 			opError::MessageError(this,"opinclude file scanner error", false);
// 		else if(IncludedFile->ParseError())
// 			opError::MessageError(this,"opinclude file parser error", false);
// 		else if(IncludedFile->LoopError())
// 			opError::MessageError(this, "opinclude file parser find error", false);
// 		
// 		//NOTE: has to be a better way to pass errors around...
// 		GetFile()->MergeErrors(IncludedFile);
// 		
// 		//opinclude errors are always fatal!
// 		opException::ThrowFatalException();
// 	}
// 	
// 	//preparse the file (so we grab the opmacros)
// 	//TODO: better would be to use a cached tracker and
// 	//		grab the data from it
// 	IncludedFile->PreProcess();
	
}

void OPIncludeNode::RegisterDependency(const opString& filename)
{
	opNode* current = this;
	while(current)
	{
		current = current->GetParent();
		if(FileNode* filenode = current->ToFileNode())
		{
			if(!filenode->GetParent())
			{
				filenode->AddDependency(filename);
				return;
			}
		}
	}
}

void FileNode::PrintIncluded(opDialectStream& stream)
{
	PrintNodeChildren(stream);
}

void FileNode::PrintIncluded(opFileStream& stream)
{
	PrintNodeChildren(stream);
}

void FileNode::PrintIncluded(opXmlStream& stream)
{
	PrintXmlChildren(stream);
}

///==========================================
/// Dependency Tracking
///==========================================

void FileNode::AddDependency(const opString& filepath)
{
	Dependencies.Insert(filepath);
}

void FileNode::SaveDependencies(const opString& filepath)
{
	std::ofstream ofs(filepath);

	opSet<opString>::iterator it = Dependencies.begin();
	opSet<opString>::iterator end = Dependencies.end();

	while(it != end)
	{
		ofs << *it << endl;
		++it;
	}
}

bool FileNode::LoadDependencies(const opString& filepath)
{
	std::ifstream     ifs(filepath, ios::in);
	opString          file;
	opArray<opString> files;
	int               size;
	
	if(ifs.is_open())
	{
		file.LoadFile(ifs);
		file.Tokenize('\n', files);

		size = files.Size();

		for (int i = 0; i < size; i++)
			Dependencies.Insert(files[i].Trim());

		return true;
	}
	
	return false;
}

bool FileNode::IsDependencyNewer(time_t timestamp)
{
	//if we find a newer one, return true
	opSet<opString>::iterator it  = Dependencies.begin();
	opSet<opString>::iterator end = Dependencies.end();

	while(it != end)
	{
		//get the timestamp on the file
		opString filepath = *it;
		
		path dependpath = filepath.GetString();
		if(exists(dependpath))
		{
			time_t dependtime = last_write_time(dependpath);
			
			if(dependtime > timestamp)
				return true;
		}
		else
			return true;
		
		++it;
	}
	
	//if not return false
	return false;
}

bool FileNode::Process()
{
	bool bResult = false;

	bResult = Super::Process();

	if(opError::HasErrors())
		return false;

	return bResult;
}

bool FileNode::PreProcess()
{
	bool bResult = false;

	bResult = Super::PreProcess();

	if(opError::HasErrors())
		return false;

	return bResult;
}

bool FileNode::PreOperations()
{
	OPERATIONS_START
	{	
		opIncludeWalker includefiles(this);
		
		opSymbolTracker tracker;
		opMacroExpander expandnodes(tracker,this);
		
		if(expandnodes.Errored())
			return false;

		ConcatenationWalker concat(this);
	}
	OPERATIONS_END
}

bool FileNode::Operations()
{
	OPERATIONS_START
	{

	}
	OPERATIONS_END
}

bool FileNode::PostOperations()
{
	OPERATIONS_START
	{
		
	}
	OPERATIONS_END
}