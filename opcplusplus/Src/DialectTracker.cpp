///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: DialectTracker.cpp
/// Date: 03/29/2007
///
/// Description:
///
/// Dialect Tracker Source
///****************************************************************

#include "opCPP.h"

///
/// Dialect Tracker
///

DialectTracker DialectTracker::Instance;

DialectTracker::DialectTracker()
{}

DialectTracker::~DialectTracker()
{
	//delete all categories
	CategoryNodes.DeleteAllValues();	
	
	//delete all enumerations
	EnumerationNodes.DeleteAllValues();

	//delete all file declarations
	FileDeclarationNodes.DeleteAllValues();
}

DialectCategory::DialectCategory(const opString& name, CategoryNode* node)
:
	DialectTypeBase(name),
	Node(node),
	IdentifierNote("identifier",NULL),
	ParentNote("parent",NULL),
	NoParentNote("noparent",NULL),
	PrefixNote("prefix",NULL),
	PostfixNote("postfix",NULL),
	DefaultNote("default",NULL),
	Prefix(NULL)
{
	IdentifierNote.SetParent(this);
	IdentifierNote.ForceHidden();
	ParentNote.SetParent(this);
	NoParentNote.SetParent(this);
	PrefixNote.SetParent(this);
	PostfixNote.SetParent(this);
	DefaultNote.SetParent(this);
}

DialectEnumeration::DialectEnumeration(const opString& name, EnumerationNode* node)
: 
	DialectTypeBase(name), 
	Node(node),
	IdentifierNote("identifier",NULL),
	ParentNote("parent",NULL),
	Prefix(NULL)
{
	IdentifierNote.SetParent(this);
	IdentifierNote.ForceHidden();
	ParentNote.SetParent(this);
}

DialectTypeBase::DialectTypeBase(const opString& name)
: 
	DialectSetting(name, NULL)
{
}

DialectEnumeration::~DialectEnumeration()
{
	Locations.DeleteAll();
}

DialectCategory::~DialectCategory()
{
	Locations.DeleteAll();
}

DialectTypeBase::~DialectTypeBase()
{
	Disallows.DeleteAll();	
}

//
// DialectTracker
//

// register a category
DialectCategory* DialectTracker::Register(CategoryNode* node)
{
	return GetInstance().RegisterCategory(node);
}

// register an enumeration
DialectEnumeration* DialectTracker::Register(EnumerationNode* node)
{
	return GetInstance().RegisterEnumeration(node);
}

// register an extension
void DialectTracker::Register(ExtensionNode* node)
{
	GetInstance().RegisterExtension(node);
}

// register an extensionpoint
void DialectTracker::Register(ExtensionPointNode* node)
{
	GetInstance().RegisterExtensionPoint(node);
}

// register an extendpoint
void DialectTracker::Register(ExtendPointNode* node)
{
	GetInstance().RegisterExtendPoint(node);
}

// register a file declaration
void DialectTracker::Register(FileDeclarationNode* node)
{
	GetInstance().RegisterFileDeclaration(node);
}

opNode* DialectCategory::GetTypeNode()
{
	return Node;
}

opNode* DialectEnumeration::GetTypeNode()
{
	return Node;
}

// register a category
DialectCategory* DialectTracker::RegisterCategory(CategoryNode* node)
{
	opString         name = node->GetName()->GetValue();
	categoryiterator it   = CategoryNodes.Find(name);
	categoryiterator end  = CategoryNodes.End();
	
	/*=== If the category is not yet registered, register it. ===*/

	if (it == end)
	{
		RegisterGlobal(name,node);
		
		it = CategoryNodes.Insert(name,new DialectCategory(name,node));
	}

	/*=== Check for alt prefixes. ===*/

	{
		// Check if we have a class prefix.
		if ( ClassPrefixNode* prefix = node->GetClassPrefix() )
		{	 
			opString      prefixstring = prefix->GetPrefix()->GetValue();
			CategoryNode* old;
			opString      name;

			// If we've already registered a prefix with this 
			// category, throw an error.
			if ( PrefixNodeBase* oldprefix = (*it).second->GetPrefix() )
			{
				opError::AltPrefixRedefinitionError("category", oldprefix, prefix);
			}
			// If this class prefix already exists, throw an error.
			else if ( AltClassMap.Find( prefixstring, old ) )		
			{
				ClassPrefixNode* oldprefix = old->GetClassPrefix();

				prefix->PrintString(name);

				opError::RedefinitionError(name, oldprefix, prefix);
			}
			// Register this prefix.
			else
			{
				RegisterGlobal(prefixstring, prefix);

				AltClassMap[ prefixstring ] = node;

				(*it).second->SetPrefix( prefix );

				Prefixes.Insert( prefixstring );
			}
		}
		// Check if we have a struct prefix.
		else if ( StructPrefixNode* prefix = node->GetStructPrefix() )
		{
			opString      prefixstring = prefix->GetPrefix()->GetValue();
			CategoryNode* old;
			opString      name;

			// If we've already registered a prefix with this 
			// category, throw an error.
			if ( PrefixNodeBase* oldprefix = (*it).second->GetPrefix() )
			{
				opError::AltPrefixRedefinitionError("category", oldprefix, prefix);
			}
			// If this class prefix already exists, throw an error.
			else if ( AltStructMap.Find( prefixstring, old ) )		
			{
				StructPrefixNode* oldprefix = old->GetStructPrefix();

				prefix->PrintString(name);

				opError::RedefinitionError(name, oldprefix, prefix);
			}
			// Register this prefix.
			else
			{
				RegisterGlobal(prefixstring, prefix);

				AltStructMap[ prefixstring ] = node;

				(*it).second->SetPrefix( prefix );

				Prefixes.Insert( prefixstring );
			}
		}
	}
	
	return (*it).second;
}

// register an enumeration
DialectEnumeration* DialectTracker::RegisterEnumeration(EnumerationNode* node)
{
	opString            name = node->GetName()->GetValue();
	enumerationiterator it   = EnumerationNodes.Find(name);
	enumerationiterator end  = EnumerationNodes.End();

	/*=== If the enumeration is not yet registered, register it. ===*/

	if (it == end)
	{
		RegisterGlobal(name,node);

		it = EnumerationNodes.Insert(name,new DialectEnumeration(name,node));
	}

	/*=== Check for alt prefixes. ===*/

	{
		// Check if we have an enumeration prefix.
		if ( EnumPrefixNode* prefix = node->GetEnumPrefix() )
		{
			opString         prefixstring = prefix->GetPrefix()->GetValue();
			EnumerationNode* old;
			opString         name;

			// If we've already registered a prefix with this 
			// category, throw an error.
			if ( PrefixNodeBase* oldprefix = (*it).second->GetPrefix() )
			{
				opError::AltPrefixRedefinitionError("enumeration", oldprefix, prefix);
			}
			// If this class prefix already exists, throw an error.
			else if ( AltEnumMap.Find( prefixstring, old ) )		
			{
				EnumPrefixNode* oldprefix = old->GetEnumPrefix();

				prefix->PrintString(name);

				opError::RedefinitionError(name, oldprefix, prefix);
			}
			// Register this prefix.
			else
			{
				RegisterGlobal(prefixstring, prefix);

				AltEnumMap[ prefixstring ] = node;

				(*it).second->SetPrefix( prefix );

				Prefixes.Insert( prefixstring );
			}
		}
	}

	return (*it).second;
}

// register an extension
void DialectTracker::RegisterExtension(ExtensionNode* node)
{
	ExtensionNode* existing = NULL;
	opString       name     = node->GetName()->GetValue();

	// if there's a duplicate, throw an error..
	if ( ExtensionNodes.Find(name, existing) )
	{
		opError::RedefinitionError(name, existing, node);
	}

	ExtensionNodes.Insert(name, node);
}

// register an extension point
void DialectTracker::RegisterExtensionPoint(ExtensionPointNode* node)
{
	opString       name = node->GetName()->GetValue();
	ExtensionNode* ext  = GetExtension( name );

	if ( !ext )
	{
		opError::InvalidExtensionError(node, name, "extensionpoint");
	}
}

// register an extend point
void DialectTracker::RegisterExtendPoint(ExtendPointNode* node)
{
	ExtensionNode* existing = NULL;
	opString       name     = node->GetName()->GetValue();

	if ( !ExtensionNodes.Find(name, existing) )
	{
		opError::InvalidExtensionError(node, name, "extendpoint");
	}

	existing->AddExtendPoint(node);
}

// get a category
DialectCategory* DialectTracker::GetCategory(const opString& name)
{
	categoryiterator it  = GetInstance().CategoryNodes.Find(name);
	categoryiterator end = GetInstance().CategoryNodes.End();
	
	if(it == end)
		return NULL;
	
	return (*it).second;
}

// get an enumeration
DialectEnumeration* DialectTracker::GetEnumeration(const opString& name)
{
	enumerationiterator it = GetInstance().EnumerationNodes.Find(name);
	enumerationiterator end = GetInstance().EnumerationNodes.End();

	if(it == end)
		return NULL;

	return (*it).second;
}

// get an extension
ExtensionNode* DialectTracker::GetExtension(const opString& name)
{
	ExtensionNode* node = NULL;

	GetInstance().ExtensionNodes.Find(name, node);

	return node;
}

// get a file declaration
DialectFileDeclaration* DialectTracker::GetFileDeclaration(const opString& name)
{
	DialectFileDeclaration* node = NULL;

	GetInstance().FileDeclarationNodes.Find(name, node);

	return node;
}

// get a type
DialectTypeBase* DialectTracker::GetType(const opString& name)
{
	DialectTypeBase* category = GetCategory(name);
	if(category)
		return category;
	
	DialectTypeBase* enumeration = GetEnumeration(name);
	return enumeration;
}

bool DialectTracker::Register(const opString& name, opNode* node)
{
	return GetInstance().RegisterGlobal(name,node);
}

// NOTE: It only makes sense to call this function on category, enumeration,
//       prefix, datamodifier and functionmodifier nodes!
bool DialectTracker::RegisterGlobal(const opString& name, opNode* node)
{
	globaliterator it  = GlobalNodes.Find(name);
	globaliterator end = GlobalNodes.End();
	
	// add it, return true
	if (it == end)
	{
		GlobalNodes.Insert(name,node);

		return true;
	}
	
	opNode* existing = (*it).second;
	Token   oldid    = existing->GetId();
	Token   newid    = node->GetId();

	// NOTE: Modifier validation happens elsewhere.  We do not want to error here if
	//       modifiers collide.
	if      ( (oldid == G_DATAMODIFIER || oldid == G_FUNCTIONMODIFIER)
	&&        (newid == G_DATAMODIFIER || newid == G_FUNCTIONMODIFIER) );
	// NOTE: Prefixes have been validated at this point.  We want to allow prefixes of different
	//       types to have the same name.
	else if ( (oldid == G_CLASS_PREFIX || oldid == G_STRUCT_PREFIX || oldid == G_ENUM_PREFIX) 
	&&        (newid == G_CLASS_PREFIX || newid == G_STRUCT_PREFIX || newid == G_ENUM_PREFIX) );
	// Otherwise error.
	else
	{
		opError::NameCollisionError(name,existing,node);
	}
	
	return false;
}

void DialectTracker::Register(NoteDefinitionNode* notenode)
{
	GetInstance().RegisterNote(notenode);
}

void DialectCategory::RegisteredNote(NoteDefinitionNode*& noderef, NoteDefinitionNode* node)
{
	noderef = node;
	
	opString pathstring;
	node->GetPath()->PrintString(pathstring);
	node->GetArguments()->PrintString(pathstring);

	if(opParameters::Get().Diagnostics)
		Log(opString("Registered Note Definition : ") + pathstring);
}

bool DialectCategory::RegisterSpecialNote(NoteDefinitionNode* node, const opString& locationname)
{
	if(IdentifierNote.GetName() == locationname)
	{
		IdentifierNote.RegisterNote(node);
		return true;
	}
	else if(ParentNote.GetName() == locationname)
	{
		ParentNote.RegisterNote(node);
		return true;
	}
	else if(NoParentNote.GetName() == locationname)
	{
		NoParentNote.RegisterNote(node);
		return true;
	}
	else if(PrefixNote.GetName() == locationname)
	{
		PrefixNote.RegisterNote(node);
		return true;
	}
	else if(PostfixNote.GetName() == locationname)
	{
		PostfixNote.RegisterNote(node);
		return true;
	}
	else if(DefaultNote.GetName() == locationname)
	{
		DefaultNote.RegisterNote(node);
		return true;
	}
	
	return false;
}

bool DialectEnumeration::RegisterSpecialNote(NoteDefinitionNode* node, const opString& locationname)
{
	if(IdentifierNote.GetName() == locationname)
	{
		IdentifierNote.RegisterNote(node);
		return true;
	}
	else if(ParentNote.GetName() == locationname)
	{
		ParentNote.RegisterNote(node);
		return true;
	}

	return false;
}

void DialectCategory::RegisterNote(NoteDefinitionNode* node)
{
	//next get the location name string
	opString locationname;
	bool haslocation = node->GetPathLocation(locationname);
	
	if(!haslocation)
	{
		opError::MessageError(node,"Note definition is missing location specifier.");
	}

	if(RegisterSpecialNote(node,locationname))
		return;

	DialectLocation* location = FindLocation(DialectLocation::ToId(locationname));

	if(!location)
	{
		opError::MessageError(node,"Note definition location '" + locationname + "' is invalid.");
	}

	location->RegisterNote(node);
}


void DialectEnumeration::RegisterNote(NoteDefinitionNode* node)
{
	//next get the location name string
	opString locationname;
	bool haslocation = node->GetPathLocation(locationname);

	if(!haslocation)
	{
		opError::MessageError(node,"Note definition is missing location specifier.");
	}

	if(RegisterSpecialNote(node,locationname))
		return;

	DialectEnumLocation* location = FindLocation(DialectLocation::ToId(locationname));

	if(!location)
	{
		opError::MessageError(node,"Note definition location '" + locationname + "' is invalid.");
	}

	location->RegisterNote(node);
}

void DialectLocation::RegisterNote(NoteDefinitionNode* node)
{
	//next get the tag name string
	opString tagname;
	bool hastag = node->GetPathTag(tagname);

	if(!hastag)
	{
		opError::MessageError(node,"Note definition is missing tag specifier.");
	}

	DialectNoteBase* tag = FindTag(tagname);

	if(!tag)
	{
		opError::MessageError(node,"Note definition tag '" + tagname + "' is invalid.");
	}

	tag->RegisterNote(node);
}

void DialectEnumLocation::RegisterNote(NoteDefinitionNode* node)
{
	//next get the tag name string
	opString tagname;
	bool hastag = node->GetPathTag(tagname);

	if(!hastag)
	{
		opError::MessageError(node,"Note definition is missing tag specifier.");
	}

	DialectNoteBase* tag = FindTag(tagname);

	if(!tag)
	{
		opError::MessageError(node,"Note definition tag '" + tagname + "' is invalid.");
	}

	tag->RegisterNote(node);
}

void DialectMap::RegisterNote(NoteDefinitionNode* node)
{
	//next get the optional sub-tag name string
	opString subtagname;
	bool hassubtag = node->GetPathSubTag(subtagname);

	if(!hassubtag)
	{
		//error, no sub-tag available
		//TODO: this error could be improved
		opError::MessageError(node,"Map note definition is missing sub-tag specifier.");
	}

	//check if the
	//path is done  
	if(node->GetPath()->GetScopeNum() > 4)
	{
		//error, too many elements
		opError::MessageError(node,"Map note definition has too many tag specifiers.");
	}

	//register with the map
	RegisterNote(node,subtagname);
}

bool DialectMap::IsAllNotesEmpty()
{
	if(NoteDefinitionNode* node = StartNote.GetNoteDefinition())
		if(!node->IsBodyEmpty())
			return false;
	if(NoteDefinitionNode* node = MappingNote.GetNoteDefinition())
		if(!node->IsBodyEmpty())
			return false;
	if(NoteDefinitionNode* node = EndNote.GetNoteDefinition())
		if(!node->IsBodyEmpty())
			return false;

	return true;
}

void DialectEnumMap::RegisterNote(NoteDefinitionNode* node)
{
	//next get the optional sub-tag name string
	opString subtagname;
	bool hassubtag = node->GetPathSubTag(subtagname);

	if(!hassubtag)
	{
		//error, no sub-tag available
		//TODO: this error could be improved
		opError::MessageError(node,"Map note definition is missing sub-tag specifier.");
	}

	//check if the
	//path is done  
	if(node->GetPath()->GetScopeNum() > 4)
	{
		//error, too many elements
		opError::MessageError(node,"Map note definition has too many tag specifiers.");
	}

	//register with the map
	RegisterNote(node,subtagname);
}

void DialectTracker::RegisterNote(NoteDefinitionNode* notenode)
{
	/*=== Get the first ID of the note path. ===*/

	opString specifier;
	
	if ( !notenode->GetPathType(specifier) )
	{
		// Missing first ID in note path.
		opError::MessageError(notenode, "Note definition is missing initial specifier.");
	}

	/*=== Now, we want to register the note in the correct place. ===*/
	/*=== It's either a file, category or enumeration.            ===*/

	// CASE 1: file
	if ( DialectFileDeclaration* filedecl = GetFileDeclaration(specifier) )
	{
		filedecl->RegisterNote(notenode);
	}
	// CASE 2: category/enumeration/etc.
	else if ( DialectTypeBase* notetype = GetType(specifier) )
	{
		notetype->RegisterNote(notenode);
	}
	else
		opError::MessageError(notenode, "Note definition specifier '" + specifier + "' is invalid.");
}

//
// Local Registration Functions
//

DialectLocation* 
DialectCategory::RegisterLocation(CategoryLocationNode* node)
{	
	opString locationname = node->GetName()->GetStringValue();
	DialectLocation::Locations Id = DialectLocation::ToId(locationname);
	
	//find existing location
	int num = Locations.Size();
	for(int i = 0; i < num; i++)
	{
		if(Locations[i]->GetId() == Id)
			return Locations[i];
	}
	
	//create new location
	DialectLocation* l = new DialectLocation(locationname,this);
	
	if(!l->SetId(locationname))
	{
		opError::MessageError(node,"location '" + locationname + "' is invalid: use 'body' OR 'heading' OR 'footer' OR 'source'.");
	}
	
	Locations.push_back(l);
	
	return l;
}

DialectEnumLocation* 
DialectEnumeration::RegisterLocation(EnumerationLocationNode* node)
{	
	opString locationname = node->GetName()->GetStringValue();
	DialectLocation::Locations Id = DialectLocation::ToId(locationname);

	//find existing location
	int num = Locations.Size();
	for(int i = 0; i < num; i++)
	{
		if(Locations[i]->GetId() == Id)
			return Locations[i];
	}

	//create new location
	DialectEnumLocation* l = new DialectEnumLocation(locationname,this);

	if(!l->SetId(locationname))
	{
		opError::MessageError(node,"location '" + locationname + "' is invalid: use 'body' OR 'heading' OR 'footer' OR 'source'.");
	}

	Locations.push_back(l);

	return l;
}

opString DialectLocationBase::ToString(Locations locid)
{
	if (locid == body)
		return "body";
	else if (locid == heading)
		return "heading";
	else if (locid == post)
		return "post";
	else if (locid == footer)
		return "footer";
	else if (locid == source)
		return "source";
	else if (locid == sourceheading)
		return "sourceheading";
	else if (locid == sourcefooter)
		return "sourcefooter";

	return "invalid";
}

DialectLocation::Locations
DialectLocationBase::ToId(const opString& locstring)
{
	if (locstring == "body")
		return body;
	else if (locstring == "heading")
		return heading;
	else if (locstring == "post")
		return post;
	else if (locstring == "footer")
		return footer;
	else if (locstring == "source")
		return source;
	else if (locstring == "sourceheading")
		return sourceheading;
	else if (locstring == "sourcefooter")
		return sourcefooter;

	return invalid;
}

bool DialectLocationBase::SetId(const opString& locstring)
{
	Id = ToId(locstring);

	if(Id == invalid)
		return false;
	return true;
}

void DialectCategory::RegisterFunctionModifier(FunctionModifierNode* node)
{
	opString name = node->GetName()->GetValue();

	if(node->GetArguments())
	{
		if (FunctionModifierNode* existing = FindValueFunctionModifier(name))
		{
			opString name;

			node->PrintString(name);

			opError::RedefinitionError(name, existing, node);
		}
		else
		{
			DialectTracker::Register(name,node);
			ValueFunctionModifiers.Insert(name,node);
		}
	}
	else
	{
		if (FunctionModifierNode* existing = FindFunctionModifier(name))
		{
			opString name;

			node->PrintString(name);

			opError::RedefinitionError(name, existing, node);
		}
		else
		{
			//TODO: oops, need to fix the registration, it only needs to check
			//		categories against modifiers (same named data/function are allowed)
			DialectTracker::Register(name,node);
			FunctionModifiers.Insert(name,node);
		}
	}
}

void DialectCategory::RegisterDataModifier(DataModifierNode* node)
{
	opString name = node->GetName()->GetValue();
	
	if(node->GetArguments())
	{
		if(DataModifierNode* existing = FindValueDataModifier(name))
		{
			opString name;

			node->PrintString(name);

			opError::RedefinitionError(name, existing, node);
		}
		else
		{
			DialectTracker::Register(name,node);
			ValueDataModifiers.Insert(name,node);
		}
	}
	else
	{
		if(DataModifierNode* existing = FindDataModifier(name))
		{
			opString name;

			node->PrintString(name);

			opError::RedefinitionError(name, existing, node);
		}
		else
		{
			DialectTracker::Register(name,node);
			DataModifiers.Insert(name,node);
		}
	}
}

DialectDisallow* DialectTypeBase::RegisterDisallow(DisallowNode* node)
{
	opString name = node->GetName()->GetValue();
	
	if( DialectDisallow* existing = FindDisallow(name) )
	{
		opString name;

		node->PrintString(name);

		opError::RedefinitionError(name, existing->GetNode(), node);
	}
	
	DialectDisallow* d = new DialectDisallow(name,this,node);
	
	Disallows.push_back(d);
	
	return d;
}

DataModifierNode* DialectCategory::FindDataModifier(const opString& name)
{
	DataModifierMap::iterator it = DataModifiers.Find(name);
	DataModifierMap::iterator end = DataModifiers.end();

	if(it != end)
		return it->second;
	return NULL;
}

FunctionModifierNode*	DialectCategory::FindFunctionModifier(const opString& name)
{
	FunctionModifierMap::iterator it = FunctionModifiers.Find(name);
	FunctionModifierMap::iterator end = FunctionModifiers.end();

	if(it != end)
		return it->second;
	return NULL;
}

DataModifierNode* DialectCategory::FindValueDataModifier(const opString& name)
{
	DataModifierMap::iterator it = ValueDataModifiers.Find(name);
	DataModifierMap::iterator end = ValueDataModifiers.end();

	if(it != end)
		return it->second;
	return NULL;
}

DialectDisallow* DialectTypeBase::FindDisallow(const opString& name)
{
	int num = Disallows.Size();
	for(int i = 0; i < num; i++)
	{
		if(Disallows[i]->GetName() == name)
			return Disallows[i];
	}
	return NULL;
}

FunctionModifierNode* DialectCategory::FindValueFunctionModifier(const opString& name)
{
	FunctionModifierMap::iterator it = ValueFunctionModifiers.Find(name);
	FunctionModifierMap::iterator end = ValueFunctionModifiers.end();

	if(it != end)
		return it->second;
	return NULL;
}

bool DialectCategory::HasBasicModifier(const opString& name)
{
	if(FindFunctionModifier(name)
	|| FindDataModifier(name))
		return true;
	return false;
}

bool DialectCategory::HasValueModifier(const opString& name)
{
	if(FindValueFunctionModifier(name)
	|| FindValueDataModifier(name))
		return true;
	return false;
}

DialectLocation* DialectCategory::FindLocation(DialectLocation::Locations locationtoken)
{
	int num = Locations.Size();
	for(int i = 0; i < num; i++)
	{
		if(Locations[i]->GetId() == locationtoken)
			return Locations[i];
	}

	return NULL;
}

DialectEnumLocation* DialectEnumeration::FindLocation(DialectLocation::Locations locationtoken)
{
	int num = Locations.Size();
	for(int i = 0; i < num; i++)
	{
		if(Locations[i]->GetId() == locationtoken)
			return Locations[i];
	}

	return NULL;
}


bool DialectMapExpression::Evaluate(ModifierSupportBase* statement)
{
	return Expression->Evaluate(statement);
}

bool DialectMapExpression::Validate(DialectExpressionMap* map)
{
	return Expression->Validate(map);
}

bool DialectExpressionMap::IsMapped(ModifierSupportBase* statement)
{
	int num = Expressions.Size();
	
	//default: true
	if(!num)
		return true;
	
	//works like AND
	for(int i = 0; i < num; i++)
	{
		bool result = Expressions[i].Evaluate(statement);
		
		if(!result)
			return false;
	}
	
	return true;
}


void DialectExpressionMap::BuildValuedModifiers(vector<opString>& modifiernames)
{
	//TODO: cache this
	
	bool bFirst = true;
	opSet<opString> allmodifiers;
	
 	int exnum = (int)Expressions.size();
 	for(int i = 0; i < exnum; i++)
 	{
		if(IsNode* node = node_cast<IsNode>(Expressions[i].GetExpression()))
		{
			opSet<opString> modifiers;
			
			Expressions[i].BuildValuedModifiers(modifiers);
			
			allmodifiers.Union(modifiers);
		}
 	}
	
	copy(allmodifiers.begin(),allmodifiers.end(),back_inserter(modifiernames));
}

void DialectMapExpression::BuildValuedModifiers(opSet<opString>& modifiers)
{
	Expression->GetKnownModifiers(modifiers);
}


DialectNoteBase* DialectLocationBase::FindTag(const opString& name)
{
	OrderedNoteList::pointer_iterator i   = OrderedNotes.begin();
	OrderedNoteList::pointer_iterator end = OrderedNotes.end();

	while (i != end)
	{
		if (i->GetName() == name)
			return *i;

		++i;
	}
	
	return NULL;
}

// Given the name of a note, returns the iterator or end if not found.
DialectLocationBase::OrderedNoteList::iterator DialectLocationBase::GetPosition(const opString& notename)
{
	OrderedNoteList::pointer_iterator i   = OrderedNotes.begin();
	OrderedNoteList::pointer_iterator end = OrderedNotes.end();

	while (i != end)
	{
		if ( i->GetName() == notename )
			return i;

		++i;
	}

	return i;
}

// insert a note at a particular position (if before/after modifier
// is present)
void DialectLocationBase::InsertOrdered(DialectNoteBase* node, OrderModifierNodeBase* order)
{
	// If there is no order modifier, jsut insert it at the end.
	if ( !order )
		OrderedNotes.push_back( node );
	// If we have a before modifier, validate it and then insert it.
	else if ( BeforeNode* before = node_cast<BeforeNode>( order ) )
	{
		opString                  loc = before->GetLocation()->GetValue();
		OrderedNoteList::iterator i   = GetPosition( loc );

		// If we can't find the before location, error.
		if ( i == OrderedNotes.end() )
		{
			opError::MessageError( before, "Construct '" + loc + "' does not exist previously." );
		}

		OrderedNotes.Insert( i, node );
	}
	// If we have an after modifier, validate it and then insert it.
	else if ( AfterNode* after = node_cast<AfterNode>( order ) )
	{
		opString                  loc = after->GetLocation()->GetValue();
		OrderedNoteList::iterator i   = GetPosition( loc );

		// If we can't find the after location, error.
		if ( i == OrderedNotes.end() )
		{
			opError::MessageError( after, "Construct '" + loc + "' does not exist previously." );
		}

		OrderedNotes.InsertAfter( i, node );
	}
}

DialectNote* DialectLocation::FindNote(const opString& name)
{
	int num = (int)Notes.size();
	for(int i = 0; i < num; i++)
	{
		if(Notes[i]->GetName() == name)
			return Notes[i];
	}

	return NULL;
}

DialectEnumNote* DialectEnumLocation::FindNote(const opString& name)
{
	int num = (int)Notes.size();
	for(int i = 0; i < num; i++)
	{
		if(Notes[i]->GetName() == name)
			return Notes[i];
	}

	return NULL;
}

DialectMap* DialectLocation::FindMap(const opString& name)
{
	int num = (int)Maps.size();
	for(int i = 0; i < num; i++)
	{
		if(Maps[i]->GetName() == name)
			return Maps[i];
	}

	return NULL;
}

void DialectNote::BuildValidArguments(vector<opString>& arguments)
{
	DialectNoteBase::BuildValidArguments(arguments);
	
	arguments.push_back("class_name");
	arguments.push_back("parent_name");

	//alternatives (mangled)
	arguments.push_back("alt_class_name");

	//NOTE: probably more arguments could be added,
	//		need to figure out expansion before going 
	//		argument crazy though.
}

void DialectNoParentNote::BuildValidArguments(vector<opString>& arguments)
{
	DialectNoteBase::BuildValidArguments(arguments);

	//NOTE: parent_name is not a valid argument for the noparent note.

	arguments.push_back("class_name");
	arguments.push_back("alt_class_name");
}

void DialectMap::BuildValidArguments(vector<opString>& arguments)
{
	assert(0);
}

void DialectEnumMap::BuildValidArguments(vector<opString>& arguments)
{
	assert(0);
}

void DialectNoteBase::BuildValidArguments(vector<opString>& arguments)
{
	//implicit arguments (modifier-less)
	arguments.push_back("scope");
	arguments.push_back("namespace_start");
	arguments.push_back("namespace_end");
	arguments.push_back("source_line");
	arguments.push_back("source_file");

	//alternatives (mangled)
	arguments.push_back("alt_scope");
	arguments.push_back("alt_namespace_start");
	arguments.push_back("alt_namespace_end");
}

bool DialectNoteBase::GetArgumentDescription(const opString& name, opString& description)
{
	if(ModifierDescriptions::GetArgument(name,description))
		return true;

	//TODO: now ask the category about them
	return false;
}

bool DialectNoteBase::GetModifierDescription(const opString& name, opString& description)
{
	if(name.EndsWith("()"))
	{
		if(ModifierDescriptions::GetValuedModifier(name,description))
			return true;
	}
	else
	{
		if(ModifierDescriptions::GetModifier(name,description))
			return true;
	}

	//TODO: now ask the category about them
	return false;
}

void DialectEnumNote::BuildValidArguments(vector<opString>& arguments)
{
	arguments.push_back("enum_type");
	
	DialectNoteBase::BuildValidArguments(arguments);
}

void DialectEnumMapNote::BuildValidArguments(vector<opString>& arguments)
{

	if(Type == Name || Type == Value)
	{
		arguments.push_back("enum_name");
		arguments.push_back("map_index");
	}

	if(Type == Value)
		arguments.push_back("enum_value");

	if(Type == End)
		arguments.push_back("map_count");

	DialectEnumNote::BuildValidArguments(arguments);

	//TODO: add arguments
	//all: enum_type()
	//Start: enum_type()
	//Name: enum_name()
	//Value: enum_name(), enum_value()
	//End: enum_type(), map_count()

	//add valued modifiers as well (none as of yet)
	//Map->BuildValuedModifiers(arguments);
}

void DialectMapNote::BuildValidArguments(vector<opString>& arguments)
{
	//add valued modifiers as well
	if (Type == Mapping)
		GetMap()->BuildValuedModifiers(arguments);
	
	//this is a listing of all valid automatic -arguments-
	if(Type == Mapping)
	{
		//value modifier arguments
		arguments.push_back("member_name");
		arguments.push_back("all_modifiers");
		
		if(GetMap()->GetMode() == DialectMap::DataMap)
		{
			arguments.push_back("data_type");
			arguments.push_back("data_full_type");
		}
		
		if(GetMap()->GetMode() == DialectMap::FunctionMap)
		{
			arguments.push_back("function_return_type");
			arguments.push_back("function_arguments");
			arguments.push_back("function_arguments_count");
		}
		
		arguments.push_back("map_index");
	}
	
	if(Type == End)
	{
		//implicit arguments (modifier-less)
		arguments.push_back("map_count");
	}
	
	DialectNote::BuildValidArguments(arguments);
}

bool DialectMapNote::GetModifierDescription(const opString& name, opString& description)
{
	return GetMap()->GetModifierDescription(name,description);
}

bool DialectMapNote::GetArgumentDescription(const opString& name, opString& description)
{
	return GetMap()->GetArgumentDescription(name,description);
}

bool DialectMap::GetModifierDescription(const opString& name, opString& description)
{
	if(DialectNoteBase::GetModifierDescription(name,description))
		return true;
	
	if(name.EndsWith("()"))
	{
		if(Mode == DataMap)
		{
			if(DataModifierNode* node = GetCategory()->FindValueDataModifier(name.RLeft(2)))
			{
				if(node->GetDescription(description))
					return true;
			}
		}
		else if(Mode == FunctionMap)
		{
			if(FunctionModifierNode* node = GetCategory()->FindValueFunctionModifier(name.RLeft(2)))
			{
				if(node->GetDescription(description))
					return true;
			}
		}
	}
	else
	{
		if(Mode == DataMap)
		{
			if(DataModifierNode* node = GetCategory()->FindDataModifier(name))
			{
				if(node->GetDescription(description))
					return true;
			}
		}
		else if(Mode == FunctionMap)
		{
			if(FunctionModifierNode* node = GetCategory()->FindFunctionModifier(name))
			{
				if(node->GetDescription(description))
					return true;
			}
		}
	}

	return false;
}

bool DialectMap::GetArgumentDescription(const opString& name, opString& description)
{
	if(DialectNoteBase::GetArgumentDescription(name,description))
		return true;

	if(Mode == DataMap)
	{
		if(DataModifierNode* node = GetCategory()->FindValueDataModifier(name))
		{
			if(node->GetDescription(description))
				return true;
		}
	}
	else if(Mode == FunctionMap)
	{
		if(FunctionModifierNode* node = GetCategory()->FindValueFunctionModifier(name))
		{
			if(node->GetDescription(description))
				return true;
		}
	}

	return false;
}

bool DialectDisallow::GetModifierDescription(const opString& name, opString& description)
{
	if(DialectNoteBase::GetModifierDescription(name,description))
		return true;
	
	if(!GetCategory())
		return false;
	
	if(name.EndsWith("()"))
	{
		//NOTE: it could be in both!
		if(DataModifierNode* node = GetCategory()->FindValueDataModifier(name))
		{
			if(node->GetDescription(description))
				return true;
		}
		if(FunctionModifierNode* node = GetCategory()->FindValueFunctionModifier(name))
		{
			if(node->GetDescription(description))
				return true;
		}
	}
	else
	{
		//NOTE: it could be in both??? kinda weird.
		if(DataModifierNode* node = GetCategory()->FindDataModifier(name))
		{
			if(node->GetDescription(description))
				return true;
		}
		if(FunctionModifierNode* node = GetCategory()->FindFunctionModifier(name))
		{
			if(node->GetDescription(description))
				return true;
		}
	}

	return false;
}



bool DialectExpressionMap::ValidateExpressions()
{
	bool result = true;
	
	for(int i = 0; i < Expressions.Size(); i++)
	{
		result = Expressions[i].Validate(this)?result:false;
	}
	
	return result;
}

bool DialectMap::HasBasicModifier(const opString& name)
{
	if(Mode == DataMap)
	{
		if(DataModifierSet::HasBasicModifier(name)
		|| StatementModifierSet::HasBasicModifier(name)
		|| CategoryModifierSet::HasBasicModifier(name)
		|| GetCategory()->FindDataModifier(name))
			return true;
	}
	else if(Mode == FunctionMap)
	{
		if(FunctionModifierSet::HasBasicModifier(name)
		|| StatementModifierSet::HasBasicModifier(name)
		|| CategoryModifierSet::HasBasicModifier(name)
		|| GetCategory()->FindFunctionModifier(name))
			return true;
	}
	
	return false;
}

bool DialectMap::HasValueModifier(const opString& name)
{
	if(Mode == DataMap)
	{
		if(DataModifierSet::HasValueModifier(name)
		|| StatementModifierSet::HasValueModifier(name)
		|| CategoryModifierSet::HasValueModifier(name)
		|| GetCategory()->FindValueDataModifier(name))
			return true;
	}
	else if(Mode == FunctionMap)
	{
		if(FunctionModifierSet::HasValueModifier(name)
		|| StatementModifierSet::HasValueModifier(name)
		|| CategoryModifierSet::HasValueModifier(name)
		|| GetCategory()->FindValueFunctionModifier(name))
			return true;
	}
	
	return false;
}

void DialectMap::GetModifiers(opArray<opString>& modifiers)
{
	if(Mode == DataMap)
	{
		GetCategory()->GetDataModifiers(modifiers);
		DataModifierSet::GetModifiers(modifiers);
	}
	else if(Mode == FunctionMap)
	{
		GetCategory()->GetFunctionModifiers(modifiers);
		FunctionModifierSet::GetModifiers(modifiers);
	}

	StatementModifierSet::GetModifiers(modifiers);
	CategoryModifierSet::GetModifiers(modifiers);
}


bool DialectDisallow::HasBasicModifier(const opString& name)
{
	if(DataModifierSet::HasBasicModifier(name)
	|| FunctionModifierSet::HasBasicModifier(name)
	|| CategoryModifierSet::HasBasicModifier(name)
	|| StatementModifierSet::HasBasicModifier(name)
	|| GetCategory()->HasBasicModifier(name))
		return true;
	return false;
}

bool DialectDisallow::HasValueModifier(const opString& name)
{
	if(DataModifierSet::HasValueModifier(name)
	|| FunctionModifierSet::HasValueModifier(name)
	|| CategoryModifierSet::HasValueModifier(name)
	|| StatementModifierSet::HasValueModifier(name)
	|| GetCategory()->HasValueModifier(name))
		return true;
	return false;
}

void DialectDisallow::GetModifiers(opArray<opString>& modifiers)
{
	if(GetCategory())
	{
		GetCategory()->GetDataModifiers(modifiers);
		GetCategory()->GetFunctionModifiers(modifiers);
	}

	DataModifierSet::GetModifiers(modifiers);
	FunctionModifierSet::GetModifiers(modifiers);
	StatementModifierSet::GetModifiers(modifiers);
	CategoryModifierSet::GetModifiers(modifiers);
}

bool DialectNote::HasBasicModifier(const opString& name)
{
	//if the category has it, or if its automatic return true
	if(CategoryModifierSet::HasBasicModifier(name)
	|| GetCategory()->HasBasicModifier(name))
		return true;
	return false;
}

bool DialectNote::HasValueModifier(const opString& name)
{
	//if the category has it, or if its automatic return true
	if(CategoryModifierSet::HasValueModifier(name)
	|| GetCategory()->HasValueModifier(name))
		return true;
	return false;
}

void DialectNote::GetModifiers(opArray<opString>& modifiers)
{
	CategoryModifierSet::GetModifiers(modifiers);
}

bool DialectEnumNote::HasBasicModifier(const opString& name)
{
	//if the category has it, or if its automatic return true
	if(EnumModifierSet::HasBasicModifier(name))
		return true;
	return false;
}

bool DialectEnumNote::HasValueModifier(const opString& name)
{
	//if the category has it, or if its automatic return true
	if(EnumModifierSet::HasValueModifier(name))
		return true;
	return false;
}

void DialectEnumNote::GetModifiers(opArray<opString>& modifiers)
{
	EnumModifierSet::GetModifiers(modifiers);
}

//TODO: I need to run this using a note type, not notedefinition
//		because I need to adjust valid arguments based on many different things
//		classes of notes:
//			start, end, note
//			mapping note (+member_name)
//			data/function notes
void DialectNoteBase::ValidateArguments(DialectNote& note)
{
	NoteDefinitionNode* notenode = note.GetNoteDefinition();
	
	typedef vector<opString>::iterator iterator;
	
	//1. get all arguments in the notenode
	vector<opString> notearguments;

	notenode->GetArguments(notearguments);
	
	//2. get all valid arguments
	vector<opString> validarguments;

	note.BuildValidArguments(validarguments);
	
	//3. make sure they all match
	int num = (int) notearguments.size();

	for(int i = 0; i < num; i++)
	{
		iterator end   = validarguments.end();
		iterator found = find(validarguments.begin(),validarguments.end(),notearguments[i]);

		if(found == end)
		{			
			opString error;
			opString spacer;
			
			error  = "Note argument '" + notearguments[i] + "' is invalid.\n";
			error += "\tValid arguments: ";
			spacer = "\t                 ";

			for(int j = 0; j < (int) validarguments.size(); j++)
			{
				if (j > 0)
				{
					error += spacer;
					error += validarguments[j];
				}
				else
				{
					error += validarguments[j];
				}

				if (j < (int) validarguments.size() - 1)
				{
					error += ',';
				}

				error += '\n';
			}

			opError::MessageError(notenode,error);
		}
	}
	
	//4. make sure theres no duplicate arguments
	for(int i = 0; i < num; i++)
	{
		for(int j = i+1; j < num; j++)
		{
			if(notearguments[i] == notearguments[j])
			{
				opError::MessageError(notenode,"Note argument '" + notearguments[i] + "' already used once.");
			}
		}
	}
}

//registration path for a normal note
void DialectNote::RegisterNote(NoteDefinitionNode* notenode)
{
	if (notenode->HasPathSubTag())
	{
		//invalid path, too many elements
		opError::MessageError(notenode,"Note definition has too many tag specifiers.");
	}
	
	opString pathstring;

	notenode->GetPath()->PrintString(pathstring);
	notenode->GetArguments()->PrintString(pathstring);
	
	// The note has not yet been defined.
	if (!Definition)
	{
		// If this note has the override keyword, throw an error
		// because it's the first time it's been seen.
		if ( OverrideNode* override = notenode->GetOverride() )
			opError::MessageError(override, "'override' modifier invalid because this is the note's first definition.");

		if(opParameters::Get().Diagnostics)
			Log("Registered Note Definition : " + pathstring);
	}
	// The note has already been defined.  Throw an error if the note
	// does not have the override modifier.
 	else if ( notenode->GetOverride() == NULL )
 	{
 		opError::NoteDefinitionError(Definition,notenode);
 	}
	
	Definition = notenode;
	
	ValidateArguments(*this);
}

void DialectMap::RegisterNote(NoteDefinitionNode* notenode, const opString& subtag)
{
	DialectNote* note;
	
	if(subtag == "start")
	{
		note = &StartNote;
	}
	else if(subtag == "end")
	{
		note = &EndNote;
	}
	else if(subtag == "mapping")
	{
		note = &MappingNote;
	}
	else
	{
		opError::MessageError(notenode,"Invalid sub-tag '" + subtag + "' used, use 'start' OR 'mapping' OR 'end'.");
	}
	
	opString pathstring;

	notenode->GetPath()->PrintString(pathstring);
	notenode->GetArguments()->PrintString(pathstring);

	NoteDefinitionNode* oldnote = note->GetNoteDefinition();

	if ( !oldnote )
	{
		// If this note has the override keyword, throw an error
		// because it's the first time it's been seen.
		if ( OverrideNode* override = notenode->GetOverride() )
			opError::MessageError(override, "'override' modifier invalid because this is the note's first definition.");

		if(opParameters::Get().Diagnostics)
			Log(opString("Registered Note Definition : ") + pathstring);
	}
	else if ( notenode->GetOverride() == NULL )
	{
		opError::NoteDefinitionError( oldnote, notenode );
	}
	
	note->SetNoteDefinition(notenode);
	
	ValidateArguments(*note);
}

void DialectEnumMap::RegisterNote(NoteDefinitionNode* notenode, const opString& subtag)
{
	DialectNote* note;

	if(subtag == "start")
	{
		note = &StartNote;
	}
	else if(subtag == "end")
	{
		note = &EndNote;
	}
	else if(subtag == "name")
	{
		note = &NameNote;
	}
	else if(subtag == "value")
	{
		note = &ValueNote;
	}
	else
	{
		opError::MessageError(notenode,"Invalid sub-tag '" + subtag + "' used, use 'start' OR 'name' OR 'value' OR 'end'.");
	}

	opString pathstring;

	notenode->GetPath()->PrintString(pathstring);
	notenode->GetArguments()->PrintString(pathstring);

	NoteDefinitionNode* oldnote = note->GetNoteDefinition();

	if ( !oldnote )
	{
		// If this note has the override keyword, throw an error
		// because it's the first time it's been seen.
		if ( OverrideNode* override = notenode->GetOverride() )
			opError::MessageError(override, "'override' modifier invalid because this is the note's first definition.");

		if(opParameters::Get().Diagnostics)
			Log(opString("Registered Note Definition : ") + pathstring);
	}
	else if ( notenode->GetOverride() == NULL )
	{
		opError::NoteDefinitionError(oldnote, notenode);
	}

	note->SetNoteDefinition(notenode);

	ValidateArguments(*note);
}

//
// GetNotes
//

//tag version
NoteDefinitionNode* DialectTracker::GetNote(const opString& category, DialectLocation::Locations location, const opString& tag)
{
	//err, do I need this?
	DialectCategory* foundcategory = GetCategory(category);
	if(!foundcategory)
		return NULL;
	
	DialectLocation* foundlocation = foundcategory->FindLocation(location);
	if(!foundlocation)
		return NULL;
	
	DialectNote* foundnote = foundlocation->FindNote(tag);
	if(!foundnote)
		return NULL;
	
	return foundnote->GetNoteDefinition();
}

//subtag version
NoteDefinitionNode* DialectTracker::GetNote(const opString& category, DialectLocation::Locations location, const opString& tag, const opString& subtag)
{
	//err, do I need this?
	DialectCategory* foundcategory = GetCategory(category);
	if(!foundcategory)
		return NULL;

	DialectLocation* foundlocation = foundcategory->FindLocation(location);
	if(!foundlocation)
		return NULL;

	DialectMap* foundmap = foundlocation->FindMap(tag);
	if(!foundmap)
		return NULL;

	return foundmap->GetNoteNode(subtag);
}

NoteDefinitionNode* DialectMap::GetNoteNode(const opString& name)
{
	if(name == "start")
		return StartNote.GetNoteDefinition();
	else if(name == "end")
		return EndNote.GetNoteDefinition();
	else if(name == "mapping")
		return MappingNote.GetNoteDefinition();
	return NULL;
}

bool DialectTracker::Validate()
{
	INIT_EXCEPTIONS
	{
		bool bresult = GetInstance().ValidateNotes();

		return bresult;
	}
	CATCH_EXCEPTIONS

	return false;
}

bool DialectTracker::ValidateNotes()
{
	bool bresult = true;
	
	{
		categoryiterator end = CategoryNodes.end();
		categoryiterator it = CategoryNodes.begin();

		while(it != end)
		{
			bresult = it->second->Validate()?bresult:false;

			++it;
		}
	}
	
	{
		enumerationiterator end = EnumerationNodes.end();
		enumerationiterator it = EnumerationNodes.begin();

		while(it != end)
		{
			bresult = it->second->Validate()?bresult:false;

			++it;
		}
	}
	
	return bresult;
}

bool DialectCategory::Validate()
{
	int num = Locations.Size();
	
	bool bresult = true;
	for(int i = 0; i < num; i++)
	{
		bresult &= Locations[i]->Validate(TypePath(this));
	}

	bresult &= ValidateSpecialNotes();
	
	int disallows = Disallows.Size();
	for(int i = 0; i < disallows; i++)
	{
		bresult &= Disallows[i]->Validate();
	}

	return bresult;
}

bool DialectEnumeration::Validate()
{
	int num = Locations.Size();

	bool bresult = true;
	for(int i = 0; i < num; i++)
	{
		bresult &= Locations[i]->Validate(TypePath(this));
	}

	bresult &= ValidateSpecialNotes();

	return bresult;
}


bool DialectCategory::ValidateSpecialNotes()
{
	GenerateDefaultNotes();
	
	if(!ValidateNote(IdentifierNote))
	{
		IdentifierNote.SetNoteDefinition(DefaultIdentifierNote);
	}
	if(!ValidateNote(ParentNote))
	{
		ParentNote.SetNoteDefinition(DefaultParentNote);
	}
	if(!ValidateNote(NoParentNote))
	{
		NoParentNote.SetNoteDefinition(DefaultNoParentNote);
	}
	if(!ValidateNote(PrefixNote))
	{
		PrefixNote.SetNoteDefinition(DefaultPrefixNote);
	}
	if(!ValidateNote(PostfixNote))
	{
		PostfixNote.SetNoteDefinition(DefaultPostfixNote);
	}
	if(!ValidateNote(DefaultNote))
	{
		DefaultNote.SetNoteDefinition(DefaultDefaultNote);
	}

	return true;
}

bool DialectEnumeration::ValidateSpecialNotes()
{
	GenerateDefaultNotes();

	if(!ValidateNote(IdentifierNote))
	{
		IdentifierNote.SetNoteDefinition(DefaultIdentifierNote);
	}
	if(!ValidateNote(ParentNote))
	{
		ParentNote.SetNoteDefinition(DefaultParentNote);
	}

	return true;
}

NoteDefinitionNode* DialectTypeBase::GenerateDefaultNote(opString category, opString tag)
{
	opNode* Node = GetTypeNode();
	
	stacked<NoteDefinitionNode> note = NEWNODE(NoteDefinitionNode);
	note->CopyBasics(Node);
	
	stacked<ScopeNode> scope = NEWNODE(ScopeNode);
	scope->CopyBasics(Node);
	note->SetPath(*scope);
	
	TerminalNode* categorynode = scope->AppendTerminalNode(category,T_ID);
	TerminalNode* tagnode = scope->AppendTerminalNode(tag,T_ID);
	scope->AddScope(categorynode);
	scope->AddScope(tagnode);
	
	stacked<BraceBlockNode> brace = NEWNODE(BraceBlockNode);
	brace->CopyBasics(Node);
	note->SetBody(*brace);
	
	stacked<NoteArgumentListNode> arguments = NEWNODE(NoteArgumentListNode);
	arguments->CopyBasics(Node);
	note->SetArguments(*arguments);
	
	note->AppendNode(brace);
	note->AppendNode(arguments);
	note->AppendNode(scope);
	
	NoteDefinitionNode* notenode = *note;
	Node->AppendNode(note);
	
	return notenode;
}

void DialectEnumeration::GenerateDefaultNotes()
{
	DefaultIdentifierNote = GenerateDefaultNote(GetName(),"identifier");
	DefaultIdentifierNote->GetBody()->AppendTerminalNode("enum",T_ENUM);
	
	//empty default notes
	DefaultParentNote = GenerateDefaultNote(GetName(),"parent");
}

void DialectCategory::GenerateDefaultNotes()
{
	DefaultIdentifierNote = GenerateDefaultNote(GetName(),"identifier");
	DefaultIdentifierNote->GetBody()->AppendTerminalNode("class",T_CLASS);
	
	DefaultDefaultNote = GenerateDefaultNote(GetName(),"default");
	DefaultDefaultNote->GetBody()->AppendTerminalNode("private",T_PRIVATE);
	DefaultDefaultNote->GetBody()->AppendTerminalNode(":",T_COLON);

	//empty default notes
	DefaultNoParentNote = GenerateDefaultNote(GetName(),"noparent");
	DefaultParentNote = GenerateDefaultNote(GetName(),"parent");
	DefaultPrefixNote = GenerateDefaultNote(GetName(),"prefix");
	DefaultPostfixNote = GenerateDefaultNote(GetName(),"postfix");
}

//validation for special notes
bool DialectTypeBase::ValidateNote(DialectNote& note)
{
	const opString& name = note.GetName();

	opString pathstring = GetName() + "::" + name;
	
	NoteDefinitionNode* node = note.GetNoteDefinition();

	if(node)
	{
		//ok, registered
		if(opParameters::Get().Diagnostics)
			Log(opString("Validated Note ") + pathstring);		
		return true;
	}
	else
	{
		//missing, error
		if(opParameters::Get().Diagnostics)
			Log(opString("Defaulted Note: ") + pathstring);
		return false;
	}
}

bool DialectLocation::Validate(const TypePath& path)
{
	OrderedNoteList::pointer_iterator i       = OrderedNotes.begin();
	OrderedNoteList::pointer_iterator end     = OrderedNotes.end();
	bool                              bresult = true;

	while (i != end)
	{
		if(DialectNote* note = i->ToNote())
			bresult = note->Validate(LocationPath(path,this))?bresult:false;
		else if(DialectMap* map = i->ToMap())
			bresult = map->Validate(LocationPath(path,this))?bresult:false;

		++i;
	}
	
	return bresult;
}

bool DialectEnumLocation::Validate(const TypePath& path)
{
	OrderedNoteList::pointer_iterator i       = OrderedNotes.begin();
	OrderedNoteList::pointer_iterator end     = OrderedNotes.end();
	bool                              bresult = true;

	while (i != end)
	{
		if(DialectNote* note = i->ToNote())
			bresult = note->Validate(LocationPath(path,this))?bresult:false;
		else if(DialectEnumMap* map = i->ToEnumMap())
			bresult = map->Validate(LocationPath(path,this))?bresult:false;

		++i;
	}

	return bresult;
}

bool DialectNote::Validate(const LocationPath& path)
{
	if(!ValidateExpressions())
		return false;

	TagPath tag(path,this);
	
	if(Definition)
	{
		//ok, registered
		opString pathstring = tag.GetPath();
		if(opParameters::Get().Diagnostics)
			Log(opString("Validated Note ") + pathstring);		
		return true;
	}
	else
	{
		//missing, error
		//TODO: need a good error, but shouldn't throw?
		opString pathstring = tag.GetPath();
		opError::MessageError(GetNoteNode(),pathstring + " undefined.",false);
		return false;
	}
}

bool DialectMap::Validate(const LocationPath& path)
{
	if(!ValidateExpressions())
		return false;
	
	TagPath tag(path,this);
	
	bool bError = false;
	
	//TODO: don't care about start, end note definitions, its annoying requiring them.
	//		might want to require end if start is defined and vice versa...

	if(StartNote.GetNoteDefinition())
	{
		//ok, registered
		opString pathstring = tag.GetPath() + "::start";
		if(opParameters::Get().Diagnostics)
			Log(opString("Validated Note ") + pathstring);
	}
	
	if(MappingNote.GetNoteDefinition())
	{
		//ok, registered
		opString pathstring = tag.GetPath() + "::mapping";
		if(opParameters::Get().Diagnostics)
			Log(opString("Validated Note ") + pathstring);
	}
	else
	{
		//missing, error
		opString pathstring = tag.GetPath() + "::mapping";
		opError::MessageError(GetMapNode(),pathstring + " undefined.",false);
		bError = true;
	}
	
	if(EndNote.GetNoteDefinition())
	{
		//ok, registered
		opString pathstring = tag.GetPath() + "::end";
		if(opParameters::Get().Diagnostics)
			Log(opString("Validated Note ") + pathstring);
	}
	
	return !bError;
}

bool DialectEnumMap::Validate(const LocationPath& path)
{
// 	if(!ValidateExpressions())
// 		return false;
	
	TagPath tag(path,this);

	bool bError = false;

	if(StartNote.GetNoteDefinition())
	{
		//ok, registered
		opString pathstring = tag.GetPath() + "::start";
		if(opParameters::Get().Diagnostics)
			Log(opString("Validated Note ") + pathstring);
	}

	if(NameNote.GetNoteDefinition())
	{
		//ok, registered
		opString pathstring = tag.GetPath() + "::name";
		if(opParameters::Get().Diagnostics)
			Log(opString("Validated Note ") + pathstring);
	}
	else
	{
		//missing, error
		//TODO: need a good error, but shouldn't throw?
		opString pathstring = tag.GetPath() + "::name";
		opError::MessageError(GetMapNode(),pathstring + " undefined.",false);
		bError = true;
	}


	if(ValueNote.GetNoteDefinition())
	{
		//ok, registered
		opString pathstring = tag.GetPath() + "::value";
		if(opParameters::Get().Diagnostics)
			Log(opString("Validated Note ") + pathstring);
	}
	else
	{
		//missing, error
		//TODO: need a good error, but shouldn't throw?
		opString pathstring = tag.GetPath() + "::value";
		opError::MessageError(GetMapNode(),pathstring + " undefined.",false);
		bError = true;
	}


	if(EndNote.GetNoteDefinition())
	{
		//ok, registered
		opString pathstring = tag.GetPath() + "::end";
		if(opParameters::Get().Diagnostics)
			Log(opString("Validated Note ") + pathstring);
	}

	return !bError;
}

///==========================================
/// Modifier Settings
///==========================================

bool DialectNote::IsHidden()
{
	if ( bForceHidden )
		return true;

	if(!Node)
		return false;
	
	DialectStatementBase* statement = Node->GetParent()->ToDialectStatementBase();
	
	if(statement->GetModifiers())
		return statement->GetModifiers()->HasModifier(G_HIDDEN);
	
	return false;
}


bool DialectMapNote::IsHidden()
{
	if(!GetMap()->GetMapNode())
		return false;
	
	DialectStatementBase* statement = GetMap()->GetMapNode()->GetParent()->ToDialectStatementBase();

	if(statement->GetModifiers())
		return statement->GetModifiers()->HasModifier(G_HIDDEN);

	return false;
}

bool DialectEnumMapNote::IsHidden()
{
	if(!GetEnumMap()->GetMapNode())
		return false;

	DialectStatementBase* statement = GetEnumMap()->GetMapNode()->GetParent()->ToDialectStatementBase();

	if(statement->GetModifiers())
		return statement->GetModifiers()->HasModifier(G_HIDDEN);

	return false;
}

TerminalNode* ModifierNodeBase::ErrorString()
{
	DialectStatementBase* statement = GetParent()->ToDialectStatementBase();
	
	if(statement->GetModifiers())
	{
		ErrorNode* node = statement->GetModifiers()->FindNode<ErrorNode>();

		if(node)
			return node->GetError();
	}

	return NULL;
}

TerminalNode* DisallowNode::ErrorString()
{
	DialectStatementBase* statement = GetParent()->ToDialectStatementBase();

	if(statement->GetModifiers())
	{
		ErrorNode* node = statement->GetModifiers()->FindNode<ErrorNode>();

		if(node)
			return node->GetError();
	}

	return NULL;
}

///==========================================
/// String Queries
///==========================================

void DialectCategory::GetDataModifiers(opArray<opString>& modifiers)
{
	GetBasicDataModifiers(modifiers);
	GetValueDataModifiers(modifiers);
}

void DialectCategory::GetFunctionModifiers(opArray<opString>& modifiers)
{
	GetBasicFunctionModifiers(modifiers);
	GetValueFunctionModifiers(modifiers);
}

void DialectCategory::GetBasicDataModifiers(opArray<opString>& modifiers)
{
	DataModifierMap::iterator it  = DataModifiers.begin();
	DataModifierMap::iterator end = DataModifiers.end(); 
	
	while(it != end)
	{
		modifiers.push_back(it->first);
		++it;
	}
}

void DialectCategory::GetBasicFunctionModifiers(opArray<opString>& modifiers)
{
	FunctionModifierMap::iterator it  = FunctionModifiers.begin();
	FunctionModifierMap::iterator end = FunctionModifiers.end(); 

	while(it != end)
	{
		modifiers.push_back(it->first);
		++it;
	}
}

void DialectCategory::GetValueDataModifiers(opArray<opString>& modifiers)
{
	DataModifierMap::iterator it  = ValueDataModifiers.begin();
	DataModifierMap::iterator end = ValueDataModifiers.end(); 

	while(it != end)
	{
		modifiers.push_back(it->first + "()");
		++it;
	}
}

void DialectCategory::GetValueFunctionModifiers(opArray<opString>& modifiers)
{
	FunctionModifierMap::iterator it  = ValueFunctionModifiers.begin();
	FunctionModifierMap::iterator end = ValueFunctionModifiers.end(); 

	while(it != end)
	{
		modifiers.push_back(it->first + "()");
		++it;
	}
}

// get the list of extensions
const opMap<opString, ExtensionNode*>& DialectTracker::GetExtensions()
{
	return GetInstance().ExtensionNodes;
}

//
// File declaration stuff..
//

// check for duplicate file declaration locations
void DialectTracker::CheckForDuplicateFileDeclarationLocations(FileDeclarationNode* node)
{
	FileDeclarationBodyNode* body = node->GetBody();
	opNode::iterator         i    = body->GetBegin();
	opNode::iterator         end  = body->GetEnd();

	// Check for duplicate locations.
	while (i != end)
	{
		if ( FileDeclarationLocationStatementNode* statement = node_cast<FileDeclarationLocationStatementNode>(*i) )
		{
			FileDeclarationLocationNode* location = node_cast<FileDeclarationLocationNode>( statement->GetStatement() );
			opString                     name     = location->GetName()->GetValue();
			FileDeclarationLocationNode* existing = NULL;

			// Error if this location already exists.
			if ( FileDeclarationLocations.Find(name, existing) )
				opError::RedefinitionError(name, existing, location);

			// Enter it into the set.
			FileDeclarationLocations.Insert(location->GetName()->GetValue(), location);
		}

		++i;
	}
}

// register a file declaration
void DialectTracker::RegisterFileDeclaration(FileDeclarationNode* node)
{
	/*=== First, we make sure that none of this node's locations already exist globally. ===*/

	CheckForDuplicateFileDeclarationLocations(node);

	/*=== Now, register the file. ===*/
	
 	DialectFileDeclaration* filedecl = NULL;
 	opString                name     = node->GetName()->GetValue();
 
	// If this file doesn't exist, create a new one and register it.
 	if ( !FileDeclarationNodes.Find(name, filedecl) )
 	{
 		filedecl = new DialectFileDeclaration(name);
 
 		FileDeclarationNodes.Insert(name, filedecl);
 	}

	// Now, merge the file declaration's locations into 'filedecl'.
	filedecl->MergeLocations(node);
}

// Merges a DialectFileDeclaration's and a FileDeclarationNode*'s locations.
// NOTE: At this point we already know that there are no duplicates!
void DialectFileDeclaration::MergeLocations(FileDeclarationNode* node)
{
 	FileDeclarationBodyNode* body = node->GetBody();
 	opNode::iterator         i    = body->GetBegin();
 	opNode::iterator         end  = body->GetEnd();
 
 	while (i != end)
 	{
		// If this is a location statement, continue.
		if ( FileDeclarationLocationStatementNode* statement = node_cast<FileDeclarationLocationStatementNode>( *i ) )
		{
			FileDeclarationLocationNode* loc   = node_cast<FileDeclarationLocationNode>( statement->GetStatement() );
			OrderModifierNodeBase*       order = loc->GetOrder();

			// There's no before/after modifier.
			if ( !order )
				Locations.push_back( loc );
			// We have a before modifier.
			else if ( BeforeNode* before = node_cast<BeforeNode>( order ) )
			{
				opString refloc = before->GetLocation()->GetValue();

				// throw an error if it doesn't exist
				if ( !LocationExists(refloc) )
				{
					opError::MessageError( before, "Location '" + refloc + "' does not exist previously in file '" + Name + "'." );
				}

				AddLocationBefore( loc, refloc );
			}
			// We have an after modifier.
			else if ( AfterNode* after = node_cast<AfterNode>( order ) )
			{
				opString refloc = after->GetLocation()->GetValue();

				// throw an error if it doesn't exist
				if ( !LocationExists(refloc) )
				{
					opError::MessageError( after, "Location '" + refloc + "' does not exist previously in file '" + Name + "'." );
				}

				AddLocationAfter( loc, refloc );
			}
		}
		
 		++i;
 	}
}

// returns the location specified, or null if not found
FileDeclarationLocationNode* DialectFileDeclaration::FindLocation(const opString& name)
{
	opList<FileDeclarationLocationNode*>::pointer_iterator i   = Locations.begin();
	opList<FileDeclarationLocationNode*>::pointer_iterator end = Locations.end();

	while (i != end)
	{
		if (i->GetName()->GetValue() == name)
			return *i;

		++i;
	}

	return NULL;
}

// returns true if the location exists
bool DialectFileDeclaration::LocationExists(const opString& name)
{
	return FindLocation(name) != NULL;
}

// add a location before 'before'
void DialectFileDeclaration::AddLocationBefore(FileDeclarationLocationNode* node, opString& before)
{
	opList<FileDeclarationLocationNode*>::pointer_iterator i   = Locations.begin();
	opList<FileDeclarationLocationNode*>::pointer_iterator end = Locations.end();

	while (i != end)
	{
		if (i->GetName()->GetValue() == before)
			break;

		++i;
	}

	Locations.Insert( i, node );
}

// add a location after 'after'
void DialectFileDeclaration::AddLocationAfter(FileDeclarationLocationNode* node, opString& after)
{
	opList<FileDeclarationLocationNode*>::pointer_iterator i   = Locations.begin();
	opList<FileDeclarationLocationNode*>::pointer_iterator end = Locations.end();

	while (i != end)
	{
		if (i->GetName()->GetValue() == after)
			break;

		++i;
	}

	Locations.InsertAfter( i, node );
}

// register a note with a file declaration
// currently, the only one is the filename
void DialectFileDeclaration::RegisterNote(NoteDefinitionNode* notenode)
{
	/*=== We must figure out what kind of file note this is. ===*/

	opString type;

	notenode->GetPathItem(1, type);

	opString pathstring;

	notenode->GetPath()->PrintString(pathstring);
	notenode->GetArguments()->PrintString(pathstring);

	/*=== Switch on file note type. ===*/

	// filename note
	if (type == "filename")
	{
		// Error if we have too many note path items.
		if ( notenode->NumPathItems() > 2 )
			opError::MessageError(notenode, "File note definition has too many tag specifiers." );

		// If this is the first time we've seen this note, register it.
		if ( !FilenameNote )
		{
			// If this note has the override keyword, throw an error
			// because it's the first time it's been seen.
			if ( OverrideNode* override = notenode->GetOverride() )
				opError::MessageError(override, "'override' modifier invalid because this is the note's first definition.");

			if(opParameters::Get().Diagnostics)
				Log("Registered Note Definition : " + pathstring);
		}
		else if ( notenode->GetOverride() == NULL )
		{
			opError::NoteDefinitionError(FilenameNote, notenode);
		}
		
		FilenameNote = notenode;
	}
	else
	{
		// If we get here, this is an invalid file note!
		opError::MessageError(notenode, "Invalid file note type '" + type + "'.");
	}
}

// If the class prefix exists, returns the associated CategoryNode, otherwise NULL.
CategoryNode* DialectTracker::GetAltClassPrefix(const opString& prefix)
{
	CategoryNode* node;

	if ( DialectTracker::GetInstance().AltClassMap.Find(prefix, node) )
		return node;

	return NULL;
}

// If the struct prefix exists, returns the associated CategoryNode, otherwise NULL.
CategoryNode* DialectTracker::GetAltStructPrefix(const opString& prefix)
{
	CategoryNode* node;

	if ( DialectTracker::GetInstance().AltStructMap.Find(prefix, node) )
		return node;

	return NULL;
}

// If the enumeration prefix exists, returns the associated EnumerationNode, otherwise NULL.
EnumerationNode* DialectTracker::GetAltEnumerationPrefix(const opString& prefix)
{
	EnumerationNode* node;

	if ( DialectTracker::GetInstance().AltEnumMap.Find(prefix, node) )
		return node;

	return NULL;
}

// This method returns all registered prefixes (a set of strings).
const opSet<opString>& DialectTracker::GetAllPrefixes()
{
	return DialectTracker::GetInstance().Prefixes;
}