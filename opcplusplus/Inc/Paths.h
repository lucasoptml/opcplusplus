///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Paths.h
/// Date: 12/11/2006
///
/// Description:
///
/// Path Utility Function Header
/// 
/// 
/// 
///****************************************************************

using namespace boost::filesystem;

inline bool paths_match(const path& a, const path& b)
{
	if(a == b)
		return true;

#ifdef WIN32
	//case insensitive comparison for windows
	opString astring = opString(a.string()).ToLower();
	opString bstring = opString(b.string()).ToLower();

	if(astring == bstring)
		return true;
#endif

	return false;
}


inline path to_relative_path(path p, path basepath)
{
	if(p.is_complete())
	{
		//we need to convert this path, since its complete
		path workingpath = basepath;

		//special case for windows (case insensitive)


		//first make sure the begin are equal, or else return p (absolute)
		//TODO: well...I need to handle the following cases:
		//A - c:\blah.oh B - d:\blah.doh
		//	well...generation is really machine dependent if you switch paths ever.
		//I'd say correct this, then make doh generation non-path based

		path::iterator wend = workingpath.end();
		path::iterator pend = p.end();

		path::iterator pit = p.begin();
		path::iterator wit = workingpath.begin();

		if(!paths_match(*pit,*wit))
			return p;

		while(paths_match(*pit,*wit) && wit != wend && pit != pend)
		{
			//go until one is exhausted or a mismatch occurs.

			++pit;
			++wit;
		}

		path newpath;

		//first add the remainder of wit (../'s)
		while(wit != wend)
		{
			newpath /= "..";
			++wit;
		}

		//then add the remainder of the targetted
		while(pit != pend)
		{
			newpath /= *pit;
			++pit;
		}

		return newpath;
	}

	return p;
}

inline path to_relative_path(path p)
{
	return to_relative_path(p,boost::filesystem::initial_path());

}


