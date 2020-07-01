///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Namespaces.h
/// Date: 03/03/2007
///
/// Description:
///
/// Put all namespace-related stuff in here.
///****************************************************************

namespace nodes
{
	// forward declared node types
	class opNode;
	class opNodeBase;
	class TerminalNode;
	class FileNode;
	class OPIncludeNode;
	class OPMacroNode;
	class OPObjectNode;
	class StatementNode;
	class OPEnumStatementNode;
	class OPObjectStatementNode;
	class FunctionDefinitionStatementNode;
	class ConstructorDefinitionStatementNode;
	class ConstructorStatementBase;
	class DestructorStatementBase;
	class DestructorDefinitionStatementNode;
	class StateStatementNode;
	class DataStatementNode;
	class ParenBlockNode;
	class BraceBlockNode;
	class BracketBlockNode;
	class StatementModifierBase;
	class ValuedModifierNode;
	class ScopeNode;
	class ExpandCallArgumentNode;
	class ExpandCallArgumentListNode;
	class FunctionArgumentDefinitionNode;
	class FunctionArgumentDefaultNode;
	class StatementBase;
	class DataStatementBase;
	class FunctionStatementBase;
	class ModifierSupportBase;
	class CPlusPlusNode;
	class CPlusPlusBodyNode;
	class PreprocessorStatementNode;
}

namespace dialect_nodes
{
	class CategoryBodyNode;
	class CategoryMapBodyNode;
	class NoteNode;
	class CategoryMapNode;
	class CategoryLocationNode;
	class CategoryNode;
	class EnumerationBodyNode;
	class CategoryLocationBodyNode;
	class EnumerationLocationBodyNode;	
	class ModifierNodeBase;
	class DataModifierNode;
	class FunctionModifierNode;
	class NoteDefinitionNode;
	class CategoryMapNode;
	class DialectStatementBase;
	class CriteriaArgumentNode;
	class CriteriaNodeBase;
	class DisallowNode;
	class IsnotBodyNode;
	class IsBodyNode;
	class EnumerationNode;
	class EnumerationLocationNode;
	class EnumerationLocationBodyNode;
	class LocationNodeBase;
	class EnumerationMapNode;
	class ExtendPointNode;
	class ExtensionNode;
	class ExtensionPointNode;
	class FileDeclarationNode;
	class FileDeclarationLocationNode;
	class FileDeclarationLocationStatementNode;
	class DialectModifiersNode;
	class PrefixNodeBase;
	class ClassPrefixNode;
	class StructPrefixNode;
	class EnumPrefixNode;
	class OverrideNode;
	class OrderModifierNodeBase;
	class BeforeNode;
	class AfterNode;
}

namespace interfaces
{
	//this namespace contains all interfaces in opcpp
}

namespace memory
{
	class NewHandler;
}

namespace exceptions
{
	class setexceptionmode;
}

namespace errors
{
	struct AlterContext;
	class opError;
}

namespace scanner
{

}

namespace timing
{

}

namespace parameters
{

}

namespace dialect
{
	struct TypePath;
	struct LocationPath;
	struct TagPath;
	struct SubTagPath;
	struct DialectNote;
	struct DialectMap;
	struct DialectMapExpression;
	struct DialectExpressionMap;
	struct DialectLocation;
	struct DialectCategory;
	struct DialectEnumMap;
}

namespace walkers
{
	class opMacroExpander;
}

namespace visitors
{
	class opVisitor;
}

namespace demo
{

}

namespace beta
{

}

namespace delegates
{

}

namespace modifiers
{

}

class opSymbolTracker;

using namespace modifiers;
using namespace exceptions;
using namespace nodes;
using namespace dialect_nodes;
using namespace interfaces;
using namespace memory;
using namespace errors;
using namespace scanner;
using namespace timing;
using namespace parameters;
using namespace dialect;
using namespace walkers;
using namespace visitors;
using namespace demo;
using namespace beta;
using namespace delegates;

