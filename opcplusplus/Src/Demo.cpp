///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Demo.cpp
/// Date: 08/25/2007
///
/// Description:
///
/// opCpp Demo Protection
///****************************************************************

#include "opCPP.h"

///
/// opDemoSupport
///

bool                           opDemoSupport::DemoMode        = true;
const char*				       opDemoSupport::LicenseFileName = "opC++.oplicense";
const char*				       opDemoSupport::ExtraHashString = "1337haxor? .. :P";
opMultiMap<opString, opString> opDemoSupport::Blacklist;

// Returns the demo banner.
const char* opDemoSupport::GetDemoBanner()
{
	if ( !opDemoSupport::IsDemoMode() )
		return "";
	
	return
		"........................................................................................................................\n"
		"........................................................................................................................\n"
		"........................................................................................................................\n"
		"........................................................................................................................\n"
		"........................................................................................................................\n"
		"........................................................................................................................\n"
		"..........................  .... ...  ..,=I$O$..........................................................................\n"
		"................:+7O8DNMMMMMMMMNDD888OOOOOOOOI..........................................................................\n"
		"........?OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOD...........................................................................\n"
		"........~8OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM ..........................................................................\n"
		"........~8OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM...........................................................................\n"
		"........:8OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM...........................................................................\n"
		"........:8OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM...........................................................................\n"
		".........8OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOODO...........................................................................\n"
		".........8OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOON,...........................................................................\n"
		"........ 8OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM............................................................................\n"
		".........DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM............................................................................\n"
		".........DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM............................................................................\n"
		".........DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO$............................................................................\n"
		".........DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO8=............................................................................\n"
		".........DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOON.............................................................................\n"
		".........DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM ............................................................................\n"
		"........ DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM.............................................................................\n"
		"........ NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM ............................................................................\n"
		".........NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOD.............................................................................\n"
		".........NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO7.............................................................................\n"
		"........ MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM,.............................................................................\n"
		"........................................................................................................................\n"
		"........................................................................................................................\n"
		"........................................................................................................................\n"
		"........................................................................................................................\n"
		"........................................................................................................................\n"
		"........MMOZMDDDNM?  ..NNNNNNNNNNNNN$=,, .............   ................ ..............................................\n"
		"........MOOON...78ODMMZNOOOOOOOOOOOOOOO8N. ..........:M. ............. .M,..............................................\n"
		".......?NO~..NOD,..DOOOOO....... ==....DO8N........ O8.?OOOOOOOOOOOOOOZ..D..............................................\n"
		"......~OZ ..OOOOOO..+OOOOOOZ...8OOOOO? ..8OM........M.,OOOOOOOOOOOOOOOO..D..... ..... ............. . ..................\n"
		"......NO...OOOOOOOZ..DOOOOO8...OOOOOOO,...ODD.......M.:OOOOOOOOOOOOOOOO..D.....$M8888MM............,MN8888MZ ...........\n"
		".....MO=..ZOOOOOOO8...OOOOO8...OOOOOOOD...IOM.......M.:OOOO........OOOO..D....:? ......M...........N.......$,...........\n"
		".....D8...DOOOOOOOO...$OOOO8...OOOOOOO8...=OM.......M.:OOOO.=MMMM..OOOO..D....~=.OOOO~.M...........M.,OOOO ?,...........\n"
		"... NO$.. OOOOOOOOO=...OOOO8...OOOOOOOD...?OM.......M.:OOOO.~~..D~...... 8....~=.OOOO~.M...........M.,OOOO ?,...........\n"
		"... NO....OOOOOOOOO7...MOOO8...OOOOOOO=...88M.......M.:OOOO.~~.. NNNDDNMM.NMMMM=.OOOO~.MMMMMD,8NMMMM.,OOOO ?NMMMN. .....\n"
		"....8O....OOOOOOOOOO....OOO8...OOOOOOO ..~OM........M.:OOOO.~~....      ~7.......OOOO~. .... M .... .,OOOO...... 8,.....\n"
		"....8O ...OOOOOOOOO8....OOO8...8OOOD....8OM,........M.:OOOO.~~..........?:.OOOOOOOOOOOOOOOO=.M.IOOOOOOOOOOOOOOOO..?.....\n"
		"....8O....OOOOOOOOOD....OOO8....,,,:+NZO8M..........M.:OOOO.~~..........?:.OOOOOOOOOOOOOOOO=.M.IOOOOOOOOOOOOOOOO..?.....\n"
		"....8O....OOOOOOOOOO....OOO8...OOOOOOOM, ...........M.:OOOO.~~..........?:.OOOOOOOOOOZOOOOO=.M ?OOOOOOOOOOOOOOOO..?.....\n"
		"....NO....OOOOOOOOO7...DOOO8...OOOOOOOM.............M.:OOOO.~~.. .7DDDDO M.......OOOO~......7MI......,OOOO.... .:M .....\n"
		"....NO$ ..OOOOOOOOO+...OOOO8...OOOOOO8M.............M.:OOOO.~~..$M......++.   ~=.OOOO~.M    ....   M.,OOOO ?,   ........\n"
		"....:88...8OOOOOOOO...7OOOO8...OOOOOON+.............M.:OOOO.~~..N .OOOO..D....~=.OOOO~.M...........M.,OOOO ?,...........\n"
		".....MO=..ZOOOOOOO8...OOOOO8...OOOOOOM .............M.:OOOO.~ZZZ$..OOOO..D....~+ ::::..M.......... M..::::.7............\n"
		"......DO...OOOOOOOZ  ZOOOOO8...OOOOOOM..............M.:OOOO........OOOO..D.....MN====?M............~MI===~MM............\n"
		"......:O8...OOOOO8 .?OOOOOOZ...8OOOOON..............M.:OOOOOOOOOOOOOOOO..D......     . .............     .  ............\n"
		".......$8O:..DO8=..DZOOOO+,.....,=8O8?............. M..OOOOOOOOOOOOOOOO..D..............................................\n"
		"........MOOOO...=DOOOOOOO8888888888OD,..............,M...................O..............................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOOOOM.................ZMMMMMMMMMMMMMMMMMN...............................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOOOOM.................. .............. .................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOOOOM...................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOOOOZ...................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOOO8,...................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOOOM....................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOOOM ...................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOOOM....................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOODD....................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOON.....................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOOM.....................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOOM.....................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOO8.....................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOO8=.....................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOON......................................................................................\n"
		"........MOOOOOOOOOOOOOOOOOOOOOOOOM......................................................................................\n"
		".........~NMN8OOOOOOOOOOOOOOOOOOOM......................................................................................\n"
		"............ . $MMNOOOOOOOOOOOOOOD......................................................................................\n"
		"....................=MMMOOOOOOOOOI......................................................................................\n"
		"..........................~MMM8OD ......................................................................................\n"
		".............................  :M ......................................................................................\n"
		"........................................................................................................................\n"
		"........................................................................................................................\n"
		"........................................................................................................................\n"
		"......... Created with opC++ Demo Version - Buy to remove banner and licensing restrictions.               .............\n"
		"......... This generated code may be used for non-commercial and evaluation use ONLY.				        .............\n"
		"........................................................................................................................\n"
		"........................................................................................................................\n\n";
}

// Convert a decimal to octal.	
opString opDemoSupport::DecToOctal(int decimal)
{
	opString octal;

	while(decimal)
	{
		int remainder = decimal % 8;
		decimal = decimal / 8;

		octal = opString(remainder) + octal;
	}

	return octal;
}

// Random letters.
opString opDemoSupport::RandLetters()
{
	opString random = opString(abs(rand()*1231));
	
	for(int i = 0; i < random.Length(); i++)
	{
		char value = random[i] - '0';
		char letter = 'a' + value;
		
		random[i] = letter;
	}
	
	return random;
}

// This method generates the demo string.
opString opDemoSupport::GenerateDemoString()
{
	srand((unsigned int)time(NULL));

	opString letters = RandLetters();

	opString watermark = "Code generated with opCPP demo.";

	opString random;

	random += '"';

	bool lastsplit = false;
	for(int i = 0; i < watermark.Length(); i++)
	{
		if(lastsplit)
		{
			random += "\" \"";
			lastsplit = false;
		}

		if(rand()%2)
		{
			random += watermark[i];
		}
		else
		{
			random += "\\";
			random += DecToOctal(watermark[i]);
		}

		if(rand()%2)
			lastsplit = true;
	}

	random += '"';

	return random;
}

// Print the demo string.
void opDemoSupport::PrintDemoString(opFileStream& stream, opString classname, OPObjectNode* object)
{
	if ( !opDemoSupport::IsDemoMode() )
		return;
	
// 	DialectNote&        prefixnote	= object->GetCategorySettings()->GetPrefixNote();
// 	DialectNote&        postfixnote = object->GetCategorySettings()->GetPostfixNote();
	
	//random letter configurations
	opString randomletters1 = RandLetters();
	opString randomletters2 = RandLetters();
	opString randomletters3 = RandLetters();
	
	//"code generated by opcpp" string
	opString randomstring = GenerateDemoString();
	
	ValuedModifierNode* scope = node_cast<ValuedModifierNode>(object->OPTypeBase::GetContextArgument("scope"));
	stream.header << endl;
	
	stream.header << "class " << classname << randomletters1 << endl;
	stream.header << "{" << endl;
	
	stream.header.Indent();
	
	//write to the header
	stream.header << "virtual const char* " << randomletters2 << "()" << endl;
	stream.header << "{" << endl;
	stream.header << "\treturn " << randomstring << ";" << endl;
	stream.header << "}" << endl;
	stream.header << "static " << classname << randomletters1 << " " << randomletters3 << ";" << endl;
	
	stream.header.DeIndent();
	stream.header << "};";
	stream.header << endl;

	stream.header << "typedef ";
	scope->GetArguments()->PrintOriginal(stream.header.body);
	stream.header << "::" << classname << randomletters1 << " " << classname << randomletters3 << ";" << endl;

	stream.source << classname << randomletters3 << " ";
	stream.source << classname << randomletters3 << "::" << randomletters3 << ";" << endl;
	stream.source << endl;
}

// Returns true if the key/hash combination has been blacklisted.
bool opDemoSupport::IsBlacklisted(const opString& key, const opString& hash)
{
	opMultiMap<opString, opString>::iterator i   = Blacklist.Find(hash);
	opMultiMap<opString, opString>::iterator end = Blacklist.End();

	while (i != end && i->first == hash)
	{
		if (i->second == key)
			return true;

		++i;
	}

	return false;
}

// This method reads in the license file and figures out whether or not 
// we should run in demo mode.
bool opDemoSupport::ValidateLicense(opParameters& params)
{
	opString      licenseFile = opParameters::Get().License.GetString();
	path          licensePath = licenseFile.GetString();
	std::ifstream ifs( licensePath.string().c_str() );

	// If the license file does not exist, we're in demo mode.
	if ( !ifs )
	{
		DemoMode = true;
		return false;
	}

	opString          file;
	opArray<opString> lines;

	file.LoadFile(ifs);
	file.FixNewlines();
	file.Tokenize('\n', lines);
	ifs.close();

	// If their license file is corrupt, we're in demo mode.
	if (lines.Size() < 3)
	{
		DemoMode = true;
		return false;
	}

	opString key  = lines[0].RemoveWhitespace() + lines[1].RemoveWhitespace() + opDemoSupport::ExtraHashString;
	opString hash = lines[2].RemoveWhitespace();

	// If the hash of the key does not match the hash in the file, we're in demo mode.
	if (md5Encode(key) != hash)
	{
		DemoMode = true;
		return false;
	}

	PopulateBlacklist();

	// If this key/hash combination has been blacklisted, then we're in demo mode.
	if (IsBlacklisted(key, hash))
	{
		DemoMode = true;
		return false;
	}

	// We're not in demo mode!
	DemoMode = false;

	return true;
}

