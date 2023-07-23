///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: UtilityInterfaces.h
/// Date: 01/04/2007
///
/// Description:
///
/// Utility interfaces.
///****************************************************************

#pragma once

namespace interfaces
{

///==========================================
/// RemoveWhitespace interface
///==========================================

template<class Parent>
class RemoveWhitespace : public Parent
{
public:
	IMPLEMENTS_INTERFACE(RemoveWhitespace)

	bool Parse();

	void removeWhitespace()
	{
		iterator i   = this.GetBegin();
		iterator end = this.GetEnd();

		while (i != end)
		{
			Token tok = i->GetId();

			if (tok == T_WHITESPACE || tok == T_NEWLINE)
			{
				iterator newi = i;
				++i;
				this.DeleteNode(newi);
				continue;
			}

			++i;
		}
	}
};

///==========================================
/// RemoveComments interface
///==========================================

template<class Parent>
class RemoveComments : public Parent
{
public:
	IMPLEMENTS_INTERFACE(RemoveComments)

	bool Parse();

	void removeComments()
	{
		iterator i   = this.GetBegin();
		iterator end = this.GetEnd();

		while (i != end)
		{
			Token tok = i->GetId();

			if (tok == T_CCOMMENT || tok == T_COMMENT)
			{
				iterator newi = i;
				++i;
				this.DeleteNode(newi);
				continue;
			}

			++i;
		}
	}

};

///==========================================
/// Clean interface
///==========================================

template<class Parent>
class Clean : public RemoveWhitespace< RemoveComments< Parent > >
{
public:
	IMPLEMENTS_INTERFACE(Clean)

	bool Parse();

	void CleanAll()
	{
		this.removeComments();
		this.removeWhitespace();
	}
};

///==========================================
/// Trim
///==========================================

template<class Parent>
class Trim : public Parent
{
public:
	IMPLEMENTS_INTERFACE(Trim)

	bool PreParse();
	
	bool Parse();
	
	void DoTrim();
	
};

///==========================================
/// ListUtility
///==========================================

template<class Parent>
class ListUtility : public Parent
{
public:
	IMPLEMENTS_INTERFACE(ListUtility)

	// makes a list, tokenized on delimiter..
	template<Token Delimiter, class ArgumentType>
	void MakeDelimiterList(vector<ArgumentType*>& container)
	{
		//NOTE: only want to build this list once, if its non-empty assume it was built.
		if(container.size())
			return;

		this.ResetPosition();

		if (!this.IsEmpty())
		{
			while(1)
			{
				stacked<ArgumentType> arg = opNode::PushUntil<ArgumentType>(Delimiter,false);

				if(arg->IsEmpty())
					arg->CopyBasics(this);

				if(this.IsCurrent(Delimiter))
				{
					this.Erase(Delimiter);
					container.push_back(*arg);
				}
				else
				{
					container.push_back(*arg);
					break;
				}
			}

			int size = (int) container.size();

			for (int i = 0; i < size; i++)
			{
				stacked<opNode> tempstacked = stacked<opNode>::buildstacked(container[i]);
				this.AppendNode(tempstacked);
			}
		}

		this.ResetPosition();
	}

	// delimiter specific functions
	template<class ArgumentType>
	void MakeCommaList(vector<ArgumentType*>& container)
	{
		this.MakeDelimiterList<T_COMMA>(container);
	}
};

template<class Parent>
class UnInlineSupport : public Parent
{
	IMPLEMENTS_INTERFACE(UnInlineSupport);

	bool GetInlineMode()
	{
		//if inline, then inline
		bool binline = this.HasModifier(T_INLINE);

		//if uninline, then uninline
		bool buninline = this.HasModifier(T_UNINLINE);

		//if inline and uninline, then uninline
		bool doinline = binline && !buninline;

		//if none, then use parameters
		if(!binline && !buninline)
			doinline = opParameters::Get().InlineAll;

		return doinline;
	}
};




} // end namespace interfaces

