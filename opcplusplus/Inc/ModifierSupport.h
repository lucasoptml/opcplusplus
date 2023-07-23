
#pragma once

namespace nodes
{

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

		TerminalNode* AddBasicModifier(const opString& modifiername, Token token);
		ValuedModifierNode* AddValueModifier(const opString& modifiername);

		virtual bool			    HasModifier(const opString& modifiername) = NULL;
		virtual bool			    HasModifier(Token modifiertoken) = NULL;
		virtual ValuedModifierNode* GetValuedModifier(const opString& modifiername) = NULL;

	protected:
		virtual void CreateModifiersNode() = NULL;

		ModifiersNode* modifiers;
		AutoModifiersNode* automodifiers;

	public:
		// you must fetch modifier values using this, it runs through a cached map structure
		opNode* FetchModifier(const opString& modifiername)
		{
			ModifierMap::iterator end = ModifierGenerators.end();

			ModifierMap::iterator found = ModifierGenerators.Find(modifiername);

			if (found != end)
			{
				if ((*found).second.CachedModifier != NULL)
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

			while (it != end)
			{
				if ((*it).second.CachedModifier != NULL)
				{
					++it;
					continue;
				}

				const opString& modifiername = (*it).first;

				//get the result from the generate modifier delegate
				opNode* result = (*it).second.GenerateModifier(modifiername);

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

			ModifierGenerators.Insert(modifiername, generator);
		}

		void RegisterBasicModifier(const opString& modifiername)
		{
			ModifierDelegate modifier(this, &ModifierSupportBase::ModifierBasic);
			RegisterModifier(modifiername, modifier);
		}

		opNode* ModifierBasic(const opString& name)
		{
			return AddBasicModifier(name, T_MODIFIER);
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
			opNode* CachedModifier;
		};

		typedef opMap< opString, ModifierCache > ModifierMap;
		ModifierMap								 ModifierGenerators;

	};

}

namespace interfaces
{

	template<NodeType Parent>
	class ModifierSupport : public Parent, public ModifierSupportBase
	{
	public:
		IMPLEMENTS_INTERFACE(ModifierSupport);

		void Init()
		{
			InitModifierSupport();
		}

		// looks at auto and specified modifiers
		virtual bool HasModifier(const opString& modifiername);

		// only looks at special modifiers
		virtual bool HasModifier(Token modifiertoken);

		virtual opNode* GetVisibility(const opString& name)
		{
			return NULL;
		}

		// calls to GetValuedModifier will look at current modifiers, but also
		// attempt to fetch automatically generated modifiers
		virtual ValuedModifierNode* GetValuedModifier(const opString& modifiername);

		opNode* FetchModifier(const opString& name)
		{
			return ModifierSupportBase::FetchModifier(name);
		}

		void FetchAllModifiers()
		{
			return ModifierSupportBase::FetchAllModifiers();
		}

	protected:
		void CreateModifiersNode();
	};





}//end of namespace interfaces
