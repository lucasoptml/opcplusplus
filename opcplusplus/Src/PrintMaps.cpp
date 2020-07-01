///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: PrintMaps.cpp
/// Date: 12/30/2006
///
/// Description:
///
/// Map Printing Source File
///****************************************************************

#include "opCPP.h"


//
// Print Parts..
//

void FunctionDefinitionStatementNode::PrintName(opSectionStream& stream)
{
	opNode* name = FunctionDefinition->GetFunction()->GetName();
	name->PrintOriginal(stream);
}

void FunctionDefinitionStatementNode::PrintArguments(opSectionStream& stream)
{
	FunctionDefinition->GetFunction()->GetArguments()->PrintDefaultless(stream);

}

void FunctionDefinitionStatementNode::PrintReturnType(opSectionStream& stream)
{
	opNode* ret = FunctionDefinition->GetReturn();
	ret->PrintOriginal(stream);
}

//oops this is weird
opString FunctionDefinitionStatementNode::GetSignatureString()
{
	opString signature;
	signature.Reserve(128);
	
	//TODO: it really needs to grab built in modifiers as well.
	if(GetModifiers())
		GetModifiers()->PrintBuiltInReturnArgument(signature);

	//get the return type
	GetFunctionDefinition()->GetReturn()->PrintString(signature);
	signature += ' ';
	//get the function name
	GetFunctionDefinition()->GetFunction()->GetName()->PrintString(signature);
	//get the argument signature
	GetFunctionDefinition()->GetFunction()->GetArguments()->PrintSignature(signature);

	if(GetFunctionDefinition()->GetFunction()->IsConst())
		signature += " const";

	return signature;
}


void FuncPrototypeStatementNode::PrintMacroName(opSectionStream& stream)
{
	opNode* name = FunctionPrototype->GetFunction()->GetName();
	name->PrintOriginal(stream);
}

void FuncPrototypeStatementNode::PrintMacroReturnType(opSectionStream& stream)
{
	opNode* ret = FunctionPrototype->GetReturn();
	ret->PrintOriginal(stream);
}

void FuncPrototypeStatementNode::PrintMacroArguments(opSectionStream& stream)
{
	FunctionArgumentListNode* args = FunctionPrototype->GetFunction()->GetArguments();
	args->PrintOriginal(stream);
}


//
// Settings Based opObject Printing
//


void OPObjectNode::PrintVisibility(VisibilityMode mode, opSectionStream& stream)
{
	if(mode == vismode_none)
		return;
	
	DialectCategory* category = GetCategorySettings();
	
	if(mode == vismode_public)
	{
		stream << endl << "public:" << endl;
	}
	else if(mode == vismode_private)
	{
		stream << endl << "private:" << endl;
	}
	else if(mode == vismode_protected)
	{
		stream << endl << "protected:" << endl;
	}
	else if(mode == vismode_default)
	{	
		DialectNote* note = NULL;
		
		note = &category->GetDefaultNote();
		
		stream.PrintComment("//default note: " + category->GetName() + "::" + note->GetName());
		stream << endl;
		PrintNote(*note,stream);
	}
	
}
