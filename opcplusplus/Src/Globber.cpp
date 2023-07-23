#include "opCPP.h"


//
// Globber Source File
//

//TODO: need to make it work safely
//		need to make it catch some exceptions probably
//		need to make it use string/opString a lot less in this and use
//			path a lot more (its slow converting between them too probably).


Globber::Globber()
{

}

Globber::~Globber()
{

}


//
// Glob Mode Execution...
//

//convert a string path to a safe path...
void ConvertToSafePath(opString& stringpath)
{
	stringpath = stringpath.Replace('\\','/');
}


bool Globber::Glob(const opParameters& p)
{
	//NOTE: Here's what we're trying to do here
	//		first we find all the oh files
	//		then we find where all the ocpp files SHOULD be
	//		then we find where all the ooh files SHOULD be
	//		now, we only want to include an oh file in the index
	//			if the matching ooh and ocpp files exist (because maybe its not in the project anymore
	//				...orphaned or whatever)
	//		and we only want to update the indexes if they're out of date (if no ooh files changed why update it?)
	//			so, only if the above conditions hold do we include an oh file in the index and print the index.

	//NOTE: we must replace all '\' characters with '/' or else
	//		boost filesystem will complain.
	//		that is done in opParameters now.

	//TEST: these are test inputs, the real ones should be parameter derived
	opString outputdir = p.GeneratedDirectory.GetValue();
	
	//TODO: it only supports oh files, should definitely do this differently.
	//		the issue is .doh file output, it looks like an oh file (maybe use .h?)
	opString extension = "oh";
	
	//NOTE: heres the new process...
	//I need to recursively find all ooh files
	//within the output directory
	//we only want ooh files with matching ocpp files
	//once we have narrowed these down,
	//we only want to include ooh/ocpp files with matching oh files
	//so back-convert all these paths
	//and do checks, this builds our validohfiles vector,
	//which is now usable.
	
	path outputpath = p.GeneratedDirectory.GetString();

	set<path> oohfiles;
	set<path> ocppfiles;

	FindFilesInDirectoryRecursive(outputpath,".ooh",oohfiles);
	FindFilesInDirectoryRecursive(outputpath,".ocpp",ocppfiles);
	
	typedef set<path>::const_iterator pathit;

	// find the valid oh files
	vector<ohfileinfo> validohfiles;

	pathit oohend = oohfiles.end();
	for(pathit oohit = oohfiles.begin(); oohit != oohend; ++oohit)
	{
		pathit ocppend = ocppfiles.end();
		for(pathit ocppit = ocppfiles.begin(); ocppit != ocppend; ++ocppit)
		{
			//now we want to remove their extensions
			path oohpath = (*oohit);
			path ocpppath = (*ocppit);
			
			opString ocppstring = ocpppath.string();
			opString oohstring = oohpath.string();
			
			ocppstring = ocppstring.RLeft(5);
			oohstring = oohstring.RLeft(4);
			
			//potential oh file!
			if(ocppstring == oohstring)
			{
				//now convert the path...
				opString ohstring = oohstring;
				
				ohstring = opDriver::FromGeneratedPath(ohstring);
				
				//ohstring = ohstring.Replace("_up/","../");
				
				//ohstring = ohstring.Right(outputpath.string() + "/");
				path ohpath = ohstring.GetString();
				
				//found a valid oh file!
				if(exists(ohpath))
				{
					if( opString(ohpath.leaf().string()).Right('.') == extension)
						validohfiles.push_back(ohfileinfo(ohpath,oohpath,ocpppath));
				}
			}
		}
	}
	
	//find the dialect doh files
// 	pathit dohend = dohfiles.end();
// 	for(pathit dohit = dohfiles.begin(); dohit != dohend; ++dohit)
// 	{
// 		//check if the .ooh file exists...
// 		//if it does we'll use it??
// 	}
	
	//TODO: hook it up to spheroid...
	//		now I need to add a test .oh file
	//		from spheroid, and gradually pound
	//		the data statement grammar into a usable condition
	//    - past this, we want sectioned streams (which aren't required, but enable other later things)
	
	//now we have all the valid filenames!
	if(validohfiles.size() && p.Verbose)
	{
		for(size_t i = 0; i < validohfiles.size(); i++)
			Log(opString("Globber: found oh file : ") + validohfiles[i].ohfilepath.string());
	}
	
	//now we need to check whether to build any of the indexes..
	
	//1.
	//determine whether or not to update the ooh index (only if there is an ooh newer than an oh)
	//so, we need to iterate over all the ooh paths
	//and see if any of the files are newer than this..
	path headerindex = outputpath / "Generated.oohindex";
	UpdateIndex<true>(validohfiles,headerindex,p);
	
	//2. 
	//we always build the ocpp index
	//determine whether or not to update the ocpp index (yes always)
	path sourceindex = outputpath / "Generated.ocppindex";
	UpdateIndex<false>(validohfiles,sourceindex,p);
	
	return true;
}

template<bool bheader>
void Globber::UpdateIndex(const vector<ohfileinfo>& files, const path& indexpath, const opParameters& p )
{
	bool bForce = p.Force;

	if( !exists(indexpath) )
		bForce = true;

	//we need to determine if 
	//iterate over all files
	bool bNewer = false;
	if(!bForce)
	{
		time_t indextime = last_write_time(indexpath);
		time_t opcpptime = opPlatform::GetOpCppTimeStamp();

		for(size_t i = 0; i < files.size(); i++)
		{
			time_t ohtime = last_write_time(files[i].ohfilepath);
			
			time_t generatedtime = bheader? 
								   last_write_time(files[i].oohfilepath):
								   last_write_time(files[i].ocppfilepath);

			//the index condition is the important one
			//the header condition is not so important - that just says that an oh file wasn't build (oops)
			if( indextime < opcpptime )
			{
				if(p.Verbose)
					Log(opPlatform::GetOpCppExecutableName() + " newer than index, forcing glob");

				bNewer = true;
				break;
			}

			if( generatedtime > ohtime && generatedtime > indextime)
			{
				bNewer = true;
				break;
			}
		}
	}

	string indexname = bheader?"oohindex":"ocppindex";

	if(bNewer || bForce)
	{
		if(p.Verbose)
			Log(string("Globber: ") + indexname + " out of date, rebuilding...");
		WriteIndex<bheader>(files,indexpath,p);
	}
	else
	{
		if(p.Verbose)
			Log(string("Globber: ") + indexname + " up to date, skipping...");
	}
}

template<bool bheader>
void Globber::WriteIndex(const vector<ohfileinfo>& files, const path& indexpath, const opParameters& p)
{
	//first, create the stream and verify it works
	FileWriteStream o(indexpath.string());

	if(o.is_open())
	{
		//first lets write the index file header
		//lets make it match the other file header somewhat..
		//write normal information
		o << "/*" << endl;
		o << "\tGlob File:     " << indexpath.string() << endl;
		o << "\topCPP Version: " << opString( opVersion::GetVersion() ) << endl;
		o << "\tBuild Date:    " << __DATE__ << " at " << __TIME__ << endl;
		o << "*/" << endl << endl;

		//first lets extract a guarding macro from the headerindex path
		opString filestring = indexpath.string();
		filestring.Replace('/','_');
		filestring.Replace(' ','_');
		filestring.Replace('.','_');
		filestring.Replace(':','_');
		filestring.Replace('+',"PLUS");
		filestring.Replace('-',"DASH");
		//TODO: should actually catch any and all characters which are valid
		//		in linux and windows file systems, but which are not valid in
		//		the preprocessor
		//TODO: this is also in print.cpp, should point to a common function

		filestring = opString("__") + filestring + "__";

		//next lets write any header specific defines
		o << "#ifndef " << filestring << endl;
		o << "#define " << filestring << endl << endl;

		if(!bheader)
		{
			if (!opParameters::Get().Compact)
				o << "//compile source now" << endl;

			o << "#ifndef OPCOMPILE_SOURCE" << endl;
			o << "\t#define OPCOMPILE_SOURCE" << endl;
			o << "#endif" << endl << endl;
 		}

		//now lets write all the file lists
		//how this works is... sections with NOSECTION defined will not be compiled
		//but theres no similar requirement (SECTION define) for compiling a section...

		opString macrostart = bheader ? "OPCOMPILE_OOH_" : "OPCOMPILE_OCPP_";
		opString buildmacro = bheader ? "OPCOMPILING_OOH" : "OPCOMPILING_OCPP";
		
		//include the dialect output files for the header index

		vector<opString> dialects;

		//dialect output paths
		opSet<path> pdialects = opDriver::GetDialectFiles();
		typedef opSet<path>::iterator diterator;

		diterator dend = pdialects.end();
		
		path outputpath = p.GeneratedDirectory.GetString();
		
		for(diterator dit = pdialects.begin(); dit != dend; ++dit)
		{
			path dialectpath = *dit;

			//path dialect = outputpath / dialectpath;
			path dialect = dialectpath;
			opString dialectstring = dialect.string();

			if(bheader)
				dialectstring += ".ooh";
			else
				dialectstring += ".ocpp";

			dialectstring.Replace("../","_/");

// 			if(exists(dialectstring.GetString()))
// 			{
				dialects.push_back(dialectstring);
//			}
		}
		
		
		
		//now lets write the header section
		if (!opParameters::Get().Compact)
			o << "//compile all the code in file header sections now" << endl;

		o << "#define " << macrostart << "HEADING" << endl;
		WriteDialectList<bheader>(o,dialects,buildmacro + "_HEADING");
		WriteIndexList<bheader>(o,files,buildmacro + "_HEADING");
		o << "#undef " << macrostart << "HEADING" << endl << endl;
		
		//now lets write the body section
		if (!opParameters::Get().Compact)
			o << "//compile all the code in file body sections now" << endl;

		o << "#define " << macrostart << "BODY" << endl;
		WriteDialectList<bheader>(o,dialects,buildmacro + "_BODY");
		WriteIndexList<bheader>(o,files,buildmacro + "_BODY");
		o << "#undef " << macrostart << "BODY" << endl << endl;
		
		//now lets write the footer section
		if (!opParameters::Get().Compact)
			o << "//compile all the code in file footer sections now" << endl;

		o << "#define " << macrostart << "FOOTER" << endl;
		WriteDialectList<bheader>(o,dialects,buildmacro + "_FOOTER");
		WriteIndexList<bheader>(o,files,buildmacro + "_FOOTER");
		o << "#undef " << macrostart << "FOOTER" << endl << endl;
		
		//write the guard footer
		o << "#endif//header" << endl << endl;
	}

}

template< bool bheader >
void Globber::WriteIndexList(FileWriteStream& o, const vector<ohfileinfo>& files, const opString& macroname)
{
	o << endl;

	for(size_t i = 0; i < files.size(); i++)
	{
		opString filestring;
		opString ohfilestring;

		if(bheader)
		{
			ohfilestring = files[i].ohfilepath.string();
			filestring = ohfilestring + ".ooh";
		}
		else
		{
			ohfilestring = files[i].ohfilepath.string();
			filestring = ohfilestring + ".ocpp";
		}
		
		o << "#ifdef " << macroname << endl;
		o << '\t' << macroname << '(' << ohfilestring << ')' << endl;
		o << "#endif" << endl;
		o << "#include \"" << filestring << "\"" << endl << endl;
	}
}

template< bool bheader >
void Globber::WriteDialectList(FileWriteStream& o, const vector<opString>& dialects, const opString& macroname)
{
	o << endl;

	for(size_t i = 0; i < dialects.size(); i++)
	{
		o << "#include \"" << dialects[i] << "\"" << endl << endl;
	}
}

//
// Globber Utility Functions
//

void Globber::FindFilesInDirectory(const opString& directory, const opString& extension, vector<opString>& foundfiles)
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
			opString filestring = path_found.string();
			
			//does the extension match?
			if(filestring.RRight(extension.Length()) == extension)
				foundfiles.push_back(filestring);
		}
	}
}

void Globber::FindFilesInDirectoryRecursive(const path& directory, const opString& ext, set<path>& foundfiles)
{
	//does this directory exist?
	if ( !exists( directory ) )
		return;

	directory_iterator end_itr; // default construction yields past-the-end
	for ( directory_iterator itr( directory ); itr != end_itr; ++itr )
	{
		if (is_directory( *itr ) )
		{
			FindFilesInDirectoryRecursive(*itr,ext,foundfiles);
		}
		else
		{
			//get the path opstring..
			path path_found = *itr;

			//does the extension match?
			if(extension(path_found) == ext)
				foundfiles.insert(path_found);
		}
	}}


