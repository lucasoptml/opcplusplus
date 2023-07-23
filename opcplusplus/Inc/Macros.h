///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Macros.h
/// Date: 01/04/2007
///
/// Description:
///
/// File containing all opcpp compiler-related macros.
///****************************************************************

///==========================================
/// Parsing macros
///==========================================

#define CATCH_EXCEPTIONS							\
	}												\
	catch (opException::opCPP_Exception&)			\
	{												\
	exceptionhandler.CaughtException();			\
	return false;								\
	}												\
	catch (opException::opCPP_ParseException&)		\
	{												\
	exceptionhandler.CaughtException();			\
	return false;								\
	}								

#define INIT_EXCEPTIONS							\
	ExceptionCatchHandler exceptionhandler;		\
	try											\
	{

///==========================================
/// opNode macros
///==========================================

#define DECLARE_NODE_BASE(typ,supr,grammar,file,linenum)													\
	public:																									\
	typedef supr Super;																						\
	typ()																									\
	{																										\
		SetId(grammar);																						\
		Init();																								\
	}																										\
	static opString GetNodeTypeFile() {return file;}														\
	static int GetNodeTypeLine() {return linenum;}															\
	static opString StaticGetNodeTypeSource() { return GetNodeTypeFile() + "(" + GetNodeTypeLine() + ")"; }	\
	opString GetNodeTypeSource() { return StaticGetNodeTypeSource(); }										\
	const char* GetNodeType() {return #typ;}																\
	stacked<typ> Clone()																					\
	{																										\
		return CloneProcess<typ>(this);																		\
	}																										\
	stacked<opNode> CloneGeneric()																			\
	{																										\
		return stacked<opNode>::buildstacked(*Clone());														\
	}																										\
	void visit(opVisitor& visitor)																			\
	{																										\
		ExceptionCatchHandler exceptionhandler;																\
		try																									\
		{																									\
			if ( visitor.enter( this ) )																	\
				visit_children( visitor );																	\
			visitor.leave( this );																			\
		}																									\
		catch (opException::opCPP_Exception&)																\
		{																									\
			exceptionhandler.CaughtException();																\
			visitor.SetFailed(true);																		\
		}																									\
	}

//this is a requirement, otherwise PARSE_START will not work correctly.
#define DECLARE_NODE(typ,supr,grammar) DECLARE_NODE_BASE(typ,supr,grammar,__FILE__,__LINE__)

//NEWNODE macro
//this is only for debugging memory allocations
//and eliminating memory leaks
#define NEWNODE(type) NewHandler::NewNode( new type,__FILE__,__LINE__)

///==========================================
/// Interface macros / exceptions
///==========================================

//use these to validate order of interfaces
//use this declaration as many times as needed within interfaces

//NOTE: I had to disable these interfaces for a while :/

#define REQUIRES_INTERFACE(iface)											\
	
// removed enum required_##iface { iface##_requires = iface##_implementation };

//void Requires_##iface() { Implements_##iface(); }

//use this declaration once per interface (within the class)
#define IMPLEMENTS_INTERFACE(iface)						\
public:													\
typedef Parent Super;									\
typedef opList<opNode*>::pointer_iterator iterator;		\
iface<Parent>()											\
{														\
	this.Init();												\
}

// removed:	enum implements_##iface { iface##_implementation };	\


#define PARSE_START							\
	if(!Super::Parse())						\
		return false;						\
	AlterContext newcontext( this->GetId(), this);	\
	this->ResetPosition();						\
	INIT_EXCEPTIONS

#define PARSE_END			\
		return true;		\
	CATCH_EXCEPTIONS

#define PREPARSE_START							\
	AlterContext newcontext(this->GetId(),this);\
	if(!Super::PreParse())						\
		return false;							\
	this->ResetPosition();						\
	INIT_EXCEPTIONS	

#define PREPARSE_END		\
		return true;		\
	CATCH_EXCEPTIONS

//wrap visitor functions - catches exceptions
//function must return false for failure/errors
#define OPERATIONS_START				\
AlterContext newcontext(opNode::GetId(),this);	\
	opNode::ResetPosition();					\
	INIT_EXCEPTIONS		

#define OPERATIONS_END		\
		return true;		\
	CATCH_EXCEPTIONS

#define POSTPARSE_START						\
	AlterContext newcontext(opNode::GetId(),this);	\
	if (!Super::PostParse())				\
		return false;						\
	opNode::ResetPosition();				\
	INIT_EXCEPTIONS

#define POSTPARSE_END		\
		return true;		\
	CATCH_EXCEPTIONS

#define DISALLOW_START						\
	AlterContext newcontext(opNode::GetId(),this);	\
	opNode::ResetPosition();						\
	INIT_EXCEPTIONS

#define DISALLOW_END		\
		return true;		\
	CATCH_EXCEPTIONS

//HIT macro
//NOTE: always remove tok immediately after hit, or else
//		you can get an endless error loop
#define HIT(tok)					\
	if(!opNode::IgnoreUntil<tok>())	\
	{								\
		done = true;				\
	}								\
	else

//loop macros
#define LOOP_START(t)								\
	AlterContext newcontext(t,this);				\
	this->ResetPosition();							\
	bool done = false;								\
	while(!done)									\
		{											\
			ExceptionCatchHandler exceptionhandler; \
			try										\
			{

#define LOOP_END									\
			}										\
			catch(opException::opCPP_Exception&)	\
			{										\
				exceptionhandler.CaughtException();	\
			}										\
		}											\
	this->ResetPosition();

// INSPECT macros
#define INSPECT_START(t)				\
	AlterContext newcontext(t,this);	\
	this->ResetPosition();			

#define INSPECT_END						\
	this->ResetPosition();				\
	return false;

///==========================================
/// Misc macros
///==========================================

// null index
#define INDEX_NONE -1

//windows macro collision fixes
#undef GetClassName

namespace workarounds
{
	//lovely endl that actually has a type
	struct endlinestruct
	{
		inline operator char() const { return '\n'; }	
		
		friend ostream& operator << (ostream& o, const workarounds::endlinestruct& endline)
		{
			return o << '\n';
		}
	};
}

using namespace workarounds;

#define endl endlinestruct()

//assert stuff
#ifdef _DEBUG
	#undef assert
	#define assert(expression) {if(!(expression)) {opPlatform::Assertion();}}
#else
	#undef assert
	#define assert(expression) 
#endif

// to catch abstract function usage
// (even though they're not technically abstract)
#define ABSTRACT_FUNCTION assert(0);

///==========================================
/// Debugging Macros
///==========================================

#ifdef _DEBUG
#define DebugAllocationNumber(number) if(opNode::GetStaticAllocationNumber() == number) opPlatform::Breakpoint();
#endif

// define NULL
#undef NULL
#define NULL 0

// chevrons
#define LCHEV "\253"
#define RCHEV "\273"

//==========================================
// Visitor Macros
//==========================================

#define DECLARE_VISIT(nodetype)			\
	virtual bool enter(nodetype* node)	\
	{									\
		return true;					\
	}									\
	virtual void leave(nodetype* node)	\
	{									\
	}