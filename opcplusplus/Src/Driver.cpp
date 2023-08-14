///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Driver.cpp
/// Date: 11/26/2006
///
/// Description:
///
/// Contains opDriver source code.
///****************************************************************

#include "opCPP.h"

//
// opDriver
//

opSet<path> opDriver::OhFiles;
opSet<path> opDriver::DohFiles;
int         opDriver::NumErrors = 0;

void opDriver::Initialize()
{
	// initialize token stuff
	InitTokens();
	
	// init error stuff
	opError::InitParseErrors();

	// init the timer
	opTimer::InitTimeSeconds();
}

// converts the input opCPP format to c++ format
bool opDriver::Convert(const opParameters &p)
{
	opMemoryTracker memorytracker;
	
	bool bResult = true;

	// run it
	try
	{
		// Validate the files specified on the command 
		// line (oh and doh).
		if (!ValidateDialectFiles(p)
		||  !ValidateFiles(p))
		{
			return false;
		}

		if(p.CleanMode)
		{
			bResult = CleanMode(p) ? bResult : false;

			return bResult;
		}

		if(!bResult)
			return false;
		
		bool bSkipCompiling = false;
		
		//must be in normal mode to read dialects
		if(p.NormalMode)
		{
			//check dependencies
			bool bNewDependency = CheckDependencies();
			
			if (bNewDependency)
			{
				if (p.Verbose)
				{
					Log("Dependency is out of date, forcing recompile ...");
					Log("");
				}
				
				ForceCompile();
			}
			
			//if new dependency, we can't try to skip
			if (!bNewDependency && !p.Force)
			{
				//early skip detection:
				//if we have any dialect files out of date, we must continue
				if(DialectsCurrent())
				{
					//if we have any code files out of date, we must continue
					if(CodeCurrent())
					{
						//or else, we can just get out of here...
						bSkipCompiling = true;
					}
				}
			}
			
			if(!bSkipCompiling)
				bResult = DialectMode(p)?bResult:false;			
		}

		if(!bSkipCompiling)
		{
			if(!bResult)
				return false;

			//validate dialect read parameters
			if(!opParameters::ValidateParameters())
				return false;

			//validate registered dialects
			if(!DialectTracker::Validate())
			{
				opError::Print();	
				return false;
			}

			if(p.NormalMode)
				bResult = NormalMode(p)?bResult:false;

			if(!bResult)
				return false;
		}

		// glob mode
		if(p.GlobMode)
			bResult = GlobMode(p)?bResult:false;

		if(!bResult)
			return false;

		return true;
	}
	catch(opException::opCPP_Exception&)
	{
		opError::ExceptionError("opCPP");
	}
	catch(opException::opCPP_ParseException&)
	{
		opError::ExceptionError("opCPP Parse");
	}
	catch(opException::opCPP_FatalException&)
	{
		opError::ExceptionError("opCPP Fatal");
	}
	catch(opException::opCPP_AssertException& e)
	{
		opError::ExceptionError("Assert Failed (" + e.exceptionstring + ")");
	}
	catch( std::filesystem::filesystem_error& fe )
	{
		opString errorstr = fe.what();
		//TODO: this may be incorrect (was fe.who() replace)
		errorstr.Replace(fe.path1().string(), "Error: Improper path detected ");
		Log(errorstr);
		Log(opParameters::Get().GetCommandLineString());
	}
	catch(...)
	{
		opError::ExceptionError("Unhandled Unknown");
	}

	opException::CaughtException();

	return false;
}

void opDriver::ForceCompile()
{
	opParameters& p = opParameters::GetWritable();
	p.Force = true;
}

//TODO: dependencies does track file paths, only names - this doesn't matter really.
bool opDriver::CheckDependencies()
{
	const opParameters& p = opParameters::Get();
	
	bool bResult = false;

	int size = p.Depend.size();
	for(int i = 0; i < size; i++)
	{
		opString filestring = p.Depend[i];
		
		path filepath = filestring.GetString();
		
		if(exists(filepath))
		{
			opString targetfile = filepath.filename().string();
			
			targetfile += ".target";
			
			path targetpath = path(p.GeneratedDirectory.GetString()) / targetfile.GetString();
			
			// if it doesn't exist we need to create it
			if(!exists(targetpath))
			{
				create_directories(p.GeneratedDirectory.GetString());

				//just create a new empty target file
				std::ofstream target(targetpath);
				target << "target file used for opC++ -dependencies";
				bResult = true;
				continue;
			}
			
			//now check for the time
			auto filetime = last_write_time(filepath);
			auto targettime = last_write_time(targetpath);
			
			if(filetime > targettime)
			{
				//just create a new empty target file
				std::ofstream target(targetpath);
				target << "target file used for opC++ -dependencies";
				bResult = true;
				continue;
			}
		}
	}

	return bResult;
}

void opDriver::FindFilesInDirectory(const opString& directory, const string& ext, opSet<path>& foundfiles)
{
	path dirpath = directory.GetString();

	//does this directory exist?
	if ( !exists( dirpath ) )
		return;//no
	
	directory_iterator end_itr; // default construction yields past-the-end
	for ( directory_iterator itr( dirpath ); itr != end_itr; ++itr )
	{
		if ( !is_directory( *itr ) )
		{
			//get the path opstring..
			path path_found = *itr;
			
			//does the extension match?
			if(path_found.extension() == ext)
				foundfiles.insert(path_found);
		}
	}
}

bool opDriver::ValidateFiles(const opParameters& p)
{
	/*=== files ===*/

	const vector<opString>&    tmpFiles = p.Files.GetValues();
	opList<opString>           Files(tmpFiles.begin(), tmpFiles.end());
	opList<opString>::iterator i   = Files.Begin();
	opList<opString>::iterator end = Files.End();
	opList<opString>::iterator old;
	
	while (i != end)
	{
		path filepath = (*i).GetString();

		if (exists(filepath))
		{
			filepath = to_relative_path(filepath);
			OhFiles.insert(filepath);
			old = i;
			++i;
			Files.Erase(old);
			continue;
		}

		++i;
	}
	
	/*=== files in -d locations ===*/

	const vector<opString>& Dirs = p.Directories.GetValues();

	for (size_t d = 0; d < Dirs.size(); d++)
	{
		i   = Files.Begin();
		end = Files.End();

		while (i != end)
		{
			path filepath = Dirs[d].GetString() / to_relative_path((*i).GetString());

			if (exists(filepath))
			{
				filepath = to_relative_path(filepath);
				OhFiles.insert(filepath);
				old = i;
				++i;
				Files.Erase(old);
				continue;
			}

			++i;
		}
	}

	/*=== error if files were not found ===*/

	if (!Files.IsEmpty())
	{
		Log("Error: The following files were not found:");

		i   = Files.Begin();
		end = Files.End();

		while (i != end)
		{
			Log("\t'" + *i + "'");

			++i;
		}

		Log("");

		return false;
	}

	/*=== files in -ohd locations ===*/

	const vector<opString>& FileDirs = p.FileDirectories.GetValues();

	for(size_t d = 0; d < FileDirs.size(); d++)
		FindFilesInDirectory(FileDirs[d],".oh",OhFiles);

	return true;
}

// returns true if the file exists in the files specified on
// the command line arguments
bool opDriver::FileExists(path file)
{
	return find(OhFiles.begin(),  OhFiles.end(),  file) != OhFiles.end() 
		|| find(DohFiles.begin(), DohFiles.end(), file) != DohFiles.end();
}

// compiles all files
bool opDriver::NormalMode(const opParameters &p)
{
	// verify the output directory...
	path dirpath = p.GeneratedDirectory.GetString();
	
	if(!exists(dirpath))
		create_directories(dirpath);
	
	opSet<path> files = GetFiles();
	
	// if there are no files to compile, return false
	if (files.size() == 0)
	{
		if (!p.Silent)
			Log("Error: No files to compile!");
		
		return false;
	}
	
	// compile all files
	bool bResult = true;
	
	typedef opSet<path>::const_iterator fileit;
	
	if (p.Verbose)//spacing in verbose mode
		Log(' ');
	
	for( fileit it = files.begin(); it != files.end(); ++it )
	{
		bResult = NormalModeFile(p,*it)?bResult:false;
	}
	
	// If we had errors, print out the number of errors.
	if (NumErrors > 0)
	{
		Log("");
		string errorstring = (NumErrors == 1) ? " error" : " errors";
		Log("opC++ - " + opString(NumErrors) + errorstring);
		Log("");
	}

	if (!p.Silent && files.size() > 1)
	{
		if(p.Verbose)
		{
			Log(' ');
			
			if (bResult)
			{
				Log("opC++ - 0 errors");
				Log("");
				Log("--------------------------------");
				Log("All Files Compiled Successfully!");
				Log("--------------------------------");
			}
			else
			{
				Log("-------------------------------------");
				Log("Some File(s) Compiled Unsuccessfully!");
				Log("-------------------------------------");
			}
			
			Log(' ');
		}
	}

	return bResult;
}

string opDriver::ToGeneratedPath(const opString& inpath)
{
	const opParameters& p = opParameters::Get();
	
	path adjustedpath = to_relative_path(inpath.GetString());

	// 2. if it's a absolute path, I need to handle the root
	if(adjustedpath.has_root_directory())
	{
		//cases: //
		//       /
		//       c:/

		opString pathstring = adjustedpath.string();
		if(pathstring.StartsWith("/"))
		{
			pathstring = "_root" + pathstring;
		}
		else if(pathstring.StartsWith("//"))
		{
			pathstring = "_net" + pathstring.Right(1);
		}
		else
		{
			//c:/
			pathstring.Replace(":","");
			pathstring = "_win/" + pathstring;
		}

		adjustedpath = pathstring.GetString();
	}
	
	opString adjustedstring = adjustedpath.string();
	adjustedstring.Replace("../","_/");
	
	// 3. if it's a complete network path, I need to handle the root
	path newpath = path(p.GeneratedDirectory.GetString()) / adjustedstring.GetString();
	
	return newpath.string();
}

string opDriver::GetRelativePath(const opString& targetfile, const opString& basefile)
{
	//return to_relative_path(targetpath.GetString(),basepath.GetString()).string();
	//targetpath = Oh/Types.oh <- this must be the real path!
	//basepath   = Generated/Types/Oh/Types.oh
	//want         ../../../Oh/Types.oh

	//if relative path -> easy
	//if absolute path -> must use absolute path
	path targetpath = targetfile.GetString();
	path basepath   = basefile.GetString();

	//absolute case
	if(targetpath.is_absolute())
		return targetpath.string();

	path::iterator pathend = basepath.end();
	--pathend;
	
	path::iterator pathit = basepath.begin();

	path newpath;

	while(pathit != pathend)
	{
		newpath /= "..";
		
		++pathit;
	}

	newpath /= targetpath;

	//FIXME: newpath.normalize();

	return newpath.string();
}

string opDriver::FromGeneratedPath(const opString& inputpath)
{
	const opParameters& p = opParameters::Get();
	
	path directory = p.GeneratedDirectory.GetString();
	opString directorystring = directory.string();
	
	opString newpath = inputpath.Substring(directorystring.Length()+1,inputpath.Length());	
	newpath.Replace("_/","../");

	// windows absolute path 
	if(newpath.StartsWith("_win/"))
	{
		newpath = newpath.Right(5);
		newpath.Insert(1,":");
	}
	// linux root path
	else if(newpath.StartsWith("_root/"))
	{
		newpath = newpath.Right(5);
	}
	// network share path
	else if(newpath.StartsWith("_net/"))
	{
		newpath = "/" + newpath.Right(4);
	}
	
	return newpath.GetString();
}

string opDriver::GetOutputPath(const opParameters& p, const path& inputpath)
{
	// convert the input path to the associated generated path
	path adjustedpath = ToGeneratedPath(inputpath.string());

	return adjustedpath.string();
}

// compiles a file in normal mode
bool opDriver::NormalModeFile(const opParameters& p, const path& filename)
{
	double totaltimestart = opTimer::GetTimeSeconds();

	//build the output filename strings...
	//fix this for ../ case (convert to string and find & replace...)
	opString sfile = GetOutputPath(p,filename);
	
	path oohpath  = (sfile + ".ooh").GetString();
	path ocpppath = (sfile + ".ocpp").GetString();

	path outputpath = oohpath.parent_path();

	if(!exists(outputpath))
		create_directories(outputpath);
	
	//lets check the timestamp...
	if(!p.Force)
	{
		std::filesystem::file_time_type ohtime = last_write_time(filename);
		
		//we want to rebuild upon upgrades / new builds
		std::filesystem::file_time_type opcpptime = opPlatform::GetOpCppTimeStamp();
		
		if (exists(oohpath) && exists(ocpppath))
		{
			std::filesystem::file_time_type oohtime  = last_write_time(oohpath);
			std::filesystem::file_time_type ocpptime = last_write_time(ocpppath);
			
			std::filesystem::file_time_type dohtime = GetGeneratedDialectTimestamp(p);
			
			FileNode tempfile;
			tempfile.LoadDependencies(sfile + ".depend");
			bool bNewDepend = tempfile.IsDependencyNewer(oohtime);

			if (bNewDepend)
			{
				if (p.Verbose)
				{
					Log("Included file newer than generated file, forcing recompile ...");
					Log("");
				}
			}
			//up to date if ooh newer than oh, and ooh newer than opcpp build
			else if(oohtime < opcpptime || ocpptime < opcpptime)
			{
				if (p.Verbose)
				{
					Log(opPlatform::GetOpCppExecutableName() + " newer than generated file, forcing recompile ...");
					Log("");
				}
			}
			else if (oohtime <= dohtime || ocpptime <= dohtime)
			{
				if (p.Verbose)
				{
					Log("Dialect newer than generated file, forcing recompile ...");
					Log("");
				}
			}
			else if (oohtime > ohtime && ocpptime > ohtime)
			{
				if(p.Verbose)
					Log(filename.string() + " is up to date");
				
				return true;
			}
		}
	}
	
	opError::Clear();
	
	//output compiling -file- to std out
	if (!p.Silent)
	{
		Log(opString("Compiling ") + filename.string() + " ...");
	}
	
	//load the oh file, it will be tracked elsewhere
	OPFileNode* filenode = FileNode::Load<OPFileNode>(filename.string(),opScanner::SM_NormalMode);
	
	//filenode should be non-null even if there were errors
	assert(filenode);
	
	if(opError::HasErrors())
	{
		if (p.PrintTree)
			filenode->PrintTree(filename.string());
			
		opError::Print();
		return false;
	}

	// no errors, let's print the output files
	try
	{
		// Save dependencies file.
		opString dependpath  = sfile + ".depend";
		filenode->SaveDependencies(dependpath);

		// open the output files for the generated code...
		FileWriteStream hfile(oohpath.string());
		FileWriteStream sfile(ocpppath.string());
		
		if(hfile.is_open() && sfile.is_open())
		{
			filenode->SetFiles(oohpath.string(),ocpppath.string());
			
			opFileStream filestream(hfile,sfile);
			
			// add the pre-pend path (for relative #lines)
			filestream.SetDepths(oohpath.string());
			
			// files are open, now print to them
			filenode->PrintNode(filestream);
			
			filestream.Output();
		}
		else
		{
			Log("Could not open output file(s)!");
			return false;
		}
	}
	catch(opException::opCPP_Exception&)
	{
		//??? ever
	}

	//print xml!
	if(p.PrintXml)
	{
		try
		{
			path xmlpath = (sfile + ".xml").GetString();

			// open the output files for the generated code...
			std::ofstream xfile(xmlpath);

			if(xfile.is_open())
			{
				opXmlStream filestream(xfile);

				// files are open, now print to them
				filenode->PrintXml(filestream);
			}
			else
			{
				Log("Could not open output xml file!");
				return false;
			}
		}
		catch(opException::opCPP_Exception&)
		{
			//??? ever
		}
	}
	
	//any errors left?
	//shouldn't be really
	opError::Print();

	double totaltimeend = opTimer::GetTimeSeconds();
	double totaltimeMs = (totaltimeend-totaltimestart)*1000.0;

	//TODO: allow PrintTree to any stream
	//		and add support for PrintTree to file
	//print the AST to stdout
	if (p.PrintTree)
		filenode->PrintTree(filename.string());

	//write the verbose compilation notice
	if (p.Verbose)
	{
		Log("");
		Log(opString("Compilation successful ... took ") + totaltimeMs + " ms (" + filenode->GetScanMs() + " scan ms, " + filenode->GetParseMs() + " parse ms)");
	}

	return true;
}

bool opDriver::ValidateDialectFiles(const opParameters& p)
{
	/*=== files ===*/

	const vector<opString>&    tmpDialects = p.Dialects.GetValues();
	opList<opString>           Dialects(tmpDialects.begin(), tmpDialects.end());
	opList<opString>::iterator i   = Dialects.Begin();
	opList<opString>::iterator end = Dialects.End();
	opList<opString>::iterator old;

	while (i != end)
	{
		path filepath = (*i).GetString();

		if (exists(filepath))
		{
			filepath = to_relative_path(filepath);
			DohFiles.insert(filepath);
			old = i;
			++i;
			Dialects.Erase(old);
			continue;
		}

		++i;
	}

	/*=== files in -d locations ===*/

	const vector<opString>& Dirs = p.Directories.GetValues();

	for (size_t d = 0; d < Dirs.size(); d++)
	{
		i   = Dialects.Begin();
		end = Dialects.End();

		while (i != end)
		{
			path filepath = Dirs[d].GetString() / to_relative_path((*i).GetString());

			if (exists(filepath))
			{
				filepath = to_relative_path(filepath);
				DohFiles.insert(filepath);
				old = i;
				++i;
				Dialects.Erase(old);
				continue;
			}

			++i;
		}
	}

	/*=== error if files were not found ===*/

	if (!Dialects.IsEmpty())
	{
		Log("");
		Log("Error: The following dialects were not found:");

		i   = Dialects.Begin();
		end = Dialects.End();

		while (i != end)
		{
			Log("\t'" + *i + "'");

			++i;
		}

		Log("");

		return false;
	}

	return true;
}

std::filesystem::file_time_type opDriver::GetDialectTimestamp(const opParameters& p)
{
	//find the maximum timestamp on the dialects (input dialects, since we output every time)
	std::filesystem::file_time_type maxtime; //FIXME: = 0;

	for(int i = 0; i < p.Dialects.size(); i++)
	{
		// TODO - use generated dialects instead..
		path filepath = p.Dialects[i].GetString();
		
		//doh file exists?
		if(exists(filepath))
		{
			maxtime = max(last_write_time(filepath),maxtime);				
		}
	}
	
	return maxtime;
}

std::filesystem::file_time_type opDriver::GetGeneratedDialectTimestamp(const opParameters& p)
{
	//find the maximum timestamp on the dialects (input dialects, since we output every time)
	std::filesystem::file_time_type maxtime;//FIXME:  = 0;

	for(int i = 0; i < p.Dialects.size(); i++)
	{
		opString filestring = GetOutputPath(p,p.Dialects[i].GetString());
		
		path filepath = (filestring + ".ooh").GetString();

		//doh file exists?
		if(exists(filepath))
		{
			maxtime = max(last_write_time(filepath),maxtime);				
		}
	}

	return maxtime;
}

//read dialects
bool opDriver::DialectMode(const opParameters& p)
{
	opSet<path> files = GetDialectFiles();

	// if there are no files to compile, return false
	if (files.size() == 0)
	{
 		if (!p.Silent)
 			Log("Warning: No dialects to read!");
	}

	// compile all files
	bool bResult = true;

	typedef opSet<path>::const_iterator fileit;

	if (p.Verbose)//spacing in verbose mode
		Log(' ');

	for( fileit it = files.begin(); it != files.end(); ++it )
	{
		bResult = DialectModeFile(p,*it)?bResult:false;
	}

	if (!p.Silent && files.size() > 1)
	{
		if(p.Verbose)
		{
			Log(' ');

			if (bResult)
			{
				Log("--------------------------------");
				Log("All Dialects Read Successfully!");
				Log("--------------------------------");
			}
			else
			{
				Log("-------------------------------------");
				Log("Some Dialect(s) Read Unsuccessfully!");
				Log("-------------------------------------");
			}

			Log(' ');
		}
	}

	return bResult;
}

bool opDriver::DialectModeFile(const opParameters& p, const path& filename)
{
	double totaltimestart = opTimer::GetTimeSeconds();
	
	opError::Clear();
	
	//output compiling -file- to std out
	if (!p.Silent)
	{
		Log(opString("Reading dialect ") + filename.string() + " ...");
	}

	//load the oh file, it will be tracked elsewhere
	DialectFileNode* filenode = FileNode::Load<DialectFileNode>(filename.string(),opScanner::SM_DialectMode);
	
	//filenode should be non-null even if there were errors
	assert(filenode);
	
	if(opError::HasErrors())
	{
		if (p.PrintTree)
			filenode->PrintTree(filename.string());

		opError::Print();

		return false;
	}

	//check for file not found error
// 	if (filenode->FileNotFoundError())
// 	{
// 		opError::Print();
// 
// 		//this is ambiguous doh!
// 		//TODO: fix this to be specific
// 		Log(opString("Cannot open input file \"") + filename.string() + "\"!");
// 		return false;
// 	}
// 	
// 	//check for scanner error
// 	if(filenode->ScanError())
// 	{
// 		opError::Print();
// 
// 		if (p.Verbose)
// 		{
// 			Log("Compilation failed!");
// 			Log("");
// 		}
// 
// 		return false;
// 	}
// 
// 	//check for parser errors
// 	if(filenode->AnyErrors())
// 	{
// 		//print the tree (failure)
// 		if (p.PrintTree)
// 			filenode->PrintTree(filename.string());
// 
// 		opError::Print();
// 
// 		if (p.Verbose)
// 		{
// 			Log("Compilation failed!");
// 			Log("");
// 		}
// 
// 		return false;
// 	}
	
	opString spath = GetOutputPath(p,filename);
	path oohpath  = (spath + ".ooh").GetString();
	path ocpppath  = (spath + ".ocpp").GetString();
	path outputpath = oohpath.parent_path();
	
	if(!exists(outputpath))
		create_directories(outputpath);
	
	//handle dialect writing
	//we always want to read dialects though.
	bool bwrite = true;
	if(!p.Force)
	{
		//we want to rebuild upon upgrades / new builds
		if (exists(oohpath) && exists(filename))
		{
			auto oohtime  = last_write_time(oohpath);
			auto opcpptime = opPlatform::GetOpCppTimeStamp();
			auto dohtime = GetDialectTimestamp(p);
			
			filenode->LoadDependencies(spath + ".depend");
			bool bNewDepend = filenode->IsDependencyNewer(oohtime);
			
			if (bNewDepend)
			{
				if (p.Verbose)
				{
					Log("Included dialect newer than generated dialect file, forcing recompile ...");
					Log("");
				}
			}
			else if(oohtime < opcpptime)
			{
				if (p.Verbose)
				{
					Log(opPlatform::GetOpCppExecutableName() + " newer than generated dialect file, forcing recompile ...");
					Log("");
				}
			}
			else if (oohtime <= dohtime)
			{
				if (p.Verbose)
				{
					Log("Dialect newer than generated dialect file, forcing recompile ...");
					Log("");
				}
			}
			else if (oohtime > dohtime)
			{
				if(p.Verbose)
					Log(filename.string() + " is up to date");
				
				bwrite = false;
			}
		}
	}
	
	if(bwrite)
	{
		try
		{
			// Save dependencies file.
			opString dependpath  = spath + ".depend";
			filenode->SaveDependencies(dependpath);
			
			// open the output files for the generated code...
			FileWriteStream hfile(oohpath.string());
			FileWriteStream sfile(ocpppath.string());
			
			if(hfile.is_open() && sfile.is_open())
			{
				filenode->SetFiles(oohpath.string(),ocpppath.string());
				
				opDialectStream filestream(hfile,sfile);
				
				// add the pre-pend path (for relative #lines)
				filestream.SetDepths(oohpath.string());
				
				// files are open, now print to them
				filenode->PrintDialectNode(filestream);
				
				filestream.Output();
			}
			else
			{
				Log("Could not open output file(s)!");
				return false;
			}
		}
		catch(opException::opCPP_Exception&)
		{
		}
		
		//print xml!
		if(p.PrintXml)
		{
			try
			{
				path xmlpath = (spath + ".xml").GetString();
				
				// open the output files for the generated code...
				std::ofstream xfile(xmlpath);
				
				if(xfile.is_open())
				{
					opXmlStream filestream(xfile);
					
					// files are open, now print to them
					filenode->PrintXml(filestream);
				}
				else
				{
					Log("Could not open output xml file!");
					return false;
				}
			}
			catch(opException::opCPP_Exception&)
			{
				//??? ever
			}
		}
	}
	
	double totaltimeend = opTimer::GetTimeSeconds();
	double totaltimeMs = (totaltimeend-totaltimestart)*1000.0;
	
	//print the tree (success)
	if (p.PrintTree)
		filenode->PrintTree(filename.string());

	//write the verbose compilation notice
	if (p.Verbose)
	{
		Log("");
		Log(opString("Dialect reading successful ... took ") + totaltimeMs + " ms (" + filenode->GetScanMs() + " scan ms, " + filenode->GetParseMs() + " parse ms)");
		Log("");
	}

	return true;
}

bool opDriver::CleanMode(const opParameters& p)
{
	opSet<path> dohfiles = GetDialectFiles();
	opSet<path> ohfiles  = GetFiles();
	
	if (p.Verbose)
		Log("Cleaning generated files...");

	/*=== remove .oh generated code ===*/
	
	opSet<path>::iterator ohend = ohfiles.end();

	for(opSet<path>::iterator it = ohfiles.begin(); it != ohend; ++it)
	{
		string filename       = GetOutputPath(p,(*it));
		string oohfilename    = filename + ".ooh";
		string ocppfilename   = filename + ".ocpp";
		string dependfilename = filename + ".depend";

		path oohpath    = oohfilename;
		path ocpppath   = ocppfilename;
		path dependpath = dependfilename;

		if (exists(oohpath))
			remove(oohpath);
		
		if (exists(ocpppath))
			remove(ocpppath);

		if (exists(dependpath))
			remove(dependpath);
	}

	/*=== remove .doh generated code ===*/

	opSet<path>::iterator dohend = dohfiles.end();

	for(opSet<path>::iterator it = dohfiles.begin(); it != dohend; ++it)
	{
		string filename       = GetOutputPath(p,(*it));
		string oohfilename    = filename + ".ooh";
		string ocppfilename   = filename + ".ocpp";
		string dependfilename = filename + ".depend";

		path oohpath    = oohfilename;
		path ocpppath   = ocppfilename;
		path dependpath = dependfilename;

		if (exists(oohpath))
			remove(oohpath);

		if (exists(ocpppath))
			remove(ocpppath);

		if (exists(dependpath))
			remove(dependpath);
	}

	/*=== remove .index files ===*/

	string oohindex     = GetOutputPath(p,"Generated.oohindex");
	path   oohindexpath = oohindex;

	if (exists(oohindexpath))
		remove(oohindexpath);

	string ocppindex   = GetOutputPath(p,"Generated.ocppindex");
	path ocppindexpath = ocppindex;

	if (exists(ocppindexpath))
		remove(ocppindexpath);

	/*=== remove .target files ===*/

	int size = p.Depend.size();

	for (int i = 0; i < size; i++)
	{
		opString filestring = p.Depend[i];
		path     filepath   = filestring.GetString();
		opString targetfile = filepath.filename().string();

		targetfile += ".target";

		path targetpath = path(p.GeneratedDirectory.GetString()) / targetfile.GetString();

		if (exists(targetpath))
			remove(targetpath);
	}

	return true;
}

// perform globbing
bool opDriver::GlobMode(const opParameters& p)
{
	Globber Globberobj;

	//test settings
	double starttime = opTimer::GetTimeSeconds();

	//TODO: it should print info like...
	// globbing... (if not silent)
	//also..
	// and if no files were found - no files found
	// if no indexes updated -		already up to date
	// if things were updated -		updated indexes

	if (!p.Silent)
	{
		Log(opString("Globbing to ") + to_relative_path(path(p.GeneratedDirectory.GetString())).string() + " directory ...");
		Log("");
	}

	bool bResult = false;

	try
	{
		bResult = Globberobj.Glob(p);
	}
	catch( std::filesystem::filesystem_error& fe )
	{
		opString errorstr = fe.what();
		//TODO: may be incorrect, was fe.what()
		errorstr.Replace(fe.path1().string(), "Error: Improper path detected when globbing.");
		Log(errorstr);
	}
	catch (...)
	{
		opError::ExceptionError("GlobMode");
	}

	double endtime = opTimer::GetTimeSeconds();

	if(p.Verbose)
	{
		double totalms = (endtime - starttime)*1000.0;
		Log(opString("Glob Mode took ") + totalms + " ms");
		Log("");
	}

	if(!bResult && p.Verbose)
		Log(opString("Glob Mode Failed."));

	return bResult;
}

bool opDriver::CodeCurrent()
{
	//foreach dialect we have...
	//and all dependencies, are we up to date?
	//are we up to date?
	opSet<path>::iterator it  = OhFiles.begin();
	opSet<path>::iterator end = OhFiles.end();

	auto opcpptime = opPlatform::GetOpCppTimeStamp();

	while( it != end )
	{
		const path& ohfilepath = *it;
		
		//get the output path
		opString filestring = GetOutputPath(opParameters::Get(),ohfilepath);
		
		path oohfilepath  = (filestring + ".ooh").GetString();
		path ocppfilepath = (filestring + ".ocpp").GetString();
		
		if(!exists(oohfilepath))
			return false;
		
		if(!exists(ocppfilepath))
			return false;
		
		auto ohtime  = last_write_time(ohfilepath);
		auto oohtime  = last_write_time(oohfilepath);
		auto ocpptime = last_write_time(ocppfilepath);
		
		if(ohtime > oohtime
		|| ohtime > ocpptime
		|| opcpptime > oohtime
		|| opcpptime > ocpptime)
			return false;

		//now check dependencies
		FileNode file;
		file.LoadDependencies(filestring + ".depend");

		if(file.IsDependencyNewer(oohtime))
			return false;
		
		++it;
	}

	return true;
}

bool opDriver::DialectsCurrent()
{
	//foreach code files we have...
	//are we up to date?
	opSet<path>::iterator it  = DohFiles.begin();
	opSet<path>::iterator end = DohFiles.end();

	auto opcpptime = opPlatform::GetOpCppTimeStamp();

	while( it != end )
	{
		const path& dohfilepath = *it;
		
		//get the output path
		opString filestring = GetOutputPath(opParameters::Get(),dohfilepath);
		
		path oohfilepath  = (filestring + ".ooh").GetString();
		path ocppfilepath = (filestring + ".ocpp").GetString();
		
		if(!exists(oohfilepath))
			return false;
		
		if(!exists(ocppfilepath))
			return false;
		
		auto dohtime  = last_write_time(dohfilepath);
		auto oohtime  = last_write_time(oohfilepath);
		auto ocpptime = last_write_time(ocppfilepath);
		
		if(dohtime > oohtime
		|| dohtime > ocpptime
		|| opcpptime > oohtime
		|| opcpptime > ocpptime)
			return false;
		
		//now check dependencies
		FileNode file;
		file.LoadDependencies(filestring + ".depend");
		
		if(file.IsDependencyNewer(oohtime))
			return false;
		
		++it;
	}
	
	return true;
}

