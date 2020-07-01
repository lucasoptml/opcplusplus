///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Parameters.h
/// Date: 11/26/2006
///
/// Description:
///
/// Command line argument parser for opcpp compiler.
///****************************************************************

class opDriver;
class Globber;

namespace parameters
{

class opParameters;

///==========================================
/// opOption
///==========================================

//TODO: we need a validation phase for these,
//		which treats each option type as
//		its own grammar - we may need more type 
//		options (template types?) at that point

// Base option type.
class opOption
{
public:
	/**** construction / destruction ****/

	opOption(const opString& name, const opString& description, bool hidden);
	~opOption() {}

	/**** get ****/

	const opString& GetName() const
	{
		return Name;
	}

	const opString& GetDescription() const
	{
		return Description;
	}

	bool GetUsed() const
	{
		return bUsed;
	}	

	bool GetHidden() const
	{
		return bHidden;
	}

protected:
	virtual bool HasArgument() = NULL;
	
	//bool version
	virtual void UsedOption() = NULL;

	//string version
	virtual void UsedOption(const opString& arg) = NULL;

	void SetUsed(bool bused)
	{
		bUsed = bused;
	}
	
	friend opParameters;

private:
	const opString Name;
	const opString Description;
	const bool     bHidden;
	bool	       bUsed;
};

//==========================================
// opBoolOption
//==========================================

class opBoolOption : public opOption
{
public:
	/**** construction / destruction ****/

	opBoolOption(const opString& name, const opString& description, bool hidden = false, bool defaultvalue = false);

	/**** get ****/
	
	bool GetValue() const
	{
		return Value;
	}

	operator bool() const
	{
		return Value;
	}

	bool operator = (bool newval)
	{
		Value = newval;
		return Value;
	}

protected:

	/**** used option ****/

	bool HasArgument()
	{
		return false;
	}

	void UsedOption()
	{
		SetUsed(true);
		Value = !DefaultValue;
	}

	void UsedOption(const opString& arg)
	{
		ABSTRACT_FUNCTION;
	}

private:
	bool DefaultValue;
	bool Value;
};

// List of values option.
class opListOption : public opOption
{
public:
	/**** construction / destruction ****/

	opListOption(const opString& name, const opString& description, bool hidden = false);

	/**** typedefs ****/

	typedef vector<opString>			ValueType;
	typedef ValueType::iterator			iterator;
	typedef ValueType::const_iterator	const_iterator;

	/**** get ****/

	const vector<opString>& GetValues() const
	{
		return Values;
	}

	bool FindValue(const opString& s) const
	{
		return find(Values.begin(),Values.end(),s) != Values.end();
	}

	/**** modify ****/
		
	void AddValue(const opString& s)
	{
		Values.push_back(s);
	}

	/**** easy access ****/

	int size() const
	{
		return (int)Values.size();
	}

	const_iterator begin() const
	{
		return Values.begin();
	}

	const_iterator end() const
	{
		return Values.end();
	}

	const opString& operator[](int index) const
	{
		return Values[index];
	}

protected:

	/**** used option ****/

	bool HasArgument()
	{
		return true;
	}

	void UsedOption()
	{
		ABSTRACT_FUNCTION;
	}

	void UsedOption(const opString& arg)
	{
		SetUsed(true);
		
		//Tokenize adds, doesn't clear
		arg.Tokenize(",",Values);
	}

private:
	vector<opString> Values;
};

//==========================================
// opStringOption
//==========================================

class opStringOption : public opOption
{
public:
	opStringOption(const opString& name, const opString& description, bool hidden, const opString& defaultvalue);
	
	/**** get ****/

	const opString& GetValue() const
	{
		return Value;
	}

	const string& GetString() const
	{
		return Value.GetString();
	}

	opString operator = (const opString& newval)
	{
		Value = newval;
		return Value;
	}

protected:

	/**** used option ****/

	bool HasArgument()
	{
		return true;
	}

	void UsedOption()
	{
		ABSTRACT_FUNCTION;
	}

	void UsedOption(const opString& arg)
	{
		SetUsed(true);
		Value = arg;
	}

private:
	opString Value;
};

//==========================================
// opIntOption
//==========================================

class opIntOption : public opOption
{
public:
	opIntOption(const opString& name, const opString& description, bool hidden, int defaultvalue);

	/**** get ****/

	int GetValue() const
	{
		return Value;
	}

	int operator = (int newval)
	{
		Value = newval;
		return Value;
	}

protected:

	/**** used option ****/

	bool HasArgument()
	{
		return true;
	}

	void UsedOption()
	{
		ABSTRACT_FUNCTION;
	}

	void UsedOption(const opString& arg)
	{
		SetUsed(true);
		ArgumentValue = arg;
		Value = ArgumentValue.ToInt();
	}

private:
	opString ArgumentValue;
	int Value;
};

///==========================================
/// opParameters
///==========================================

class opParameters
{
	friend opDriver;
	friend Globber;

public:
	/**** singleton stuff ****/

	static const opParameters& Get();
	
	//TODO: make writable very inaccessible
	static opParameters&       GetWritable();

	static void Destroy();

	/**** startup ****/

	// read command-line arguments (Params)
	void Init(const vector<opString>& params);

	// parse command-line options (pre-dialect reading)
	bool Parse();

	// validate command-line options (post-dialect reading)
	bool Validate();

private:
	//should only be called once from opDriver
	static bool ValidateParameters()
	{
		return GetWritable().Validate();
	}

	/**** private construction / destruction ****/

	opParameters();

	~opParameters();

	/**** internal functions ****/

	void PrintSyntax();

	/**** set ****/

	bool SetParameter(const opString& optionname);

	bool SetParameter(const opString& optionname, const opString& value);

public:

	/**** get ****/

	opOption* GetOption(const opString& optionname);

	opString GetCommandLineString() const;

	/**** data ****/

	vector<opString> Params;

	/*=== normal options ===*/

	opBoolOption    Version;
	opStringOption  License;
	opListOption    Files;
	opListOption	Directories;
	opListOption	FileDirectories;
	opStringOption  GeneratedDirectory;
	opListOption	Dialects;
	opBoolOption	GlobMode;
	opBoolOption	CleanMode; 
	opBoolOption	Verbose;
	opBoolOption    Diagnostics;
	opBoolOption	Silent;
	opBoolOption	Force;
	opBoolOption	NoDebug;
	opBoolOption    Compact;
	opBoolOption	NoStandardIncludes;
	opBoolOption	InlineAll;
	opBoolOption	Ghosts;
	opBoolOption    Highlighting; 
	opBoolOption	Notations;
	opBoolOption    PrintXml;
	opIntOption		OPMacroExpansionDepth;	
	opBoolOption    FixedSys;
	opListOption	Depend;

	/*=== debug options (these options are hidden) ===*/

	opBoolOption	PrintTree;
	opBoolOption	PrintFullTree;
	bool			NormalMode;
	opBoolOption    DeveloperMode;

private:

	friend opOption;

	static void AddOption(opOption* option)
	{
		Options.push_back(option);
	}

	//initialization ordering may be a problem on linux?
	static vector<opOption*> Options;

	static opParameters* Instance;
};

} // end namespace parameters