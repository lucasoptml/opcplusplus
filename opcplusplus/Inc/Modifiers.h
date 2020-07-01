///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Modifiers.h
/// Date: 10/10/2007
///
/// Description:
///
/// Modifier listings
///****************************************************************

namespace modifiers
{
	struct ModifierSet
	{
		void GetBasicModifiers(opArray<opString>& modifiers)
		{
			copy(BasicModifiers.begin(),BasicModifiers.end(),back_inserter(modifiers));
		}

		void GetValueModifiers(opArray<opString>& modifiers)
		{
			Set::iterator end = ValueModifiers.end();
			Set::iterator it = ValueModifiers.begin();

			while(it != end)
			{
				modifiers.push_back((*it) + "()");
				++it;
			}
		}

		void GetModifiers(opArray<opString>& modifiers)
		{
			GetBasicModifiers(modifiers);
			GetValueModifiers(modifiers);
		}

	protected:
		typedef opSet<opString> Set;
		
		Set		ValueModifiers;
		Set		BasicModifiers;
	};

	struct DataModifierSet : ModifierSet
	{
		DataModifierSet();

		static bool HasValueModifier(const opString& mod)
		{
			return Instance.ValueModifiers.Contains(mod);
		}
		
		static bool HasBasicModifier(const opString& mod)
		{
			return Instance.BasicModifiers.Contains(mod);
		}

		static void GetModifiers(opArray<opString>& modifiers)
		{
			Instance.ModifierSet::GetModifiers(modifiers);
		}

	private:
		static DataModifierSet Instance;
	};

	struct FunctionModifierSet : ModifierSet
	{
		FunctionModifierSet();

		static bool HasValueModifier(const opString& mod)
		{
			return Instance.ValueModifiers.Contains(mod);
		}

		static bool HasBasicModifier(const opString& mod)
		{
			return Instance.BasicModifiers.Contains(mod);
		}

		static void GetModifiers(opArray<opString>& modifiers)
		{
			Instance.ModifierSet::GetModifiers(modifiers);
		}

	private:
		static FunctionModifierSet Instance;
	};

	struct CategoryModifierSet : ModifierSet
	{
		CategoryModifierSet();

		static bool HasValueModifier(const opString& mod)
		{
			return Instance.ValueModifiers.Contains(mod);
		}

		static bool HasBasicModifier(const opString& mod)
		{
			return Instance.BasicModifiers.Contains(mod);
		}

		static void GetModifiers(opArray<opString>& modifiers)
		{
			Instance.ModifierSet::GetModifiers(modifiers);
		}

	private:
		static CategoryModifierSet Instance;
	};


	struct StatementModifierSet : ModifierSet
	{
		StatementModifierSet();

		static bool HasValueModifier(const opString& mod)
		{
			return Instance.ValueModifiers.Contains(mod);
		}

		static bool HasBasicModifier(const opString& mod)
		{
			return Instance.BasicModifiers.Contains(mod);
		}

		static void GetModifiers(opArray<opString>& modifiers)
		{
			Instance.ModifierSet::GetModifiers(modifiers);
		}

	private:
		static StatementModifierSet Instance;
	};

	struct EnumModifierSet : ModifierSet
	{
		EnumModifierSet();

		static bool HasValueModifier(const opString& mod)
		{
			return Instance.ValueModifiers.Contains(mod);
		}

		static bool HasBasicModifier(const opString& mod)
		{
			return Instance.BasicModifiers.Contains(mod);
		}

		static void GetModifiers(opArray<opString>& modifiers)
		{
			Instance.ModifierSet::GetModifiers(modifiers);
		}

	private:
		static EnumModifierSet Instance;
	};

	struct ModifierDescriptions
	{
		ModifierDescriptions();	

		static bool GetArgument(const opString& name, opString& description)
		{
			DescMap::const_iterator i = Instance.Arguments.Find(name);

			if (i != Instance.Arguments.End())
			{
				description = i->second;
				return true;
			}

			return false;
		}

		static bool GetModifier(const opString& name, opString& description)
		{
			DescMap::const_iterator i = Instance.Modifiers.Find(name);
			
			if (i != Instance.Modifiers.End())
			{
				description = i->second;
				return true;
			}
			
			return false;
		}
		
		static bool GetValuedModifier(const opString& name, opString& description)
		{
			DescMap::const_iterator i = Instance.ValuedModifiers.Find(name);
			
			if (i != Instance.ValuedModifiers.End())
			{
				description = i->second;
				return true;
			}
			
			return false;
		}
		
		void AddBasicDescription(const opString& name, const opString& description)
		{
			Modifiers.Insert(name,description);
		}
		
		void AddValuedDescription(const opString& name, const opString& description)
		{
			Arguments.Insert(name,description);
			ValuedModifiers.Insert(name + "()",description);
		}
		
		void AddArgumentDescription(const opString& name, const opString& description)
		{
			Arguments.Insert(name,description);
		}

	private:
		typedef opMap<opString, opString> DescMap;
		
		DescMap Arguments;
		DescMap Modifiers;
		DescMap ValuedModifiers;

		static ModifierDescriptions Instance;
	};
}




//NOTE: seemed like a good idea - the only problem is implementing conditions on modifiers

/*
namespace modifiers
{
// 	enum Application
// 	{
// 		Data,
// 		Function,
// 		Category,
// 		Enum,
// 		All
// 	};
// 
// 	enum Type
// 	{
// 		Valued,
// 		Basic,
// 		None,
// 	}
// 
// 	enum ArgumentMode
// 	{
// 		Automatic,
// 		Specified,
// 	}
// 
// 	enum NoteUsage
// 	{
// 		Start,
// 		End,
// 		Mapping,
// 		Value,
// 		Name,
// 		Note,
// 	}
// 
// template< class Caller, class Target, Application app, Type type, ArgumentMode mode, NoteUsage usage >
// struct FilterCalls
// {
// 	FilterCalls(Caller* c) : caller(c)
// 	{
// 		modifiers::QueryModifiers(*this);
// 	}
// 
// 	template<class Target, Application inapp, Type intype, ArgumentMode inmode, NoteUsage inusage>
// 	void getinfo()
// 	{
// 	}
// 
// 	template<class Target>
// 	void getinfo<app,type,mode,usage>(const char* name)
// 	{
// 		caller->GotModifier(name);
// 	}
// 
// 	template<class Target,class T, Application inapp, Type intype, ArgumentMode inmode, NoteUsage inusage>
// 	void getinfo( opNode* (T::*function)(const opString&) )
// 	{
// 	}
// 
// 	template<class Target,class T>
// 	void getinfo<app,type,mode,usage>( const char* name, opNode* (T::*function)(const opString&) )
// 	{
// 		caller->GotModifier(name,function);
// 	}
// 
// private:
// 	Caller* caller;
// };
// 
// template< class Caller, class Target, Application app, Type type, ArgumentMode mode, NoteUsage usage >
// struct FilterCallsTarget
// {
// 	FilterCallsTarget(Caller* c) : caller(c)
// 	{
// 		modifiers::QueryModifiers(*this);
// 	}
// 
// 	template<class Target, Application inapp, Type intype, ArgumentMode inmode, NoteUsage inusage>
// 	void getinfo()
// 	{
// 	}
// 
// 	template<class Target>
// 	void getinfo<app,type,mode,usage>(const char* name)
// 	{
// 		caller->GotModifier(name);
// 	}
// 
// 	template<class Target,class T, Application inapp, Type intype, ArgumentMode inmode, NoteUsage inusage>
// 	void getinfo( opNode* (T::*function)(const opString&) )
// 	{
// 	}
// 
// 	template<class Target,class T>
// 	void getinfo<app,type,mode,usage>( const char* name, opNode* (T::*function)(const opString&) )
// 	{
// 		caller->GotModifier(name,function);
// 	}
// 
// private:
// 	Caller* caller;
// };

template< class Type >
class RegisterModifiers : public Type::Super
{
	typedef opNode* (Type::*myfunctiontype)(const opString&);

	void RegisterAllModifiers()
	{
		QueryModifiers(this);
	}

	template< class Type >
	void getinfo(const char* name)
	{
	}

	//if it doesn't match the type we ignore it
	template< class Type, class functionT >
	void getinfo(const char* name, functionT* myfunction )
	{
	}

	//if it matches the type we want to register it!
	template<>
	void getinfo<Type,myfunctiontype>(const char* name, myfunctiontype function)
	{
		T* typedthis = (T*)this;
		ModifierDelegate delegate(typedthis,function);
		RegisterModifier(name,delegate);
	}
};

//TODO: 1. add all the modifiers into this listing (from registrations)
//		2. inherit from the registermodifiers type on all the statements...
//		3. now extend this listing to allow you to grab automatic arguments from it
//		4. 

//1. replace DataStatementNode's stuff

template< class T > void QueryModifiers(T& visitor)
{
	visitor.getinfo("blah");
	visitor.getinfo<DataStatementNode>("data_type",&DataStatementNode::ModifierDataType);
	visitor.getinfo<DataStatementNode>("data_type",&DataStatementNode::ModifierDataType);
	visitor.getinfo<DataStatementNode>("data_type",&DataStatementNode::ModifierDataType);
	visitor.getinfo<DataStatementNode>("data_type",&DataStatementNode::ModifierDataType);
	visitor.getinfo<DataStatementNode>("data_type",&DataStatementNode::ModifierDataType);
	visitor.getinfo<DataStatementNode>("data_type",&DataStatementNode::ModifierDataType);
	visitor.getinfo<DataStatementNode>("data_type",&DataStatementNode::ModifierDataType);
	visitor.getinfo<DataStatementNode>("data_type",&DataStatementNode::ModifierDataType);
	visitor.getinfo<DataStatementNode>("data_type",&DataStatementNode::ModifierDataType);
	visitor.getinfo<DataStatementNode>("data_type",&DataStatementNode::ModifierDataType);
}

}

*/
