///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Stream.cpp
/// Date: 01/04/2007
///
/// Description:
///
/// Stream source code.
///****************************************************************

#include "opCPP.h"

///
/// opStringStream
///

bool opStringStream::ForceComments = false;

void opStringStream::EndLine()
{
	if(ignorenewlines)
		return;
	
	if (!forcenewlines && opParameters::Get().Compact)
	{
		linestream.TrimRight();

		if (linestream.IsEmpty())
		{
			//lastlinenumber += 1;
			return;
		}
	}

	if(linenumbers.size() || overrideline)
	{
		int nextlinenumber = overrideline?overrideline:linenumbers.back();
		FileNode* file = overrideline?overridefile:InputFile;
		
		if(nextlinenumber != lastlinenumber + 1)
		{
			//o += IndentString;
			
			//if we don't have line directives, I just want to comment it out for now
			if(!bLineDirectives)
				o += "// ";
			
			o += opString("#line ") + nextlinenumber + " \"";
			o += file->GetAbsoluteFileName() + "\""; 				
			
			//TODO: add info output here...
			o += "//[";
			
			//if info available...print it
			//1. generated depth (of all files)
			o += opString(gendepth);
			
			//2. depth to generated (of this file)
			o += "," + opString(filedepth);
			
			//3. original line number
			if(lineorigin != -1)
			{
				o += "," + opString(lineorigin);
			}
			
			o += "]";
			
			if (opParameters::Get().Notations)
			{
				o += " // line #'s :";
				for(int i = 0; i < (int)linenumbers.size(); i++)
					o += opString(' ') + linenumbers[i]; 
				
				if(overrideline)
					o += " (-)";
			}
			
			o += '\n';
		}
		
		lastlinenumber = nextlinenumber;
	}
	else
		lastlinenumber += 1;
	
	linenumbers.resize(0);

	if(linestream.Size())
		o+=IndentString;

	o += linestream + '\n';
	linestream.Clear();
}


void opStringStream::OverrideLine(opNode* node)
{
	overrideline = node->GetLine();
	overridefile = node->GetFile();
}

void opStringStream::ResetOverrideLine()
{
	overrideline = 0;
	overridefile = NULL;
}

void opStringStream::SetIgnoreRedirects(bool bignore)
{
	ignoreredirects = bignore;
}

void opStringStream::SetLineOrigin(opNode* node)
{
	lineorigin = node->GetLine();
}

void opStringStream::ResetLineOrigin()
{
	lineorigin = -1;
}

void opFileStream::SetDepths(const opString& outputpath)
{
	const opParameters& p = opParameters::Get();
	
	// get the generated depth
	path generatedpath = p.GeneratedDirectory.GetString();
	
	int gendepth = 0;
	
	path::iterator git = generatedpath.begin();
	path::iterator gend = generatedpath.end();
	
	while(git != gend)
	{
		++gendepth;
		++git;
	}
	
	// get the file depth
	path prependpath = outputpath.GetString();
	
	opString prepend;
	
	int filedepth = 0;
	
	path::iterator pit = prependpath.begin();
	path::iterator pend = prependpath.end();
	--pend;
	
	while(pit != pend)
	{
		++filedepth;
		++pit;
	}
	
	header.SetDepths(filedepth,gendepth);
	source.SetDepths(filedepth,gendepth);
}

