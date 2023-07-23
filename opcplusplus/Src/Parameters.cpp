///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Parameters.cpp
/// Date: 11/26/2006
///
/// Description:
///
/// Contains source code for opParameters command line parser class.
///****************************************************************

#include "opCPP.h"

//
// opOption
//

// constructor
opOption::opOption(const opString& name, const opString& description, bool hidden) 
	: Name(name),
	  Description(description),
	  bHidden(hidden),
	  bUsed(false)
{
	//register this
	opParameters::AddOption(this);
}

//
// opBoolOption
//

// constructor
opBoolOption::opBoolOption(const opString& name, const opString& description, bool hidden, bool defaultvalue)
	: opOption(name, description, hidden), Value(defaultvalue), DefaultValue(defaultvalue)
{

}

//
// opListOption
//

opListOption::opListOption(const opString& name, const opString& description, bool hidden)
	: opOption(name, description, hidden)
{

}

//
// opStringOption
//

opStringOption::opStringOption(const opString& name, const opString& description, bool hidden, const opString& defaultvalue)
	: opOption(name, description, hidden), Value(defaultvalue)
{

}

//
// opIntOption
//

opIntOption::opIntOption(const opString& name, const opString& description, bool hidden, int defaultvalue)
	: opOption(name,description,hidden), Value(defaultvalue)
{

}

//
// opParameters
//

vector<opOption*> opParameters::Options;
opParameters* opParameters::Instance = NULL;

// Returns writable reference.
opParameters& opParameters::GetWritable()
{
	if (Instance == NULL)
		Instance = new opParameters();

	return *Instance;
}

// Returns non-writable reference.
const opParameters& opParameters::Get()
{
	if (Instance == NULL)
		Instance = new opParameters();

	return *Instance;
}

// destroy the instance
void opParameters::Destroy()
{
	if (Instance)
		delete Instance;
}

// Constructor.
opParameters::opParameters()

:	

/*=== normal options ===*/

// Version
Version("version",
"Prints the opC++ compiler version to the standard out.",
false),

// License
License(
"license",
"Location of the compiler's license file.  The default is the directory in which the opC++"
"\n\tcompiler executable resides.",
false,
opPlatform::GetOpCppDirectory().MakeValidPath() + opDemoSupport::LicenseFileName
),

// Files
Files("oh",
"Input code (.oh) files to be compiled, separated by commas.",
false),

// Directories
Directories(
"d",
"Include directories for both code (.oh) and dialect (.doh) files, separated by commas.",
false),

// FileDirectories
FileDirectories(
"ohd",
"List of directories separated by commas.  All .oh files within these directories will be compiled.",
false),

// Depend
Depend(
"dependencies",
"List of additional dependencies separated by commas.  Files will be tracked by the opC++ compiler and detected changes will cause a full rebuild.",
false
),

// GeneratedDirectories
GeneratedDirectory(
"gd",
"All code is output to this directory. The default is 'Generated/'.",
false,
"Generated/"
),

// Dialects
Dialects(
"doh",
"Input dialect (.doh) files to be parsed, separated by commas."
),

// GlobMode
GlobMode(
"globmode",
"Enables glob indexing mode.  In glob mode, the compiler creates index source and header files"
"\n\t(Generated.oohindex, Generated.ocppindex) that include all generated code files.  In this way,"
"\n\tyou can include all your generated code in a project by including 'Generated.oohindex'."
),

// CleanMode
CleanMode(
"clean",
"Removes all generated files (*.ooh, *.ocpp, Generated.oohindex, Generated.ocppindex)."
),

// Verbose
Verbose(
"verbose",
"Tells the compiler to print verbose output."
),

// Diagnostics
Diagnostics(
"diagnostics",
"Tells the compiler to print diagnostic output.  This includes dialect validation messages."
),

// Silent
Silent(
"silent",
"The compiler will only print out compiler errors."
),

// Force
Force(
"force",
"Forces the compiler to recompile everything."
),

// NoDebug
NoDebug(
"nodebug",
"Suppress #line directives (suppress debugging redirection support)."
),

// InlineAll
InlineAll(
"inlineall",
"Tells the compiler to inline all functions in .oh files."
),

// Ghosts
Ghosts(
"ghosts",
"Tells the compiler to output ghost namespaces to improve IDE auto-completion."
),

// Highlighting
Highlighting(
"highlighting",
"Tells the compiler to generate highlighting #define's based on your dialect(s)."
"\n\tNote: This only works if you have a good syntax highlighting engine installed (e.g., Visual Assist)."
),

// Notations
Notations(
"notations",
"The compiler will place note expansion comments in the generated code."
),

// Compact
Compact(
"compact",
"Almost all comments and empty lines are removed from the generated code."
),

// PrintXml
PrintXml(
"printxml",
"The compiler will generate an xml representation of your opC++ code."
),

// NoStandardIncludes
NoStandardIncludes(
"nostandardincludes",
"Tells the compiler to not search its standard include path."
),

// OPMacroExpansionDepth
OPMacroExpansionDepth(
"expansion-depth",
"Specifies the maximum opmacro expansion depth.",
false,
100
),

// FixedSys
FixedSys(
"fixedsys",
"Fixes problems with fixedsys font output."
),

/*=== debug options (these options are hidden) ===*/

// PrintTree (hidden)
PrintTree(
"tree",
"Prints the abstract syntax tree to the standard out.",
true
),

// PrintFullTree (hidden)
PrintFullTree(
"fulltree",
"Prints the abstract syntax tree with values to the standard out.",
true
),

// DeveloperMode (hidden)
DeveloperMode(
"developer",
"Puts the compiler into developer mode.  This will force the compiler to print information specifically for developers.",
true
),

/*=== other ===*/

NormalMode(false)
{
}

// Destructor.
opParameters::~opParameters()
{

}

// In this function, we add all the opOptions.
void opParameters::Init(const vector<opString> &params)
{
	Params = params;
}

// returns input command line string
opString opParameters::GetCommandLineString() const
{
	opString Args = opPlatform::GetOpCppExecutableName() + " ";
	
	for(size_t i = 0; i < Params.size(); i++)
		Args += Params[i] + ' ';
	
	return Args;
}

opOption* opParameters::GetOption(const opString& optionname)
{
	int num = (int)Options.size();
	
	for(int i = 0; i < num; i++)
		if(Options[i]->GetName() == optionname)
			return Options[i];
	
	return NULL;
}

// Parses the command line parameters.
// Note: Assumes that argv[0] is not included!
bool opParameters::Parse()
{
	typedef vector<opString>::iterator paramiterator;
	
	Log("");
	
	paramiterator end     = Params.end();
	paramiterator start   = Params.begin();
	
	//for each command-line parameter...
	//check if its "-"....
	//if not, error
	//if it is, proceed
	
	bool bError = false;

	for(paramiterator it = start; it != end; ++it)
	{
		const opString& s = (*it);
		
		if(s.Length() && s[0] == '-')
		{
			//might be a command-line option
			opString optionstring = s.Right(0);

			//find the option in the parameter list
			opOption* option = GetOption(optionstring);

			// check for beta build
			if ( optionstring.GetString() == "beta" && opBetaSupport::IsBetaBuild())
				continue;

			// if this is an invalid option and it's not the 
			// special beta option
			if ( !option )
			{
				//not an option, error
				if ( optionstring.GetString() != "beta" )
				{
					Log(opString("Unknown Argument : ") + optionstring);
					Log("");
				}

				bError = true;
				break;
			}

			//does the option need an argument
			bool bneedargument = option->HasArgument();

			//does the option take bool ? inform it
			if(!bneedargument)
				option->UsedOption();
			else // does the option take string ? give it one
			{
				++it;
				if(it == end)
				{
					Log(opString("Missing Argument For ") + s);
					Log("");
					bError = true;
					break;
				}

				option->UsedOption(*it);
			}
		}
		else // not a command-line option, error
		{
			//error
			Log(opString("Invalid, Options Must Start With '-' : ") + s);
			Log("");
			bError = true;
		}
	}
	
	// handle errors
	if (bError)
	{
		PrintSyntax();
		return false;
	}
	
	//
	// Pre-dialect command-line processing:
	//
	
	//NOTE: changing of modes should only happen here in command-line parsing.
	
	// use of -oh implies NormalMode = true
	if(Files.GetUsed())
		NormalMode = true;
	
	// use of "-ohd" implies NormalMode = true
	if(FileDirectories.GetUsed())
		NormalMode = true;
	
	if(!NormalMode && !GlobMode && !CleanMode && !Version)
	{
		Log("Error: No Valid Processing Mode Specified.");
		PrintSyntax();
		return false;
	}
	
	// "-fulltree" => "-tree"
	if (PrintFullTree)
		PrintTree = true;
	
	// "-nodebug" takes effect
	if (NoDebug)
		opStringStream::SetLineDirectives(false);
	
	// here we want to add the opCpp Include directory to -d
	//NOTE: happens before dialects are read in.
	if (!NoStandardIncludes)
	{
		opString includedir = opPlatform::GetOpCppDirectory() + "/../../../opcpp/dialects";
		
		Directories.AddValue(includedir);
	}

	return true;
}

bool opParameters::Validate()
{
	// "-nodebug" takes effect
	if (NoDebug)
		opStringStream::SetLineDirectives(false);
	
	// "-fulltree" => "-tree"
	if (PrintFullTree)
		PrintTree = true;

	if (Verbose && Silent)
	{
		Log("Warning: Verbose mode trumped by silent mode.");
		Log("");

		Verbose = false;
	}

	if (Diagnostics && Silent)
	{
		Log("Warning: Diagnostics mode trumped by silent mode.");
		Log("");

		Diagnostics = false;
	}

	if (Compact && Notations)
	{
		Log("Warning: Notations mode trumped by compact mode.");
		Log("");

		Notations = false;
	}
	
	if (!NormalMode && GlobMode && PrintTree)
	{
		Log("Warning: Print tree ignored - can only print tree when compiling files.");
		Log("");

		PrintTree = false;
	}

	// Developer mode should always be enabled in debug.
#ifdef _DEBUG
	DeveloperMode = true;
#endif
	
	return true;
}

void opParameters::PrintSyntax()
{
	opString Syntax;
	
	Syntax  = "\nOptions\n";
	Syntax += "-------\n";
	Syntax += "Note: If a file or directory includes spaces, it must be in quotes!\n";
	Syntax += "      Separate multiple strings with commas and NO SPACES.\n\n";
	Syntax += "      Example:  opcpp -oh file1.oh,\"file 2.oh\" -d dir1,dir2 -verbose";
	
	Log(Syntax);
	
	//Print all options
	int  num          = (int) Options.size();
	bool bPrintHidden = opParameters::Get().DeveloperMode;

	for (int i = 0; i < num; i++)
	{
		opOption* option  = Options[i];
		bool      bHidden = option->GetHidden(); 
		
		if ( !bHidden 
		||   (bHidden && bPrintHidden) )
		{
			Log("");
			Log(opString("-") + option->GetName());
			Log(opString("\t") + option->GetDescription());
		}
	}

	Log("");
}

bool opParameters::SetParameter(const opString& optionname)
{
	opOption* option = GetOption(optionname);

	if(!option)
		return false;

	if(option->HasArgument())
		return false;
	
	if(Verbose)
		Log("setparameter " + optionname);

	option->UsedOption();
	return true;
}

bool opParameters::SetParameter(const opString& optionname, const opString& value)
{
	opOption* option = GetOption(optionname);

	if(!option)
		return false;

	if(!option->HasArgument())
		return false;

	if(Verbose)
		Log("setparameter " + optionname + " " + value);

	option->UsedOption(value);
	return true;
}



