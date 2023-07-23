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

#pragma once

namespace nodes
{

	///
	/// ModifiersBase
	///

	class ModifiersBase : public opNode
	{
	public:
		DECLARE_NODE(ModifiersBase, opNode, T_UNKNOWN);

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
		TerminalNode* FindModifier(Token modifiertoken);
		ValuedModifierNode* GetValuedModifier(const opString& modifiername);
		void			    BuildValueModifiers(vector<ValuedModifierNode*>& modifierlist);

		opNode* GetModifier(Token modtoken);

		bool PostProcess()
		{
			return true;
		}

		void CloneNode(ModifiersBase* node)
		{
			int num = (int)Modifiers.size();
			for (int i = 0; i < num; i++)
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
		DECLARE_NODE(ModifiersNode, ModifiersBase, G_MODIFIERS);

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
		DECLARE_NODE(AutoModifiersNode, ModifiersNode, G_AUTO_MODIFIERS);
	};

}
