///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: opNode.h
/// Date: 11/26/2006
///
/// Description:
///
/// This is the base node class.  All grammars inherit from this node.  The
/// class contains all base functionality.
///****************************************************************

#pragma once

///==========================================
/// Base node classes
///==========================================

struct opToken;
class  opSymbolTracker;

namespace nodes
{

class  FileNode;
class  opNode;

///
/// opNodeBase
///

// base class for all opnodes
class opNodeBase
{
	friend opNode;

public:
	/**** typedefs ****/

	typedef opList<opNode*>::pointer_iterator iterator;

	/**** construction / destruction ****/
	
	// constructor
	opNodeBase() 
		: id(T_UNKNOWN), 
		parent(NULL), 
		pos(children.Begin()),
		line(-1), 
		file(NULL) 
	{
	}
	
	// destructor
	virtual ~opNodeBase();
	
	// declare this in subclasses - the default constructor will always call this
	void Init()
	{

	}
	
	/**** get ****/
	
	Token GetId()
	{
		return id;
	}
	
	opNode* GetParent()
	{
		return parent;
	}
	
	int GetLine()
	{
		return line;
	}
	
	const iterator& GetPosition()
	{
		return pos;
	}

	iterator GetBegin()
	{
		return children.Begin();
	}

	iterator GetLast()
	{
		return children.Last();
	}

	iterator GetEnd()
	{
		return children.End();
	}
	
	//don't use if possible, instead use HasNumChildren
	int NumChildren()
	{
		return (int) children.Size();
	}

	bool HasNumChildren(int num)
	{
		return children.HasSize(num);
	}
	
	FileNode* GetFile()
	{
		return file;
	}

	/**** set ****/
	
	void SetId(Token _id)
	{
		id = _id;
	}
	
	void SetParent(opNode* _parent)
	{
		parent = _parent;
	}
	
	void SetLine(int _line)
	{
		line = _line;
	}
		
	void SetFile(FileNode* infile)
	{
		file = infile;
	}
	
	/**** utility ****/
	
	void ResetPosition()
	{
		//if(NumChildren())
		pos = GetBegin();
	}
	
	//use sparingly
	void SetPosition(iterator newpos)
	{
		pos = newpos;
	}

	void IncrementPosition()
	{
		++pos;
	}

	void DecrementPosition()
	{
		--pos;
	}

	bool IsEmpty()
	{
		return !HasNumChildren(1);
	}

	void ClearChildren()
	{
		children.Clear();
	}

	void CopyLineNum(opNode* n);
	void CopyFile(opNode* n);

	// moves nodes from node in range [start,end) into this at index
	//NOTE: watch out for pos getting moved/invalidated when using this!
	void MoveNodes(iterator index, opNode* node, iterator start, iterator end);

	template<class N> void       CollapseNode(stacked<N>& collapse, iterator index);
	template<class N> void       CollapseNodeAtCurrent(stacked<N>& collapse);
	template<class N> void       AppendNode(stacked<N>& newchild);
	template<class N> void		 PrependNode(stacked<N>& newchild);
	template<class N> void       InsertNode(stacked<N>& newChild, iterator index);
	template<class N> void       InsertNodeAtCurrent(stacked<N>& newChild);
	template<class N> stacked<N> PopNode(N* node);
	
	stacked<opNode> PopNode(iterator index);
	stacked<opNode> PopCurrentNode();
	stacked<opNode> PopFirstNode();
	stacked<opNode> PopLastNode();

private:	
	void    AppendNode(opNode* newChild);
	void	PrependNode(opNode* newChild);
	void    InsertNode(opNode* newChild, iterator index);
	void    InsertNodeAtCurrent(opNode* newChild);
	void    RemoveNode(iterator index);
	void    RemoveCurrentNode();
	void    RemoveFirstNode();
	void    RemoveLastNode();

public:
	void     DeleteNode(iterator index);
	void     DeleteNode(opNode* node);
	void     DeleteCurrentNode();
	void     DeleteFirstNode();
	void     DeleteLastNode();
	opNode*  CurrentNode();
	opNode*  PreviousNode();
	opNode*  FirstChild();
	opNode*  LastChild();
	iterator FindNodeIndex(opNode* innode);

protected:
	virtual bool PostParse() { return true; }
	virtual bool PreParse()  { return true; }
	virtual bool Parse()     { return true; }

private:
	Token           id;
	opNode*         parent;

	// list version of children stuff
	opList<opNode*> children;
	iterator		pos;

	int             line;
	FileNode*       file;
};

///
/// opNode
///

//we want expand calls to be available on all interfaces.
//NOTE: we could probably move this out eventually,
//once it's clear which classes exactly need this.

class opNode : public opNodeBase
{
public:
	/**** typedefs ****/

	typedef opNodeBase::iterator iterator;
	typedef opNodeBase			 Super;

	/**** construction / destruction ****/

	opNode() 
	{
#ifdef _DEBUG
		AllocationNumber = 0;
#endif
		SetId(T_OPNODE);
		RegisterNode();
	}

	~opNode()
	{
		UnRegisterNode();
	}

	void Init()
	{

	}

	void PostInit()
	{
		//for checking that ErrorName is defined
		if ( opParameters::Get().DeveloperMode )
			ErrorName();
	}

	/**** memory tracking support ****/

#ifdef _DEBUG
	void SetAllocationLocation(const opString& s)
	{
		AllocationLocation = s;
	}

	const opString& GetAllocationLocation()
	{
		return AllocationLocation;
	}

	int GetAllocationNumber()
	{
		return AllocationNumber;
	}

	static int GetStaticAllocationNumber()
	{
		return StaticAllocationCounter;
	}

protected:
	void RegisterNode()
	{
		AllocationNumber = StaticAllocationCounter;
		StaticAllocationCounter++;
		opMemoryTracker::Register(this);
	}

	void UnRegisterNode()
	{
		opMemoryTracker::UnRegister(this);
	}

private:
	opString AllocationLocation;
	int AllocationNumber;
	static int StaticAllocationCounter;

public:

#else
	inline void RegisterNode() {}
	inline void UnRegisterNode() {}
#endif
	
	void UnRegisterReachable()
	{
		UnRegisterNode();
		
		iterator it = GetBegin();
		iterator end = GetEnd();

		while(it != end)
		{
			it->UnRegisterReachable();
			++it;
		}
	}

	/**** cloning ****/
	
	virtual stacked<opNode> CloneGeneric()
	{
		assert(0);
		return stacked<opNode>(NULL);
	}
	
	void CloneBasics(opNode* newnode)
	{
		newnode->SetId(GetId());		
		newnode->SetLine(GetLine());
		newnode->SetFile(GetFile());
	}
	
	void CloneNode(opNode* newnode)
	{
		UndefinedFunctionError("CloneNode(" + opString(newnode->GetNodeType()) + "*)");
	}
	
	void CloneChildren(opNode* newnode)
	{
		iterator iter = GetBegin();
		iterator end  = GetEnd();

		while (iter != end)
		{
			newnode->AppendNode(*iter->CloneGeneric());

			++iter;
		}
	}
	
	void CopyBasics(opNode* innode)
	{
		CopyLineNum(innode);
		CopyFile(innode);
	}

	void CopyBasicsRecursive(opNode* innode)
	{
		CopyBasics(innode);

		iterator iter = GetBegin();
		iterator end  = GetEnd();

		while (iter != end)
		{
			iter->CopyBasicsRecursive( innode );

			++iter;
		}
	}

	/*=== visitor related ===*/

	virtual void visit(opVisitor& visitor)																			
	{		
		ExceptionCatchHandler exceptionhandler;																
			
		try																									
		{
			if ( visitor.enter( this ) )																		
				visit_children( visitor );	

			visitor.leave( this );																				
		}
		catch (opException::opCPP_Exception&)
		{
			exceptionhandler.CaughtException();

			visitor.SetFailed(true);
		}
	}

	void visit_children(opVisitor& visitor)
	{
		opNode::iterator i   = GetBegin();
		opNode::iterator end = GetEnd();

		while (i != end)
		{
			(*i)->visit( visitor );

			++i;
		}
	}
	
	/**** get ****/
	
	virtual opString GetTreeValue()
	{
		return "";
	}

	//slow, avoid
	opString GetStringValue()
	{
		opString value;
		PrintString(value);
		return value;
	}
	
	//declare-node virtuals / defaults
	virtual const char* GetNodeType()
	{
		ABSTRACT_FUNCTION;
		return "unknown";
	}
	
	virtual opString GetNodeTypeSource()
	{
		return "";
	}

	/**** printing ****/
	
	//Print Node (this node and all children)
	//should usually just call PrintOriginal, PrintTransformed, PrintNodeChildren
	//must define for all classes
	virtual void PrintNode( opFileStream& stream )
	{
		UndefinedFunctionError("PrintNode(opFileStream&)");
	}
	
	//Print Node (for printing to a single stream)
	virtual void PrintDialectNode( opDialectStream& stream )
	{
		UndefinedFunctionError("PrintDialectNode(opDialectStream&)");
	}
	
	virtual void PrintXml( opXmlStream& stream )
	{
		PrintXmlChildren(stream);
	}

	//printing
	
	//print original should print something close to the original input tokens
	virtual void PrintOriginal( opSectionStream& stream )
	{
		PrintOriginalUnspacedChildren(stream);
		
		UndefinedFunctionError("PrintOriginal(opSectionStream&)");
	}
	
	virtual void PrintString(opString& s)
	{
		PrintStringChildren(s);
	
		UndefinedFunctionError("PrintString(opString&)");
	}
	
	//print transformed should print macro wrapped output if available
	//(directly to the same header body location)
	virtual void PrintTransformed( opSectionStream& stream )
	{
		PrintTransformedChildren(stream);
	}
	
protected:
	void PrintNodeChildren( opFileStream& stream );
	void PrintNodeChildren( opDialectStream& stream );
	void PrintOriginalChildren( opSectionStream& stream );
	void PrintOriginalUnspacedChildren( opSectionStream& stream );
	void PrintTransformedChildren( opSectionStream& stream );
	void PrintStringChildren(opString& s);
	void PrintStringUnspacedChildren(opString& s);
	void PrintXmlChildren(opXmlStream& stream);
	
public:
	virtual void MacroPrint(opSectionStream& stream, int& charnum)
	{
		MacroPrintChildren(stream, charnum);
		
		UndefinedFunctionError("MacroPrint");
	}
	
	void MacroPrintChildren(opSectionStream& stream, int& charnum);
	
	/**** pre process ****/
	
	virtual bool PreProcess();
	bool PreProcessChildren();
	
	virtual bool PostProcess();
	bool PostProcessChildren();
	
	virtual bool PreParse();
	virtual bool PostParse();

	/**** process ****/
	
	virtual bool Process();

	bool ProcessChildNodes();

	/**** utility ****/

	virtual bool IsTerminal() 
	{ 
		return false; 
	}

	virtual bool IsGrammar()
	{
		return true;
	}
	
	virtual bool Parse()
	{
		return true;
	}

	virtual bool Verify()
	{
		return false;
	}

	virtual FileNode* ToFileNode()
	{
		return NULL;
	}

	//custom error string name
	virtual opString ErrorName()
	{ 
		if ( opParameters::Get().DeveloperMode )
		{
			if (GetId() != T_OPNODE)
				UndefinedFunctionError("opString ErrorName()");
		}

		return ""; 
	}

	// Logs a necessary undefined function.
	void UndefinedFunctionError(const opString& functionsignature)
	{
		// Only do this in developer mode.
		if ( !opParameters::Get().DeveloperMode )
			return;

		opString classtype = GetNodeType();
		opString classlocation = GetNodeTypeSource();
		Log(classlocation + " : error : " + functionsignature + " not defined for class " + classtype);
	}

	// prints out the AST
	void PrintTree(const opString& filename, int depth = 0);

	bool IsCurrent(Token compare)
	{
		if (GetPosition() == GetEnd())
			return false;

		return GetPosition()->GetId() == compare;
	}

	bool IsPrevious(Token compare)
	{
		if(!IsEmpty())
		{
			if(GetPosition() == GetBegin())
				return false;
			
			iterator current = GetPosition();
			--current;
	
			return current->GetId() == compare;
		}

		return false;
	}
	
	void EatWhitespaceAndComments();
	
	// "is" functions
	bool IsWhitespace();
	bool IsComment();
	bool IsOverloadableOperator();
	bool IsModifier();
	
	bool IsInside(Token t);
	
	//to functions
	virtual StatementBase*				ToStatementBase()		 { return NULL; }
	virtual StatementModifierBase*		ToStatementModifierBase() { return NULL; }
	virtual DataStatementBase*			ToDataStatementBase()	 { return NULL; }
	virtual FunctionStatementBase*		ToFunctionStatementBase() { return NULL; }
	virtual DialectStatementBase*		ToDialectStatementBase()	 { return NULL; }
	virtual PreprocessorStatementNode*	ToPreprocessorStatementNode() { return NULL; }
	virtual ConstructorStatementBase*	ToConstructorStatementBase() { return NULL; }
	virtual DestructorStatementBase*	ToDestructorStatementBase() { return NULL; }
	
	/**** queries ****/
	virtual void BuildClassScope(opString& outscope);
	
	//get the class scope without ending ::
	opString GetClassScope();

public:
	/**** matching functions / utility functions ****/

	template<Token t1>
	bool IgnoreUntil();
	template<Token t1, Token t2>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3, Token t4>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3, Token t4, Token t5>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14>
	bool IgnoreUntil();
	template<Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15>
	bool IgnoreUntil();

	void Erase(Token t);
	void ReverseErase(Token t);

	// disallow - disallows token t in the current context (recursive
	// if specified)
	
	// disallow - nonrecursive version
	void Disallow(Token t)
	{
		DisallowBase(t, false);
	}

	// disallow - recursive version
	void DisallowRecursive(Token t)
	{
		DisallowBase(t, true);
	}

	// PushUntil range version - pushes in the range [position,end)
	template<class T> stacked<T> PushUntil(iterator end);

	// Push Until
	// Adds nodes to an existing node until one of the tokens in t is reached.  Throws an
	// error when EOF or EOB is reached, if so desired.

	// pushuntil for 1 token
	void PushUntilAdd(opNode* addto, Token t, bool bErrorOnMissing = true);

	// pushuntilor - nongeneration versions
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, bool bErrorOnMissing = true);
	void PushUntilOrAdd(opNode* addto, Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, bool bErrorOnMissing = true);

	// pushuntil for 1 token - generation version
	template<class T> stacked<T> PushUntil(Token t, bool bErrorOnMissing = true);

	// pushuntilor - generation versions
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, Token t16, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, Token t16, Token t17, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, Token t16, Token t17, Token t18, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, Token t16, Token t17, Token t18, Token t19, bool bErrorOnMissing = true);
	template<class T> stacked<T> PushUntilOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, Token t16, Token t17, Token t18, Token t19, Token t20, bool bErrorOnMissing = true);

	// push until end
	template<class T> stacked<T> 
	PushUntilEnd();

	// push until end - opNode version
	stacked<opNode> PushUntilEnd();

	// takes the children of one node and makes them
	// children of a new node of type T
	template<class T, class N> 
	stacked<T> Transform(stacked<N>& node);

	// expect method (removes node from children list)
	template<class T> inline stacked<T> Expect(Token t)
	{
		//if its a grammar
		//compile time assert: verify this is true
		//TODO: test this
		//assert(!tokenmapping::NodeTypeIsGrammar<T>() || tokenmapping::NodeTypeToken<T>::id == t);
		vector<Token> tokens;
		tokens.push_back(t);

		return ExpectBase<T, true>(tokens);
	}
	
	// expect or - allows the user to expect token A or token B or ... etc.
	template<class T> stacked<T> ExpectOr(Token t1, Token t2);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3, Token t4);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14);
	template<class T> stacked<T> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15);

	// expect - non-template
	stacked<opNode> Expect(Token t)
	{
		vector<Token> tokens;
		tokens.push_back(t);

		return ExpectBase<opNode, true>(tokens);
	}

	// expect or - non-template
	stacked<opNode> ExpectOr(Token t1, Token t2);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3, Token t4);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14);
	stacked<opNode> ExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15);

	// reverse expect
	template<class T> inline stacked<T> ReverseExpect(Token t, opNode* after = NULL)
	{
		vector<Token> tokens;
		tokens.push_back(t);
		return ExpectBase<T, true>(tokens, true, after);
	}

	// reverse expect or
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, opNode* after = NULL);
	template<class T> stacked<T> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, opNode* after = NULL);

	inline stacked<opNode> ReverseExpect(Token t, opNode* after = NULL)
	{
		vector<Token> tokens;
		tokens.push_back(t);
		return ExpectBase<opNode, true>(tokens, true, after);
	}

	// reverse expect or
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, opNode* after = NULL);
	stacked<opNode> ReverseExpectOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15, opNode* after = NULL);

	// special Expect for overload-able operators
	opNode* CheckOverloadableOperator();

	// check method (calls expect node, but doesn't remove
	// from children list)
	template<class T> T* Check(Token t)
	{
		vector<Token> tokens;
		tokens.push_back(t);
		return *ExpectBase<T, false>(tokens);
	}

	// check or - allows the user to check token A or token B or ... etc.
	//			  without pulling them off the node tree
	template<class T> T* CheckOr(Token t1, Token t2);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3, Token t4);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14);
	template<class T> T* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15);
	
	// check - non-template version
	opNode* Check(Token t)
	{
		vector<Token> tokens;
		tokens.push_back(t);

		return *ExpectBase<opNode, false>(tokens);
	}
	
	// check or - non-template
	opNode* CheckOr(Token t1, Token t2);
	opNode* CheckOr(Token t1, Token t2, Token t3);
	opNode* CheckOr(Token t1, Token t2, Token t3, Token t4);
	opNode* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5);
	opNode* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6);
	opNode* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7);
	opNode* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8);
	opNode* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9);
	opNode* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10);
	opNode* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11);
	opNode* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12);
	opNode* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13);
	opNode* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14);
	opNode* CheckOr(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15);
	
	// checks to make sure there are no more non-whitespace tokens
	// if there are, it throws an exception
	void CheckNone();
	
	// allow only methods : up to 12 arguments
	void AllowOnly(Token t);
	void AllowOnly(Token t1, Token t2);
	void AllowOnly(Token t1, Token t2, Token t3);
	void AllowOnly(Token t1, Token t2, Token t3, Token t4);
	void AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5);
	void AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6);
	void AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7);
	void AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8);
	void AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9);
	void AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10);
	void AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11);
	void AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12);
	void AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13);
	void AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14);
	void AllowOnly(Token t1, Token t2, Token t3, Token t4, Token t5, Token t6, Token t7, Token t8, Token t9, Token t10, Token t11, Token t12, Token t13, Token t14, Token t15);
	
	// disallow both - only one of the two types is allowed
	void DisallowBoth(Token t1, Token t2);

	// Make method
	// make a new node based on a node at pos (w/ checking)
	template<class T> 
	stacked<T> Make(Token t);
	
	// peek function - allows you to peek ahead
	// in the children array PeekAhead times
	bool Peek(Token t);
	bool PeekUncleaned(Token t);
	bool PeekStart(Token t);
	bool PeekEnd(Token t);
	
	//find nearest parent of type T
	//assumes a parent of this type exists
	template<class T>
	T* FindParent();
	
	//find the first node of type T
	template<class T>
	T* FindNode();

	//create node functions
	TerminalNode* AppendTerminalNode(const opString& stringvalue, Token tokenid);
	TerminalNode* PrependTerminalNode(const opString& stringvalue, Token tokenid);
	
private:
	/*=== utility base methods ===*/

	void DisallowBase(Token t, bool bRecursive, opNode* inside = NULL);

	template<class TokenList> 
	bool IgnoreUntilBase();

	void AllowOnlyBase(const vector<Token>& Tokens);

	template<class T, bool bRemoveNode> 
	stacked<T> ExpectBase(const vector<Token>& Tokens, bool bReversed = false, opNode* before = NULL);

	template<class T> stacked<T> 
	PushUntilEndBase();

	opNode* PushUntilBase(opNode* addto, const vector<Token> &t, bool bErrorOnMissing);
};

//
// Safe Casts
//

template<class T>
inline T* node_cast(opNode* node)
{
	if(node && node->GetId() == tokenmapping::NodeTypeToken<T>::id)
		return (T*)node;
	return NULL;
}

template<>
inline TerminalNode* node_cast<TerminalNode>(opNode* node)
{
	if(node && TokenFunctions::IsTerminal(node->GetId()))
		return (TerminalNode*)node;
	return NULL;
}

//
// Faster Casts (don't check for NULL)
//

template<class T>
inline T* fast_node_cast(opNode* node)
{
	if(node->GetId() == tokenmapping::NodeTypeToken<T>::id)
		return (T*)node;
	return NULL;
}

template<>
inline TerminalNode* fast_node_cast<TerminalNode>(opNode* node)
{
	if(TokenFunctions::IsTerminal(node->GetId()))
		return (TerminalNode*)node;
	return NULL;
}

} // end namespace nodes

