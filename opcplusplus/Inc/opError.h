///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: opError.h
/// Date: 11/26/2006
///
/// Description:
///
/// Contains all error class declarations / inline methods.
///****************************************************************

namespace errors
{

///==========================================
/// ErrorInfo
///==========================================

struct ErrorInfo
{
	/*=== construction ===*/

	ErrorInfo()
		: File(NULL),
		  Line(-1),
		  Error("")
	{
	}

	ErrorInfo(FileNode* file, int line, const opString& error)
		: File(file),
		  Line(line),
		  Error(error)
	{
	}

	ErrorInfo(const ErrorInfo& info)
	{
		*this = info;
	}

	/*=== operator overloads ===*/

	ErrorInfo& operator = (const ErrorInfo& info)
	{
		File  = info.File;
		Line  = info.Line;
		Error = info.Error;

		return *this;
	}

	/*=== data ===*/

	FileNode* File;		// Pointer to the file that this error occured in.
	int		  Line;		// Error line number.
	opString  Error;	// The actual error message.
};

struct AlterContext;

///==========================================
/// opError
///==========================================

class opError
{
public:
	/*=== construction ===*/

	opError()  {}
	~opError() {}

	/*=== get ===*/

	static const opString& GetParseError(Token t)
	{
		return ParseErrors[t];
	}

	static bool HasErrors()
	{
		if(!Errors.IsEmpty())
			return true;
		return false;
	}

private:
	/*=== set ===*/

	static void SetCurrentContext(Token context, opNode* node)
	{
		ContextToken = context;
		ContextNode = node;
	}
	
	static void GetCurrentContext(Token& token, opNode*& node)
	{
		token = ContextToken;
		node = ContextNode;
	}
	
	friend AlterContext;

public:
	/*=== utility ===*/

	static void InitParseErrors();
	static void Print();
	static int  NumErrors() { return Errors.Size(); }
	static void Clear();
	static void TruncateErrorName(opString& name);
	
	/*=== exception errors ===*/

	// "<Exception> Exception Caught! : Please report to www.opcpp.com"
	static void ExceptionError(const opString &Exception);

	/*=== scanner errors ===*/

	// Found unbounded C-style comment.
	static void UnboundedCommentError(opNode* root, int line);

	// Found unbounded string.
	static void UnboundedStringError(opNode* root, int line);

	/*=== compiler errors ===*/

	// "Member <string message> already defined.  See previous definition."
	static void RedefinitionError(const opString& name, opNode* olddef, opNode* newdef);

	// "Member <string message> cannot be same name as enclosing category."
	static void MemberNameSameAsCategoryNameError(const opString& name, opNode* category, opNode* def);

	// "Expected <string message> got <Value> in <Context>"
	static void ExpectError(const opString& expected, opNode* got);

	// "Expected <Grammar> got <Value> in <Context>"
	static void ExpectError(Token expected, opNode* got);

	// "Expected <Multiple> got <Value> in <Context>"
	static void ExpectError(const vector<Token>& expected, opNode* got);
	
	// Expected <type(s)>, before <got> in context <context>, instead got <token>.
	static void ReverseExpectError(const vector<Token>& expected, opNode* got, opNode* before);

	// Missing <type(s)>, in context <context>.
	static void MissingError(const vector<Token>& expected, opNode* where);

	// "<Value> not allowed in <Context>, Removal may solve"
	static void CheckNoneError(opNode* unexpected);
	
	// "<Value> not allowed in <Context>"
	static void DisallowError(opNode* disallowed);

	// Reached premature end of <location>, Expected <list of grammars>
	static void PrematureError(Token expected, opNode* beginning, bool bEOF);

	// Reached premature end of <location>, Expected <list of grammars>
	static void PrematureError(const vector<Token>& expected, opNode* beginning, bool bEOF);

	// Reached premature end of <location>.
	static void PrematureError(opNode* beginning, bool bEOF);

	// Mutual exclusion error.
	static void MutualExclusionError(opNode* node1, opNode* node2);
	
	// Invalid <modifier name> value <value>: failed regex "regex"
	// Invalid <modifier name> value <value>: <custom error message>
	// TODO: need to store more information in the tracker to do this.
	static void InvalidValueError(ValuedModifierNode* modifier, ModifierNodeBase* definition);

	// Invalid data modifier <Modifier> used, available modifiers: <Valid Modifier List>
	static void InvalidDataModifierError(opNode* badmodifier);

	// Invalid function modifier <Modifier> used, available modifiers: <Valid Modifier List>
	static void InvalidFunctionModifierError(opNode* badmodifier);

	static void ExpandError(opNode* node, const opString& signature, const opSymbolTracker& tracker);

	/*=== dialect errors ===*/
	
	static void NoteRedeclaredError(DialectNote* oldnote, NoteNode* newnote);
	
	static void NoteDefinitionError(NoteDefinitionNode* oldnode, NoteDefinitionNode* newnode);
	
	static void MapRedeclaredError(DialectMap* oldmap, CategoryMapNode* newmap);
	
	static void NameCollisionError(const opString& name, opNode* oldnode, opNode* newnode);

	static void DisallowError(DisallowNode* disallow, opNode* location);

	// got a bad operand in a criteria expression
	static void OperandError(opNode* node, DialectExpressionMap* map, const opString& operandname);

	// invalid extendpoint error
	static void InvalidExtensionError(opNode* node, const opString& name, const opString& type);

	// alt prefix redefinition error
	static void AltPrefixRedefinitionError(const opString& type, opNode* oldprefix, opNode* newprefix);

	/*=== generic ===*/
	
	static void MessageError(opNode* node, const opString& message, bool bThrowException = true);
	
private:
	/*=== private utility ===*/
	
	static void		PrintMissingErrors();
	static void		Add(FileNode* file, int line, const opString& error);
	static bool		SortFunctor(const ErrorInfo& e1, const ErrorInfo& e2);
	static opString NodeString(opNode* node);
	static opString ContextString();
	static void     GetDataModifierList(const opString& spacer, opString& modifierlist, opNode* innernode);
	static void     GetFunctionModifierList(const opString& spacer, opString& modifierlist, opNode* innernode);
	static void     GetList(const opString& spacer, opString& output, opSet<opString>& list);

	/*=== data ===*/
	
	static opList<ErrorInfo> Errors;
	static opString          ParseErrors[Tokens_MAX + 1];
	static Token             ContextToken;
	static opNode*			 ContextNode;
	static int               MaxErrorNameLength;
};

struct AlterContext
{
	//temporary node altering context
	AlterContext(opNode* node);

	//parse/loop contexts
	AlterContext(Token token, opNode* node)
	{
		opError::GetCurrentContext(oldtoken,oldnode);
		opError::SetCurrentContext(token,node);
		benabled = true;
	}

	~AlterContext()
	{
		if(benabled)
			Disable();
	}
	
	void Disable()
	{
		opError::SetCurrentContext(oldtoken,oldnode);
		benabled = false;
	}

private:
	bool benabled;
	Token oldtoken;
	opNode* oldnode;
};

}//end of errors namespace

namespace memory
{
	template<class T>
	class stackedcontext : public stacked<T>, public AlterContext
	{
	private:
		/**** construction / destruction ****/
// 		explicit stackedcontext<T>(T* pointer)
// 			: stacked<T>(pointer), AlterContext(pointer)
// 		{}

		friend opNode;
		friend opNodeBase;
		//friend NewHandler;

	public:

		//from stacked constructor
		stackedcontext<T>(const stacked<T>& pointer)
			: stacked<T>(pointer), AlterContext(pointer.GetNode())
		{}

		//copy constructor
// 		stackedcontext<T>(const stackedcontext<T>& pointer)
// 			: stacked<T>(pointer.GetNode()), AlterContext(pointer.GetNode())
// 		{}

		~stackedcontext<T>()
		{
		}
	};
}