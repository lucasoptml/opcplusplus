///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: ModifierNodes.h
/// Date: 12/20/2006
///
/// Description:
///
/// Modifier node header.
///****************************************************************

namespace nodes
{

///
/// ModifiersBase
///

class ModifiersBase : public opNode
{
public:
	DECLARE_NODE(ModifiersBase,opNode,T_UNKNOWN);
	
	/*=== utility ===*/

	void AddModifier(opNode* innode)
	{
		Modifiers.push_back(innode);
	}
	
	/**** printing ****/

	void PrintNode(opFileStream& stream)
	{
		PrintNodeChildren(stream);
	}

	void PrintOriginal(opSectionStream& stream)
	{
		PrintOriginalChildren(stream);
	}
	
	/**** queries ****/
	
	bool			    HasModifier(const opString& modifiername);
	bool			    HasModifier(Token modifiertoken);
	TerminalNode*	    FindModifier(Token modifiertoken);
	ValuedModifierNode* GetValuedModifier(const opString& modifiername);
	void			    BuildValueModifiers(vector<ValuedModifierNode*>& modifierlist);

	opNode*			   GetModifier(Token modtoken);

	bool PostProcess()
	{
		return true;
	}
	
	void CloneNode(ModifiersBase* node)
	{
		int num = (int)Modifiers.size();
		for(int i = 0; i < num; i++)
		{
			stacked<opNode> mod = Modifiers[i]->CloneGeneric();
			node->AddModifier(*mod);
			node->AppendNode(mod);
		}
	}

private:
	vector<opNode*> Modifiers;
};

///
/// ModifiersNode
///

class ModifiersNode : public ModifiersBase
{
public:
	DECLARE_NODE(ModifiersNode,ModifiersBase,G_MODIFIERS);

	/*=== validation ===*/

	void CheckFunctionModifiers();
	void CheckDataModifiers();

	/*=== printing ===*/

	void PrintString(opString& s)
	{
		PrintStringChildren(s);
	}

	void PrintBuiltIn(opSectionStream& stream);
	void PrintBuiltInReturnArgument(opString& s);
	void PrintBuiltInArgument(opString& s);
	void PrintBuiltInSource(opSectionStream& stream);

	opString ErrorName() { return ""; }
};

///
/// AutoModifiersNode
///

class AutoModifiersNode : public ModifiersNode
{
public:
	DECLARE_NODE(AutoModifiersNode,ModifiersNode,G_AUTO_MODIFIERS);
};

class ModifierSupportBase
{
public:
	void InitModifierSupport()
	{
		modifiers = NULL;
		automodifiers = NULL;
	}
	
	void SetModifiers(ModifiersNode* innode)
	{
		modifiers = innode;
	}
	
	ModifiersNode* GetModifiers()
	{
		return modifiers;
	}
	
	void SetAutoModifiers(AutoModifiersNode* node)
	{
		automodifiers = node;
	}
	
	AutoModifiersNode* GetAutoModifiers()
	{
		return automodifiers;
	}
	
	TerminalNode*		AddBasicModifier(const opString& modifiername, Token token);
	ValuedModifierNode*	AddValueModifier(const opString& modifiername);
	
	virtual bool			    HasModifier(const opString& modifiername)	   = NULL;
	virtual bool			    HasModifier(Token modifiertoken)				   = NULL;
	virtual ValuedModifierNode* GetValuedModifier(const opString& modifiername) = NULL;
	
protected:
	virtual void CreateModifiersNode() = NULL;
	
	ModifiersNode*		modifiers;
	AutoModifiersNode*  automodifiers;
	
public:
	// you must fetch modifier values using this, it runs through a cached map structure
	opNode* FetchModifier(const opString& modifiername)
	{
		ModifierMap::iterator end = ModifierGenerators.end();

		ModifierMap::iterator found = ModifierGenerators.Find(modifiername);

		if(found != end)
		{
			if((*found).second.CachedModifier != NULL)
				return (*found).second.CachedModifier;

			//get the result from the generate modifier delegate
			opNode* result = (*found).second.GenerateModifier(modifiername);

			(*found).second.CachedModifier = result;

			return result;
		}

		return NULL;
	}
	
	//attempt to cache all modifiers
	void FetchAllModifiers()
	{
		ModifierMap::iterator it = ModifierGenerators.begin();
		ModifierMap::iterator end = ModifierGenerators.end();

		while(it != end)
		{
			if((*it).second.CachedModifier != NULL)
			{
				++it;			
				continue;
			}

			const opString& modifiername = (*it).first;

			//get the result from the generate modifier delegate
			opNode* result = (*it).second.GenerateModifier( modifiername );

			(*it).second.CachedModifier = result;
			
			++it;
		}
	}

	ValuedModifierNode* FetchValueModifier(const opString& modifiername)
	{
		opNode* node = FetchModifier(modifiername);

		return node_cast<ValuedModifierNode>(node);
	}
	
	TerminalNode* FetchBasicModifier(const opString& modifiername)
	{
		opNode* node = FetchModifier(modifiername);

		return node_cast<TerminalNode>(node);
	}

protected:
	// you must register each modifier we want to generate with this
	void RegisterModifier(const opString& modifiername, const ModifierDelegate& generator)
	{
		assert(!generator.empty());

		ModifierGenerators.Insert(modifiername,generator);
	}

	void RegisterBasicModifier(const opString& modifiername)
	{
		ModifierDelegate modifier(this,&ModifierSupportBase::ModifierBasic);
		RegisterModifier(modifiername,modifier);
	}

	opNode* ModifierBasic(const opString& name)
	{
		return AddBasicModifier(name,T_MODIFIER);
	}

protected:
	struct ModifierCache
	{
		ModifierCache()
		{
			CachedModifier = NULL;
		}

		ModifierCache(const ModifierDelegate& delegate)
		{
			GenerateModifier = delegate;
			CachedModifier = NULL;
		}

		ModifierDelegate GenerateModifier;
		opNode*          CachedModifier;
	};

	typedef opMap< opString, ModifierCache > ModifierMap;
	ModifierMap								 ModifierGenerators;

};


} // end of namespace nodes

namespace interfaces
{

template<class Parent>
class ModifierSupport : public Parent, public ModifierSupportBase
{
public:
	IMPLEMENTS_INTERFACE(ModifierSupport);

	void Init()
	{
		InitModifierSupport();
	}

	// looks at auto and specified modifiers
	virtual bool HasModifier(const opString& modifiername)
	{
		if(TerminalNode* node = FetchBasicModifier(modifiername))
		{
			return true;
		}
		
		if(modifiers)
		{
			if(modifiers->HasModifier(modifiername))
				return true;
		}
		
		if(GetVisibility(modifiername))
			return true;
		
		//TODO: should we maybe add these modifiers to this node?
		
		//handle parent modifiers...
		if( !node_cast<OPObjectNode>(this) )
		{
			if(OPObjectNode* p = FindParent<OPObjectNode>())
			{
				if(p->HasModifier(modifiername))
					return true;
			}
		}
		
		return false;
	}

	// only looks at special modifiers
	virtual bool HasModifier(Token modifiertoken)
	{
		if(modifiers)
		{
			if(modifiers->HasModifier(modifiertoken))
				return true;
		}
		
		//this handles visibility...
		if(automodifiers)
		{
			if(automodifiers->HasModifier(modifiertoken))
				return true;
		}
		
		//handle parent modifiers...
		if( !node_cast<OPObjectNode>(this) )
		{
			if(OPObjectNode* p = FindParent<OPObjectNode>())
			{
				if(p->HasModifier(modifiertoken))
					return true;
			}
		}
		
		return false;
	}

	virtual opNode* GetVisibility(const opString& name)
	{
		return NULL;
	}

	// calls to GetValuedModifier will look at current modifiers, but also
	// attempt to fetch automatically generated modifiers
	virtual ValuedModifierNode* GetValuedModifier(const opString& modifiername)
	{
		if(ValuedModifierNode* node = FetchValueModifier(modifiername))
		{
			return node;
		}		
		
		if(modifiers)
		{
			if(ValuedModifierNode* node = modifiers->GetValuedModifier(modifiername))
				return node;
		}

		//handle parent modifiers...
		if( !node_cast<OPObjectNode>(this) )
		{
			if(OPObjectNode* p = FindParent<OPObjectNode>())
			{
				return p->GetValuedModifier(modifiername);
			}
		}
		
		return NULL;
	}

	opNode* FetchModifier(const opString& name)
	{
		return ModifierSupportBase::FetchModifier(name);
	}

	void FetchAllModifiers()
	{
		return ModifierSupportBase::FetchAllModifiers();
	}

protected:
	void CreateModifiersNode()
	{
		if(automodifiers)
			return;

		stacked<AutoModifiersNode> modnode = NEWNODE(AutoModifiersNode);

		modnode->CopyBasics(this);

		automodifiers = *modnode;

		AppendNode(modnode);
	}
};

template<class Parent>
class ConditionalSupport : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ConditionalSupport);
	
	void Init()
	{
		Condition = NULL;
	}
	
	//returns how many levels printed
	int PrintConditions(opSectionStream& section)
	{
		if(Condition)
			return Condition->PrintCondition(section);
		return 0;
	}
	
	void PrintConditionEnd(int number, opSectionStream& stream)
	{
		if(!number)
			return;
		
		stream << endl;

		for(int i = 0; i < number; i++)
			stream << "#endif" << endl;
	}
	
	void SetCondition(PreprocessorStatementNode* condition)
	{
		Condition = condition;
	}
	
private:
	PreprocessorStatementNode* Condition;
};

}//end of namespace interfaces

