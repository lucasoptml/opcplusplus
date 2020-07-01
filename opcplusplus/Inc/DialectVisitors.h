//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: DialectVisitors.h
// Date: 02/03/2008
//
// Description:
//
// Dialect-related visitors.
//****************************************************************

namespace visitors
{

//==========================================
// DialectRegistrationVisitor
//==========================================

class DialectRegistrationVisitor : public opVisitor
{
public:

	/*=== visiting methods ===*/

	// enter a category
	bool enter(CategoryNode* node)
	{
		DialectCategory* categoryinfo = DialectTracker::Register(node);

		node->GetBody()->Register(categoryinfo);

		return false;
	}

	// enter an enumeration
	bool enter(EnumerationNode* node)
	{
		DialectEnumeration* enuminfo = DialectTracker::Register(node);

		node->GetBody()->Register(enuminfo);

		return false;
	}

	// enter a note definition
	bool enter(NoteDefinitionNode* node)
	{
		DialectTracker::Register(node);

		return true;
	}

	// enter a file declaration
	bool enter(FileDeclarationNode* node)
	{
		DialectTracker::Register(node);

		return false;
	}

	// enter an extension
	bool enter(ExtensionNode* node)
	{
		DialectTracker::Register(node);

		return false;
	}

	// enter an extension point
	bool enter(ExtensionPointNode* node)
	{
		DialectTracker::Register(node);

		return false;
	}

	// enter an extendpoint 
	bool enter(ExtendPointNode* node)
	{
		DialectTracker::Register(node);

		return false;
	}
};

} // end namespace visitors