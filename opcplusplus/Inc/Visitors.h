//****************************************************************
// Copyright © 2008 opGames LLC - All Rights Reserved
//
// Authors: Kevin Depue & Lucas Ellis
//
// File: Visitors.h
// Date: 02/03/2008
//
// Description:
//
// Definition of base visitor classes.
//****************************************************************

namespace visitors
{

//==========================================
// opVisitor
//
// Base visitor class.
//==========================================

class opVisitor
{
public:

	/*=== construction ===*/

	opVisitor()
		: bFailed(false)
	{

	}

	/*=== visiting methods ===*/

	// NOTE: This is totally gay/lamo, but necessary.
	//       For each node type that you want to be
	//       able to visit via enter/leave, you have 
	//       to define a macro for the node type here.
	//       If only we could use opC++ to write the
	//       opC++ compiler!!  :( 

	DECLARE_VISIT(opNode)
	DECLARE_VISIT(CategoryNode)
	DECLARE_VISIT(EnumerationNode)
	DECLARE_VISIT(NoteDefinitionNode)
	DECLARE_VISIT(FileDeclarationNode)
	DECLARE_VISIT(ExtensionNode)
	DECLARE_VISIT(ExtensionPointNode)
	DECLARE_VISIT(ExtendPointNode)

	/*=== get ===*/

	void SetFailed(bool inbool)
	{
		bFailed = inbool;
	}

	bool Failed() const
	{
		return bFailed;
	}

private:

	/*=== data ===*/

	bool bFailed;
};

} // end namespace visitors