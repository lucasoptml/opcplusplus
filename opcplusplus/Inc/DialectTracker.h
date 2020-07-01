///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: DialectTracker.h
/// Date: 03/29/2007
///
/// Description:
///
/// Dialect Settings Tracker
///****************************************************************

namespace dialect
{

	//TODO: 
	//	x	need to have all note registrations working
	//	x	need to have all note arguments validated!
	//	x	need to validate all special category note arguments! (should get context)
	//			I should do this by making these DialectNotes :P
	//		need to validate expression modifier usage?
	//			I guess if I don't do that, is ___ will just cause nothing to show up
	//			and isnot ____ just wont do anything - it would be good to validate this
	//			how? well, all modifiers can be checked from the outer category registration
	//				 and all automatic modifiers should be able to be listed from maps.
	//			I won't add this until later, its just a nice feature - since it would
	//			only flag issues a single time instead of multiple times.
	//	o	need better errors, with file & line number if available.

//
// DialectSetting
//

struct DialectSetting
{
public:
	DialectSetting(const opString& name, DialectSetting* parent) : Name(name), Parent(parent) {}
	
	virtual DialectNote* ToNote()
	{
		return NULL;
	}
	
	virtual DialectMap* ToMap()
	{
		return NULL;
	}
	
	virtual DialectEnumMap* ToEnumMap()
	{
		return NULL;
	}
	
	virtual DialectMapExpression* ToMapExpression()
	{
		return NULL;
	}
	
	virtual DialectLocation* ToLocation()
	{
		return NULL;
	}
	
	virtual DialectCategory* ToCategory()
	{
		return NULL;
	}
	
	DialectCategory* GetCategory()
	{
		DialectSetting* current = this;		
		while(current != NULL)
		{
			if(DialectCategory* category = current->ToCategory())
				return category;
			current = current->GetParent();
		}

		return NULL;
	}

	DialectLocation* GetLocation()
	{
		DialectSetting* current = this;		
		while(current != NULL)
		{
			if(DialectLocation* location = current->ToLocation())
				return location;
			current = current->GetParent();
		}
		
		return NULL;
	}

	DialectMap* GetMap()
	{
		DialectSetting* current = this;		
		while(current != NULL)
		{
			if(DialectMap* map = current->ToMap())
				return map;
			current = current->GetParent();
		}

		return NULL;
	}

	DialectEnumMap* GetEnumMap()
	{
		DialectSetting* current = this;		
		while(current != NULL)
		{
			if(DialectEnumMap* map = current->ToEnumMap())
				return map;
			current = current->GetParent();
		}
		
		return NULL;	
	}
	
	DialectSetting* GetParent()
	{
		return Parent;
	}
	
	void SetParent(DialectSetting* parent)
	{
		assert(Parent == NULL);
		Parent = parent;
	}
	
	const opString& GetName()
	{
		return Name;
	}
	
	opString GetPath()
	{
		opString settingpath;
		
		DialectSetting* current = this;
		while(current)
		{
			settingpath = current->GetName() + settingpath;

			current = current->GetParent();

			if(current)
				settingpath = "::" + settingpath;
		}

		return settingpath;
	}
	
private:
	DialectSetting* Parent;
	
	opString Name;
};

//
// DialectNoteBase
//

struct DialectNoteBase : DialectSetting
{
	DialectNoteBase(const opString& name, DialectSetting* parent) 
		: DialectSetting(name,parent)
	{}
	
	void ValidateArguments(DialectNote& note);
	
	virtual void PrintXml(opXmlStream& stream) = NULL;

	virtual void BuildValidArguments(vector<opString>& arguments);
	virtual void RegisterNote(NoteDefinitionNode* notenode) = NULL;

	//get a modifier's description
	virtual bool GetModifierDescription(const opString& name, opString& description);
	virtual bool GetArgumentDescription(const opString& name, opString& description);
};

//
// DialectMapExpression
//

struct DialectMapExpression
{
	DialectMapExpression(CriteriaNodeBase* expression) : Expression(expression)
	{
	}

	bool Evaluate(ModifierSupportBase* statement);

	bool Validate(DialectExpressionMap* map);

	void BuildValuedModifiers(opSet<opString>& modifiers);

	CriteriaNodeBase* GetExpression()
	{
		return Expression;
	}

private:

	CriteriaNodeBase* Expression;
};

//
// DialectExpressionMap
//

struct DialectExpressionMap : public DialectNoteBase
{
	DialectExpressionMap(const opString& name, DialectSetting* parent) : DialectNoteBase(name,parent)
	{}

	void				  BuildValuedModifiers(vector<opString>& modifiernames);

	bool				  IsMapped(ModifierSupportBase* statement);

	DialectMapExpression* RegisterCriteria(CriteriaNodeBase* node);

	bool ValidateExpressions();

	virtual bool HasValueModifier(const opString& name) = NULL;
	virtual bool HasBasicModifier(const opString& name) = NULL;

	virtual void GetModifiers(opArray<opString>& modifiers) = NULL;
	
private:
	opArray<DialectMapExpression> Expressions;
};

//
// DialectNote
//

struct DialectNote : public DialectExpressionMap
{
	DialectNote(const opString& name, DialectSetting* parent, NoteNode* node)
		: 
		DialectExpressionMap(name,parent),
		Node(node),
		Definition(NULL),
		bForceHidden(false)
	{}

	DialectNote(const opString& name, NoteNode* node)
		: 
		DialectExpressionMap(name,NULL),
		Node(node),
		Definition(NULL),
		bForceHidden(false)
	{}

	DialectNote* ToNote() { return this; }

	void RegisterNote(NoteDefinitionNode* notenode);
	void BuildValidArguments(vector<opString>& arguments);

	bool HasValueModifier(const opString& name);
	bool HasBasicModifier(const opString& name);
	void GetModifiers(opArray<opString>& modifiers);

	void SetNoteNode(NoteNode* node)				 { Node = node; }
	void SetNoteDefinition(NoteDefinitionNode* node) { Definition = node; }
	NoteNode* GetNoteNode()							 { return Node; }
	NoteDefinitionNode* GetNoteDefinition()			 { return Definition; }

	void PrintXml(opXmlStream& stream);

	void ForceHidden() { bForceHidden = true; }

	/**** validation ****/

	bool Validate(const LocationPath& path);

	/**** settings ****/

	virtual bool IsHidden();

private:
	NoteDefinitionNode* Definition;
	NoteNode* Node;
	bool bForceHidden;
};

//
// DialectNoParentNote
//

struct DialectNoParentNote : public DialectNote
{
	DialectNoParentNote(const opString& name, DialectSetting* parent, NoteNode* node)
		: 
		DialectNote(name,parent,node)
	{}

	DialectNoParentNote(const opString& name, NoteNode* node)
		: 
		DialectNote(name,NULL)
	{}

	void BuildValidArguments(vector<opString>& arguments);
};

//
// DialectEnumNote
//

struct DialectEnumNote : public DialectNote
{
	DialectEnumNote(const opString& name, DialectSetting* parent, NoteNode* node)
		: DialectNote(name,parent,node)
	{
	}

	DialectEnumNote(const opString& name, NoteNode* node)
		: DialectNote(name,NULL,node)
	{
	}
	
	bool HasBasicModifier(const opString& name);
	bool HasValueModifier(const opString& name);
	void GetModifiers(opArray<opString>& modifiers);

	void PrintXml(opXmlStream& stream);

	void BuildValidArguments(vector<opString>& arguments);
};

//
// DialectMapNote
//

struct DialectMapNote : public DialectNote
{
	enum NoteType
	{
		Start,
		Mapping,
		End,
	};

	DialectMapNote(const opString& name, NoteType type)
		: DialectNote(name, NULL, NULL), Type(type)
	{}

	void PrintXml(opXmlStream& stream);

	virtual void BuildValidArguments(vector<opString>& arguments);

	virtual bool IsHidden();
	
	virtual bool GetArgumentDescription(const opString& name, opString& description);
	virtual bool GetModifierDescription(const opString& name, opString& description);

private:
	NoteType	Type;
};

//
// DialectEnumMapNote
//

struct DialectEnumMapNote : public DialectEnumNote
{
	enum NoteType
	{
		//enum maps
		Start,
		Name,
		Value,
		End,
	};
	
	DialectEnumMapNote(const opString& name, NoteType type )
		: DialectEnumNote(name, NULL, NULL), Type(type)
	{}
	
	void PrintXml(opXmlStream& stream);

	virtual void BuildValidArguments(vector<opString>& arguments);
	
	virtual bool IsHidden();
	
private:
	NoteType		Type;
};

//
// DialectDisallow
//

struct DialectDisallow : public DialectExpressionMap
{
	DialectDisallow(const opString& name, DialectSetting* parent, DisallowNode* node) : DialectExpressionMap(name,parent), Node(node)
	{}
	
	void BuildValidArguments(vector<opString>& arguments) {}
	void RegisterNote(NoteDefinitionNode* notenode)       {}

	bool HasValueModifier(const opString& name);
	bool HasBasicModifier(const opString& name);
	void GetModifiers(opArray<opString>& modifiers);

	virtual bool GetModifierDescription(const opString& name, opString& description);

	void PrintXml(opXmlStream& stream) {}

	bool Validate()
	{
		if(!ValidateExpressions())
			return false;
		return true;
	}

	DisallowNode* GetNode()
	{
		return Node;
	}
	
private:
	DisallowNode* Node;
};

//
// DialectMap
//

struct DialectMap : public DialectExpressionMap
{
	DialectMap(const opString& name, DialectSetting* setting, CategoryMapNode* node) 
		: DialectExpressionMap(name, setting),
		StartNote("start",DialectMapNote::Start),
		MappingNote("mapping",DialectMapNote::Mapping),
		EndNote("end",DialectMapNote::End),
		Node(node)
	{
		StartNote.SetParent(this);
		MappingNote.SetParent(this);
		EndNote.SetParent(this);
	}

	enum MapMode
	{
		FunctionMap,
		DataMap,
	};
	
	DialectMap*  ToMap()  { return this; }

	void    SetMode(MapMode type) { Mode = type; }
	MapMode GetMode()             { return Mode; }
	
	CategoryMapNode* GetMapNode() { return Node; }

	void RegisterNote(NoteDefinitionNode* node);
	
	void BuildValidArguments(vector<opString>& arguments);
	
	NoteDefinitionNode* GetNoteNode(const opString& name);
	
	DialectNote&	GetStartNote()   { return StartNote; }
	DialectMapNote& GetMappingNote() { return MappingNote; }
	DialectNote&	GetEndNote()     { return EndNote; }
	
	/**** validation ****/
	
	bool Validate(const LocationPath& path);
	
	bool HasValueModifier(const opString& name);
	bool HasBasicModifier(const opString& name);
	void GetModifiers(opArray<opString>& modifiers);
	
	bool GetModifierDescription(const opString& name, opString& description);
	bool GetArgumentDescription(const opString& name, opString& description);

	void PrintXml(opXmlStream& stream);

	bool IsAllNotesEmpty();
	
private:

	void RegisterNote(NoteDefinitionNode* notenode, const opString& subtag);

	DialectMapNote	StartNote;
	DialectMapNote	MappingNote;
	DialectMapNote	EndNote;
	
	MapMode Mode;

	CategoryMapNode* Node;
};

//
// DialectEnumMap
//

struct DialectEnumMap : public DialectNoteBase
{
	DialectEnumMap(const opString& name, DialectSetting* parent, EnumerationMapNode* node) 
	: 
		DialectNoteBase(name,parent),
		Node(node),
		StartNote	("start",DialectEnumMapNote::Start),
		NameNote	("name",DialectEnumMapNote::Name),
		ValueNote	("value",DialectEnumMapNote::Value),
		EndNote		("end",DialectEnumMapNote::End)
	{
		StartNote.SetParent(this);
		NameNote.SetParent(this);
		ValueNote.SetParent(this);
		EndNote.SetParent(this);
	}

	DialectEnumMap* ToEnumMap() { return this; }

	bool Validate(const LocationPath& path);

	void RegisterMap();

	void BuildValidArguments(vector<opString>& arguments);

	void RegisterNote(NoteDefinitionNode* notenode);

	void PrintXml(opXmlStream& stream);

	EnumerationMapNode* GetMapNode()
	{
		return Node;
	}

	DialectEnumMapNote& GetStartNote() { return StartNote; }
	DialectEnumMapNote& GetNameNote()  { return NameNote; }
	DialectEnumMapNote& GetValueNote() { return ValueNote; }
	DialectEnumMapNote& GetEndNote()   { return EndNote; }

private:
	
	void RegisterNote(NoteDefinitionNode* notenode, const opString& subtag);

	DialectEnumMapNote StartNote;
	DialectEnumMapNote NameNote;
	DialectEnumMapNote ValueNote;
	DialectEnumMapNote EndNote;

	EnumerationMapNode* Node;
};

//
// DialectLocationBase
//

struct DialectLocationBase : DialectSetting
{
	DialectLocationBase(const opString& name, DialectSetting* parent) : DialectSetting(name,parent)
	{}

	/**** enumerations ****/

	enum Locations
	{
		invalid,
		body,
		post,
		heading,
		footer,
		source,
		sourceheading,
		sourcefooter
	};
	
	Locations GetId() { return Id; }

	DialectNoteBase* FindTag(const opString& name);

	//utility functions
	static opString ToString(Locations locid);
	static Locations ToId(const opString& locstring);
	
	bool SetId(const opString& locstring);
	
	virtual DialectNote* RegisterNote(NoteNode* node) = NULL;

	/*=== ordered note stuff.. ===*/

	typedef opList<DialectNoteBase*> OrderedNoteList;

	int GetNumOrderedNotes()
	{
		return OrderedNotes.Size();
	}

	OrderedNoteList& GetOrderedNotes()
	{
		return OrderedNotes;
	}

	OrderedNoteList::iterator GetPosition(const opString& notename);
	void InsertOrdered(DialectNoteBase* node, OrderModifierNodeBase* order);
	
protected:
	//order of notes		
	OrderedNoteList OrderedNotes;

private:
	//location type
	Locations Id;
};

//
// DialectLocation
//

struct DialectLocation : public DialectLocationBase
{
	DialectLocation(const opString& name, DialectSetting* parent) : DialectLocationBase(name,parent)
	{}
	
	/**** registration ****/
	
	//registration functions, should be registered in order.
	DialectMap*  RegisterMap(CategoryMapNode* node);
	
	//register note definition
	DialectMap*  GetMap(const opString& name);
	
	//note find
	DialectMap*  FindMap(const opString& name);
	
	void RegisterNote(NoteDefinitionNode* node);
	DialectNote* RegisterNote(NoteNode* node);
	DialectNote* GetNote(const opString& name);
	DialectNote* FindNote(const opString& name);
	
	/**** validation ****/
	
	bool Validate(const TypePath& path);
	
	DialectLocation* ToLocation() { return this; }

private:
	opArray<DialectMap*>  Maps;

	//storage of notes/maps
	opArray<DialectNote*> Notes;
};

//
// DialectEnumLocation
//

struct DialectEnumLocation : public DialectLocationBase
{
	DialectEnumLocation(const opString& name, DialectSetting* parent) : DialectLocationBase(name, parent) 
	{}
	
	void		 RegisterNote(NoteDefinitionNode* node);
	DialectNote* RegisterNote(NoteNode* node);

	DialectEnumNote* GetNote(const opString& name);
	DialectEnumNote* FindNote(const opString& name);

	DialectEnumMap* RegisterEnumMap(EnumerationMapNode* node);
	DialectEnumMap* GetEnumMap(const opString& name);

	bool Validate(const TypePath& path);

private:
	opArray<DialectEnumMap*> Maps;

	//storage of notes/maps
	opArray<DialectEnumNote*> Notes;
};

//
// DialectTypeBase
//

struct DialectTypeBase : DialectSetting
{
	DialectTypeBase(const opString& name);
	~DialectTypeBase();
	
	DialectDisallow* RegisterDisallow(DisallowNode* node);
	
	DialectDisallow* FindDisallow(const opString& name);
	DialectDisallow* GetDisallow(int index) { return Disallows[index]; }
	int			     GetNumDisallows()		{ return Disallows.Size(); }

	bool ValidateNote(DialectNote& note);

	virtual void RegisterNote(NoteDefinitionNode* notenode) = NULL;
	
protected:
	
	NoteDefinitionNode* GenerateDefaultNote(opString category, opString tag);
	
	virtual bool ValidateSpecialNotes() = NULL;
	virtual opNode* GetTypeNode() = NULL;
	
	opArray<DialectDisallow*> Disallows;
};

//
// DialectCategory
//

struct DialectCategory : public DialectTypeBase
{
	DialectCategory(const opString& name, CategoryNode* node);
	~DialectCategory();

	DialectCategory* ToCategory() { return this; }

	/**** registrations ****/
	
	void			 RegisterDataModifier(DataModifierNode* node);
	void			 RegisterFunctionModifier(FunctionModifierNode* node);
		
	void			 RegisterNote(NoteDefinitionNode* node);

	DialectLocation* RegisterLocation(CategoryLocationNode* node);

	/**** queries ****/

	DialectLocation* FindLocation(DialectLocation::Locations locationid);
	
	DataModifierNode*	  FindDataModifier(const opString& name);
	FunctionModifierNode* FindFunctionModifier(const opString& name);
	bool			      HasBasicModifier(const opString& name);
	
	DataModifierNode*	  FindValueDataModifier(const opString& name);
	FunctionModifierNode* FindValueFunctionModifier(const opString& name);
	bool				  HasValueModifier(const opString& name);
	
	void GetDataModifiers(opArray<opString>& modifiernames);
	void GetFunctionModifiers(opArray<opString>& modifiernames);

	void GetBasicDataModifiers(opArray<opString>& modifiers);
	void GetBasicFunctionModifiers(opArray<opString>& modifiers);
	void GetValueDataModifiers(opArray<opString>& modifiers);
	void GetValueFunctionModifiers(opArray<opString>& modifiers);
	
	/**** validation ****/
	
	bool Validate();
	bool ValidateSpecialNotes();
	
	/**** get ****/
	DialectNote& GetIdentifierNote() { return IdentifierNote; }
	DialectNote& GetParentNote()	 { return ParentNote; }
	DialectNote& GetNoParentNote()	 { return NoParentNote; }
	DialectNote& GetPrefixNote()	 { return PrefixNote; }
	DialectNote& GetPostfixNote()	 { return PostfixNote; }
	DialectNote& GetDefaultNote()    { return DefaultNote; }

	PrefixNodeBase* GetPrefix()                       { return Prefix;  }
	void            SetPrefix(PrefixNodeBase* innode) { Prefix = innode; }

private:

	opNode* GetTypeNode();
	
	/**** internal functions ****/
	
	bool RegisterSpecialNote(NoteDefinitionNode* node, const opString& locationname);
	void RegisteredNote(NoteDefinitionNode*& noderef, NoteDefinitionNode* node);
	
	void GenerateDefaultNotes();
	
	/**** data ****/	
	
	//node
	CategoryNode* Node;
	
	//locations
	opArray<DialectLocation*> Locations;

	//data modifiers
	typedef opMap<opString,DataModifierNode*>	DataModifierMap;
	DataModifierMap								DataModifiers;
	DataModifierMap								ValueDataModifiers;
	
	//function modifiers
	typedef opMap<opString,FunctionModifierNode*>	FunctionModifierMap;
	FunctionModifierMap								FunctionModifiers;
	FunctionModifierMap								ValueFunctionModifiers;
	
	//category note definitions
	DialectNote			IdentifierNote;
	DialectNote			ParentNote;
	DialectNoParentNote NoParentNote;
	DialectNote			PrefixNote;
	DialectNote			PostfixNote;
	DialectNote			DefaultNote;

	NoteDefinitionNode* DefaultIdentifierNote;
	NoteDefinitionNode* DefaultParentNote;
	NoteDefinitionNode* DefaultNoParentNote;
	NoteDefinitionNode* DefaultPrefixNote;
	NoteDefinitionNode* DefaultPostfixNote;
	NoteDefinitionNode* DefaultDefaultNote;

	PrefixNodeBase* Prefix;
};

//
// DialectEnumeration
//

struct DialectEnumeration : public DialectTypeBase
{
	DialectEnumeration(const opString& name, EnumerationNode* node);
	~DialectEnumeration();	

	DialectEnumLocation* RegisterLocation(EnumerationLocationNode* node);
	DialectEnumLocation* FindLocation(DialectLocation::Locations locationid);

	bool Validate();
	virtual bool ValidateSpecialNotes();

	void RegisterNote(NoteDefinitionNode* notenode);
	bool RegisterSpecialNote(NoteDefinitionNode* node, const opString& locationname);
	
	DialectNote& GetIdentifierNote() { return IdentifierNote; }
	DialectNote& GetParentNote()	 { return ParentNote; }

	PrefixNodeBase* GetPrefix()                       { return Prefix;   }
	void            SetPrefix(PrefixNodeBase* innode) { Prefix = innode; }

private:
	opNode* GetTypeNode();
	
	void GenerateDefaultNotes();
		
	opString		 Name;
	EnumerationNode* Node;

	opArray<DialectEnumLocation*> Locations;

	DialectEnumNote IdentifierNote;
	DialectEnumNote ParentNote;

	NoteDefinitionNode* DefaultIdentifierNote;
	NoteDefinitionNode* DefaultParentNote;

	PrefixNodeBase* Prefix;
};

//
// DialectFileDeclaration
//

struct DialectFileDeclaration
{
	/*=== construction ===*/

	DialectFileDeclaration(const opString& name)
		: Name(name),
		  FilenameNote(NULL)
	{

	}

	/*=== utility ===*/

	const opString& GetName()
	{
		return Name;
	}

	NoteDefinitionNode* GetFilenameNote()
	{
		return FilenameNote;
	}

	void                         MergeLocations(FileDeclarationNode* node);
	FileDeclarationLocationNode* FindLocation(const opString& name);
	bool                         LocationExists(const opString& name);
	void            	         RegisterNote(NoteDefinitionNode* notenode);

private:

	/*=== private utility ===*/

	void AddLocationBefore(FileDeclarationLocationNode* node, opString& before);
	void AddLocationAfter(FileDeclarationLocationNode* node, opString& after);

	/*=== data ===*/

	opString                             Name;
	NoteDefinitionNode*                  FilenameNote;
	opList<FileDeclarationLocationNode*> Locations;
};

///
/// Dialect Tracker 
///

class DialectTracker
{
public:
	
	/**** static registration ****/
	
	static DialectCategory*		Register(CategoryNode* node);
	static DialectEnumeration*	Register(EnumerationNode* node);
	static bool					Register(const opString& name, opNode* node);
	static void      		    Register(NoteDefinitionNode* notenode);
	static void                 Register(ExtensionNode* node);
	static void                 Register(ExtensionPointNode* node);
	static void                 Register(ExtendPointNode* node);
	static void                 Register(FileDeclarationNode* node);
	
	/**** static queries ****/
	
	static DialectCategory*		   GetCategory(const opString& name);
	static DialectEnumeration*	   GetEnumeration(const opString& name);
	static DialectTypeBase*		   GetType(const opString& name);
	static ExtensionNode*          GetExtension(const opString& name);
	static DialectFileDeclaration* GetFileDeclaration(const opString& name);
	static CategoryNode*           GetAltClassPrefix(const opString& prefix);
	static CategoryNode*           GetAltStructPrefix(const opString& prefix);
	static EnumerationNode*        GetAltEnumerationPrefix(const opString& prefix);
	static const opSet<opString>&  GetAllPrefixes();

	/*=== get entire data structures ===*/

	static const opMap<opString, ExtensionNode*>& GetExtensions();
	
	/**** note queries ****/
	
	static NoteDefinitionNode* GetNote(const opString& category, DialectLocation::Locations location, const opString& tag);
	static NoteDefinitionNode* GetNote(const opString& category, DialectLocation::Locations location, const opString& tag, const opString& subtag);
	
	/**** validation ****/
	
	static bool Validate();
	
private:
	
	/**** typedefs ****/

	typedef opHashTable<opString,DialectCategory*>::iterator	categoryiterator;
	typedef opHashTable<opString,DialectEnumeration*>::iterator	enumerationiterator;
	typedef opHashTable<opString,opNode*>::iterator				globaliterator;
	
	/**** internal functions ****/
	void                RegisterExtension(ExtensionNode* node);
	void                RegisterExtensionPoint(ExtensionPointNode* node);
	void                RegisterExtendPoint(ExtendPointNode* node);
	void                RegisterFileDeclaration(FileDeclarationNode* node);
	void                CheckForDuplicateFileDeclarationLocations(FileDeclarationNode* node);
	DialectCategory*	RegisterCategory(CategoryNode* node);
	DialectEnumeration* RegisterEnumeration(EnumerationNode* node);
	void            	RegisterNote(NoteDefinitionNode* notenode);
	bool				ValidateNotes();
	
	//RegisterGlobal
	//register a global name (to check for collisions of different types)
	//throws an error if collided, returns true if ok, false if already registered
	bool RegisterGlobal(const opString& name, opNode* node);
	
	/**** registration data ****/
	
	//category registration
	opHashTable<opString,DialectCategory*>	 CategoryNodes;
	opHashTable<opString,DialectEnumeration*> EnumerationNodes;

	//global registration
	opHashTable<opString,opNode*> GlobalNodes;

	// extension nodes
	opMap<opString, ExtensionNode*> ExtensionNodes;

	// file declarations
	opMap<opString, FileDeclarationLocationNode*> FileDeclarationLocations;
	opMap<opString, DialectFileDeclaration*>      FileDeclarationNodes;

	// alt mappings
	opMap<opString, CategoryNode*>    AltClassMap;
	opMap<opString, CategoryNode*>    AltStructMap;
	opMap<opString, EnumerationNode*> AltEnumMap;
	opSet<opString>                   Prefixes;

	/**** creation, instance ****/

	DialectTracker();
	~DialectTracker();
	
	static DialectTracker& GetInstance()
	{
		return Instance;
	}
	
	static DialectTracker Instance;
};

//
// Dialect Paths
//

//TODO: these are ugly and pointless, get rid of them
struct TypePath
{
	TypePath(DialectTypeBase* type)
		: Type(type)
	{}

	DialectTypeBase* Type;
};

struct LocationPath
{
	LocationPath(const TypePath& path, DialectLocationBase* location)
		: Type(path.Type), Location(location)
	{
	}

	DialectTypeBase* Type;
	DialectLocationBase* Location;
};

struct TagPath
{
	TagPath(const LocationPath& path, DialectNoteBase* notebase)
		: Type(path.Type), Location(path.Location), Note(notebase)
	{
	}

	opString GetPath()
	{
		opString s;
		s += Type->GetName();
		s += "::";
		s += Location->GetName();
		s += "::";
		s += Note->GetName();
		return s;
	}
	
	DialectTypeBase*	 Type;
	DialectLocationBase* Location;
	DialectNoteBase*	 Note;
};

struct SubTagPath
{
	SubTagPath(const TagPath& path, const opString& subtag)
		: Type(path.Type), Location(path.Location), Map(path.Note), SubTag(subtag)
	{}

	opString GetPath()
	{
		opString s;
		s += Type->GetName();
		s += "::";
		s += Location->GetName();
		s += "::";
		s += Map->GetName();
		s += "::";
		s += SubTag;
		return s;
	}

	DialectTypeBase*	 Type;
	DialectLocationBase* Location;
	DialectNoteBase*	 Map;
	opString			 SubTag;
};

}// end of namespace dialect