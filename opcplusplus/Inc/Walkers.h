///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Walkers.h
/// Date: 10/03/2007
///
/// Description:
///
/// Walkers - note, these should eventually disappear.  Instead
//            implement them as visitors.
///****************************************************************

namespace walkers
{

//==========================================
// opMacroExpander
//==========================================

class opMacroExpander
{
	enum expandmode
	{
		expanded,
		skipped,
		next,
	};
	
public:
	
	opMacroExpander(opSymbolTracker& intracker, opNode* node) : tracker(intracker), errors(false)
	{
		//start expanding node's children...
		ExpandChildren(node);
	}
	
	void ExpandChildren(opNode* node)
	{
		opNode::iterator it = node->GetBegin();
		opNode::iterator end = node->GetEnd();
		
		while(it != end)
		{
			expandmode mode = DoExpansion(node,it);
			if(mode == next)
				ExpandChildren(*it);
			if(mode != expanded)
				++it;
		}
	}
	
	expandmode DoExpansion(opNode* parent, opNode::iterator& current)
	{
		opNode* node = *current;
		
		if(DialectStatementBase* statement = node->ToDialectStatementBase())
			node = statement->GetStatement();

		//if we want to expand it, do so, and return clean
		if( ExpandCallNode* expandnode = node_cast<ExpandCallNode>(node) )
		{
			opNode::iterator nextit = current;
			++nextit;	
			
			errors = expandnode->Expand(tracker,nextit,parent)?errors:true;
			
			parent->DeleteNode(current);
			
			current = nextit;
			return expanded;
		}
		//if we want to skip it, return skip
		else if( OPMacroNode* macronode = node_cast<OPMacroNode>(node) )
		{
			tracker.Register(macronode);
			
			return skipped;
		}
		
		//if we have no expansion, return next
		return next;
	}

	bool Errored()
	{
		return errors;
	}

private:

	opSymbolTracker& tracker;
	bool             errors;
};

//==========================================
// MacroConcatenationWalker
//==========================================

class MacroConcatenationWalker
{
public:

	//needs a nonrecursive opmacro pass
	void MacroConcatenations(opNode* node)
	{
		WalkChildren<T_OPMACRO>(node);
	}
	
	//needs a recursive expand pass
	void ExpandConcatenations(opNode* node)
	{
		WalkChildren<T_EXPAND>(node);
	}

private:
	template< Token token >
	void WalkChildren(opNode* node)
	{
		opNode::iterator i   = node->GetBegin();
		opNode::iterator end = node->GetEnd();
		
		while(i != end)
		{
			//what we want to do:
			//if we hit an opmacro or an expand token,
			//we want to look for a concatenation...
			if(TerminalNode* terminal = node_cast<TerminalNode>(*i))
			{
				if(terminal->GetId() == token)
				{
					++i;
					while( i != end && (i->IsComment() || i->IsWhitespace()) )
					{
						++i;
						
						if(i == end)
							break;

						if(ConcatenationOperatorNode* op = node_cast<ConcatenationOperatorNode>(*i))
						{
							//we hit one, expand it
							DoChildren(op);
							DoConcatenation(op,i,node);
						}
					}
				}
			}
			
			if(token == T_EXPAND && i != end)
			{
				//don't want to look into opmacros recursively
				if(!node_cast<OPMacroNode>(*i))
					WalkChildren<token>(*i);
			}
			
			if(i != end)
				++i;
		}
	}
	
	void DoChildren(opNode* node)
	{
		opNode::iterator i   = node->GetBegin();
		opNode::iterator end = node->GetEnd();

		while(i != end)
		{
			ConcatenationOperatorNode* child = node_cast<ConcatenationOperatorNode>(*i);

			DoChildren(*i);

			if (child)
				DoConcatenation(child, i, node);
			else
			{
				++i;		
			}
		}
	}

	// Perform concatenation.
	void DoConcatenation(ConcatenationOperatorNode* op, opNode::iterator& current, opNode* parent)
	{
		ConcatenationArgumentOperatorNode* leftop  = op->GetLeft();
		ConcatenationArgumentOperatorNode* rightop = op->GetRight();
		opNode::iterator                   i       = leftop->GetLast();
		opNode::iterator                   j       = rightop->GetBegin();
		
		if (i != leftop->GetEnd()
		&&  j != rightop->GetEnd())
		{
			opString name;
			opNode*  leftnode  = *i;
			opNode*  rightnode = *j;
			
			leftnode->PrintString(name);
			rightnode->PrintString(name);
			
			stacked<TerminalNode> newNode = NEWNODE(TerminalNode(name, T_ID, op->GetLine(), op->GetFile()));
			
			leftop->DeleteNode(i);
			rightop->DeleteNode(j);	
			
			stacked<ConcatenationArgumentOperatorNode> popleft = op->PopNode(leftop);
			parent->CollapseNode(popleft, current);

			parent->InsertNode(newNode, current);

			stacked<ConcatenationArgumentOperatorNode> popright = op->PopNode(rightop);
			parent->CollapseNode(popright, current);
		}
		else
		{
			stacked<ConcatenationArgumentOperatorNode> popleft = op->PopNode(leftop);
			parent->CollapseNode(popleft, current);

			stacked<ConcatenationArgumentOperatorNode> popright = op->PopNode(rightop);
			parent->CollapseNode(popright, current);
		}
		
		opNode::iterator nextit = current;
		++nextit;
		parent->DeleteNode(current);
		current = nextit;
	}
};

//==========================================
// ConcatenationWalker
//==========================================

class ConcatenationWalker
{
public:
	ConcatenationWalker(opNode* node)
	{
		WalkChildren(node);
	}

private:
	void WalkChildren(opNode* node)
	{
		opNode::iterator i   = node->GetBegin();
		opNode::iterator end = node->GetEnd();

		while(i != end)
		{
			if(ConcatenationOperatorNode* op = node_cast<ConcatenationOperatorNode>(*i))
			{
				DoChildren(op);
				DoConcatenation(op,i,node);
				continue;
			}

			if(i != end)
				++i;
		}
	}

	void DoChildren(opNode* node)
	{
		opNode::iterator i   = node->GetBegin();
		opNode::iterator end = node->GetEnd();

		while(i != end)
		{
			ConcatenationOperatorNode* child = node_cast<ConcatenationOperatorNode>(*i);

			DoChildren(*i);

			if (child)
				DoConcatenation(child, i, node);
			else
			{
				++i;		
			}
		}
	}

	// Perform concatenation.
	void DoConcatenation(ConcatenationOperatorNode* op, opNode::iterator& current, opNode* parent)
	{
		ConcatenationArgumentOperatorNode* leftop  = op->GetLeft();
		ConcatenationArgumentOperatorNode* rightop = op->GetRight();
		opNode::iterator                   i       = leftop->GetLast();
		opNode::iterator                   j       = rightop->GetBegin();

		if (i != leftop->GetEnd()
		&&  j != rightop->GetEnd())
		{
			opString name;
			opNode*  leftnode  = *i;
			opNode*  rightnode = *j;

			leftnode->PrintString(name);
			rightnode->PrintString(name);

			stacked<TerminalNode> newNode = NEWNODE(TerminalNode(name, T_ID, op->GetLine(), op->GetFile()));

			leftop->DeleteNode(i);
			rightop->DeleteNode(j);	

			stacked<ConcatenationArgumentOperatorNode> popleft = op->PopNode(leftop);
			parent->CollapseNode(popleft, current);

			parent->InsertNode(newNode, current);

			stacked<ConcatenationArgumentOperatorNode> popright = op->PopNode(rightop);
			parent->CollapseNode(popright, current);
		}
		else
		{
			stacked<ConcatenationArgumentOperatorNode> popleft = op->PopNode(leftop);
			parent->CollapseNode(popleft, current);

			stacked<ConcatenationArgumentOperatorNode> popright = op->PopNode(rightop);
			parent->CollapseNode(popright, current);
		}

		opNode::iterator nextit = current;
		++nextit;
		parent->DeleteNode(current);
		current = nextit;
	}
};

///==========================================
/// SingleQuotesWalker
///==========================================

class SingleQuotesWalker
{
public:
	SingleQuotesWalker(opNode* node)
	{
		WalkChildren(node);
	}

private:
	void WalkChildren(opNode* node)
	{
		opNode::iterator i   = node->GetBegin();
		opNode::iterator end = node->GetEnd();

		while(i != end)
		{
			SingleQuoteOperatorNode* child = node_cast<SingleQuoteOperatorNode>(*i);

			if (child)
				DoSingleQuotes(child, i, node);
			else
			{
				WalkChildren(*i);

				++i;		
			}
		}
	}

	// Perform concatenation.
	void DoSingleQuotes(SingleQuoteOperatorNode* op, opNode::iterator& current, opNode* parent)
	{
		opString name = "'";

		op->PrintString(name);

		name += "'";

		stacked<TerminalNode> newNode = NEWNODE(TerminalNode(name, T_CHAR, op->GetLine(), op->GetFile()));

		parent->InsertNode(newNode, current);

		opNode::iterator nextit = current;
		++nextit;
		parent->DeleteNode(current);
		current = nextit;
	}
};

///==========================================
/// DoubleQuotesWalker
///==========================================

class DoubleQuotesWalker
{
public:
	DoubleQuotesWalker(opNode* node)
	{
		WalkChildren(node);
	}

private:
	void WalkChildren(opNode* node)
	{
		opNode::iterator i   = node->GetBegin();
		opNode::iterator end = node->GetEnd();

		while(i != end)
		{
			DoubleQuoteOperatorNode* child = node_cast<DoubleQuoteOperatorNode>(*i);

			if (child)
				DoDoubleQuotes(child, i, node);
			else
			{
				WalkChildren(*i);

				++i;		
			}
		}
	}

	// Perform concatenation.
	void DoDoubleQuotes(DoubleQuoteOperatorNode* op, opNode::iterator& current, opNode* parent)
	{
		opString name = '\"';

		op->PrintString(name);

		name += '\"';

		stacked<TerminalNode> newNode = NEWNODE(TerminalNode(name, T_CHAR, op->GetLine(), op->GetFile()));

		parent->InsertNode(newNode, current);

		opNode::iterator nextit = current;
		++nextit;
		parent->DeleteNode(current);
		current = nextit;
	}
};

//==========================================
// ExpandFinder
//==========================================

class ExpandFinder
{
public:
	
	ExpandFinder(opNode* node)
	{
		FindExpandChildren(node);
	}
	
private:
	void FindExpandChildren(opNode* node)
	{
		ExpandCalls<opNode>* wrapped = static_cast<ExpandCalls<opNode>*>(node);
		
		wrapped->FindExpandCalls();
		
		opNode::iterator end = node->GetEnd();
		opNode::iterator it = node->GetBegin();
		
		while(it != end)
		{
			FindExpandChildren(*it);
			++it;
		}
	}
};

//==========================================
// MacroOperatorFinder
//==========================================

class MacroOperatorFinder
{
public:

	MacroOperatorFinder(opNode* node)
	{
		FindOperatorChildren(node);
	}

private:
	void FindOperatorChildren(opNode* node)
	{
		typedef MacroConcatenations< MacroSingleQuotes< MacroDoubleQuotes< opNode > > > type;
		
		type* wrapped = static_cast<type*>(node);

		wrapped->FindConcatenations();
		wrapped->FindSingleQuotes();
		wrapped->FindDoubleQuotes();

		opNode::iterator it = node->GetBegin();
		opNode::iterator end = node->GetEnd();

		while(it != end)
		{
			FindOperatorChildren(*it);
			++it;
		}
	}
};

//==========================================
// ConditionalFinder
//==========================================

class ConditionalFinder
{
public:
	ConditionalFinder(opNode* node)
	{
		FindConditionalChildren(node,NULL);
	}

private:

	void FindConditionalChildren(opNode* node, PreprocessorStatementNode* parentcondition)
	{
		AlterContext newcontext(node);
		
		opArray<PreprocessorStatementNode*> conditionals;
		PreprocessorStatementNode* current = parentcondition;
		
		opNode::iterator it = node->GetBegin();
		opNode::iterator end = node->GetEnd();

		while(it != end)
		{
			if(FindCondition(*it,conditionals,current,parentcondition))
			{
				FindConditionalChildren(*it,current);
			}
			++it;
		}

		//should throw an error if theres anything left in the stack
		//which means you had too many conditions with too few #endif's
		
		if(conditionals.Size())
		{
			//TODO: alter context?
			vector<Token> tokens;
			tokens.push_back(T_POUND_ENDIF);
			opError::MissingError(tokens,*conditionals.Last());
		}

	}

	bool FindCondition(opNode* node, 
					   opArray<PreprocessorStatementNode*>& conditionals, 
					   PreprocessorStatementNode*& current,
					   PreprocessorStatementNode* parentcondition)
	{
		if(PreprocessorStatementNode* condition = node->ToPreprocessorStatementNode())
		{
			//we have conditional...
			PreprocessorNode* directive = condition->GetPreprocessor();

			if(PoundIfNode* node = node_cast<PoundIfNode>(directive))
			{
				//if its a #if, push the stack (?)
				//and point the if back to the previous current
				conditionals.push_back(condition);
				
				//point it back to the last one...
				if(current)
					condition->SetPrevDirective(current);
				
				current = condition;
			}
			else if(PoundIfndefNode* node = node_cast<PoundIfndefNode>(directive))
			{
				conditionals.push_back(condition);

				//point it back to the last one...
				if(current)
					condition->SetPrevDirective(current);

				current = condition;				
			}
			else if(PoundIfdefNode* node = node_cast<PoundIfdefNode>(directive))
			{
				conditionals.push_back(condition);

				//point it back to the last one...
				if(current)
					condition->SetPrevDirective(current);

				current = condition;
			}
			else if(PoundElseNode* node = node_cast<PoundElseNode>(directive))
			{
				if(conditionals.Size())
				{
					//point it back to the last one...
					if(current)
						condition->SetPrevDirective(current);

					*conditionals.Last() = condition;
					current = condition;
				}
				else
				{
					//error...
					vector<Token> tokens;
					tokens.push_back(T_POUND_IF);
					tokens.push_back(T_POUND_IFNDEF);
					tokens.push_back(T_POUND_IFDEF);
					opError::MissingError(tokens,node);
				}
				
				//if its a #else, point it back to the current conditional, make it the current conditional
				//should alter the current node in the stack
			}
			else if(PoundElifNode* node = node_cast<PoundElifNode>(directive))
			{
				if(conditionals.Size())
				{
					//point it back to the last one...
					if(current)
						condition->SetPrevDirective(current);

					*conditionals.Last() = condition;
					current = condition;
				}
				else
				{
					//error...
					vector<Token> tokens;
					tokens.push_back(T_POUND_IF);
					tokens.push_back(T_POUND_IFNDEF);
					tokens.push_back(T_POUND_IFDEF);
					opError::MissingError(tokens,node);
				}

				//if its a #else, point it back to the current conditional, make it the current conditional
				//should alter the current node in the stack
			}
			else if(PoundEndifNode* node = node_cast<PoundEndifNode>(directive))
			{
				//pop the stack
				if(conditionals.Size())
				{
					conditionals.pop_back();
					current = conditionals.Size()?*conditionals.Last():parentcondition;
				}
				else
				{
					//should throw an error if theres nothing to pop.
					//TODO: alter context
					vector<Token> tokens;
					tokens.push_back(T_POUND_IF);
					tokens.push_back(T_POUND_IFNDEF);
					tokens.push_back(T_POUND_IFDEF);
					opError::MissingError(tokens,node);
				}
			}
		}
		else
		{
			//nothing to apply, ignore
			if(!current)
				return true;
			
			//not a condition, what are we looking at?
			//if its an opobjectnode, openumnode, tell it
			//if its a statement node, tell it
			if(OPObjectNode* object = node_cast<OPObjectNode>(node))
			{
				object->SetCondition(current);
			}
			else if(OPEnumNode* object = node_cast<OPEnumNode>(node))
			{
				object->SetCondition(current);
			}
			else if(StatementBase* statement = node->ToStatementBase())
			{
				statement->SetCondition(current);
			}
		}

		//return true if we want to look at contents
		//this visitor should not alter iterators really...
		return true;
	}
};

//==========================================
// opIncludeWalker
//==========================================

class opIncludeWalker
{
public:
	opIncludeWalker(FileNode* file)
	{
		//is it a file? it should add it
		IncludeFile(file);
		
		IncludeChildren(file,file);
	}

	void IncludeChildren(opNode* parent, FileNode* file)
	{
		opNode::iterator it  = parent->GetBegin();
		opNode::iterator end = parent->GetEnd();

		while(it != end)
		{
			IncludeNode(*it,file);

			++it;
		}
	}

	void IncludeNode(opNode* node, FileNode* file)
	{
		if(DialectStatementBase* statement = node->ToDialectStatementBase())
			node = statement->GetStatement();

		if(OPIncludeNode* include = node_cast<OPIncludeNode>(node))
		{
			//one, we need to look at the file name, is it in the file yet?
			opString includestring = include->GetFullFileString();

			path includepath = includestring.GetString();

			// Error if the file does not exist.
			if (!exists(includepath))
				opError::MessageError(include, "opinclude file '" + includestring + "' not found.");
			else if (node_cast<OPFileNode>(include->GetFile()) && !opDriver::FileExists(includepath))
				opError::MessageError(include, "opinclude file '" + includestring + "' not found in the project path.");

			CheckForCycles(include,includestring);
			
			//ok to proceed, process the include
			//this should call some basic processing ONLY (preparse)
			FileNode* newfile = include->IncludeFile();
			
			//now we need to run the inclusion process on it...
			//the rest of the processing should run on it
			IncludeChildren(newfile,newfile);

			//done processing, remove it from the set
			IncludeFiles.Erase(includestring);
		}
		// look inside code nodes
		//else if (CodeStatementNode* codestatementnode = node_cast<CodeStatementNode>(node))
		//{
		else if(CodeNode* codenode = node_cast<CodeNode>(node) )
		{
			if (CodeBodyNode* body = codenode->GetBody())
				IncludeChildren(body,file);
		}
		//}
	}

	void IncludeFile(FileNode* file)
	{
		//NOTE: input name better be simplified
		IncludeFiles.insert(IncludeMap::value_type(file->GetInputName(),file));
	}

	void CheckForCycles(OPIncludeNode* include, const opString& includestring)
	{
		//TODO: throw a message error if we catch a cycle.
		
		IncludeMap::iterator end = IncludeFiles.end();
		IncludeMap::iterator found = IncludeFiles.Find(includestring);

		if(found != end)
		{
			//NOTE: already exists, we need to message error
			//oops, already included by x node...

			opString includedby = "???";
			opString errorstring;

			if( OPIncludeNode* oldinclude = node_cast<OPIncludeNode>( (*found).second ) )
			{
				FileNode* file = oldinclude->GetFile();
				includedby = file->GetInputName();
				errorstring = "infinite cycle detected, file '" + includestring + "' already included by '" + includedby + "'"; 
			}
			else if( FileNode* filenode = (*found).second->ToFileNode() )
			{
				FileNode* file = filenode;
				includedby = file->GetInputName();
				errorstring = "infinite cycle detected, file '" + includestring + "' included while being compiled"; 
			}
			
			opError::MessageError(include,errorstring);
		}
		
		//no error, add it to the map
		IncludeFiles.insert(IncludeMap::value_type(includestring,include));
	}

private:
	typedef opMap<opString,opNode*>  IncludeMap;
	IncludeMap						 IncludeFiles;
};

//==========================================
// NameCheckingWalker
//
//==========================================

class NameCheckingWalker
{
public:

	// Constructor.
	NameCheckingWalker(OPObjectNode* node)
		: Category(node),
		  CategoryName(node->GetName()->GetValue())
	{
		DoNameCheck(Category->GetBody());
	}

private:

	// Perform the name checking.
	void DoNameCheck(OPObjectBodyNode* node)
	{
		opNode::iterator i        = node->GetBegin();
		opNode::iterator end      = node->GetEnd();
		bool             bErrored = false;

		/*=== Check members against category. ===*/

		while (i != end)
		{
			bErrored = CheckMemberAgainstCategory(*i) ? true : bErrored;

			++i;
		}

		// If we had an error, throw an exception.
 		if (bErrored)
			opException::ThrowException();

		/*=== Check members against each other. ===*/

		i = node->GetBegin();

		while (i != end)
		{
			bErrored = FindName(*i) ? true : bErrored;

			++i;
		}

		// If we had an error, throw an exception.
		if (bErrored)
			opException::ThrowException();
	}

	// Make sure none of the members have the same name
	// as the category.
	bool CheckMemberAgainstCategory(opNode* node)
	{
		opString name;

		// Is it a data statement?
		if (DataStatementBase* datastatement = node->ToDataStatementBase())
			name = datastatement->GetMemberName()->GetStringValue();
		// Is it an enumeration?
		else if (OPEnumStatementNode* enumnode = node_cast<OPEnumStatementNode>(node))
			name = enumnode->GetEnum()->GetName()->GetValue();
		// Is it an opobject?
		else if (OPObjectStatementNode* opobjectnode = node_cast<OPObjectStatementNode>(node))
			name = opobjectnode->GetObject()->GetName()->GetValue();
		else if (TypedefStatementNode* typedefnode = node_cast<TypedefStatementNode>(node))
			name = typedefnode->GetTypedef()->GetName()->GetValue();

		if (name != ""
		&&  MemberMatchesCategory(name, node))
		{
			return true;
		}

		return false;
	}

	// Try to find a name or signature.
	// Returns true on a name redefinition.
	bool FindName(opNode* node)
	{
		opString name;
		bool     bErrored = false;

		/*=== name ===*/

		// Is it a data statement?
		if (DataStatementBase* datastatement = node->ToDataStatementBase())
		{
			name = datastatement->GetMemberName()->GetStringValue();

			if (NameExists(name, node))
				bErrored = true;
		}
		// Is it an enumeration?
		else if (OPEnumStatementNode* enumnode = node_cast<OPEnumStatementNode>(node))
		{
			name = enumnode->GetEnum()->GetName()->GetValue();

			if (NameExists(name, node))
				bErrored = true;
		}
		// Is it an opobject?
		else if (OPObjectStatementNode* opobjectnode = node_cast<OPObjectStatementNode>(node))
		{
			name = opobjectnode->GetObject()->GetName()->GetValue();

			if (NameExists(name, node))
				bErrored = true;
		}
		else if (ConstructorStatementBase* ctor = node->ToConstructorStatementBase())
		{
			/*=== name ===*/

			name = ctor->GetConstructor()->GetName()->GetStringValue();

			opNode*             modifier = ctor->FetchModifier("function_arguments_signature");
			ValuedModifierNode* mod      = node_cast<ValuedModifierNode>(modifier);

			mod->GetArguments()->PrintString(name);

			if (SignatureExists(name, node))
				bErrored = true;
		}
		else if (DestructorStatementBase* dtor = node->ToDestructorStatementBase())
		{
			name  = "~";
			name += dtor->GetDestructor()->GetName()->GetStringValue();

			opNode*             modifier = dtor->FetchModifier("function_arguments_signature");
			ValuedModifierNode* mod      = node_cast<ValuedModifierNode>(modifier);

			mod->GetArguments()->PrintString(name);

			if (SignatureExists(name, node))
				bErrored = true;
		}
		// is it a function?
		else if (FunctionStatementBase* function = node->ToFunctionStatementBase())
		{
			name = function->GetMemberName()->GetStringValue();

			if (NameExistsAndIsNotAFunction(name, node))
				bErrored = true;
			else
			{
				opString signature = name;

				opNode*             modifier = function->FetchModifier("function_arguments_signature");
				ValuedModifierNode* mod      = node_cast<ValuedModifierNode>(modifier);

				mod->GetArguments()->PrintString(signature);

				if (function->IsConstFunction())
					signature += " const";
				
				if (SignatureExists(signature, node))
					bErrored = true;
			}
		}

		return bErrored;
	}

	// Returns true and throws an error if the member has the same
	// name as the category.
	bool MemberMatchesCategory(const opString& name, opNode* def)
	{
		if (name == CategoryName)
		{
			opError::MemberNameSameAsCategoryNameError(name, Category, def);

			return true;
		}

		return false;
	}

	// Returns true if the name already exists, otherwise adds it.
	bool NameExists(const opString& name, opNode* def)
	{
		if (NameMap.Contains(name))
		{
			opError::RedefinitionError(name, NameMap[name], def);

			return true;
		}

		NameMap[name] = def;

		return false;
	}

	// Returns true if the name already exists and is not a function, otherwise adds it.
	bool NameExistsAndIsNotAFunction(const opString& name, opNode* def)
	{
		if (NameMap.Contains(name))
		{
			opNode*                oldnode = NameMap[name];
			FunctionStatementBase* old     = oldnode->ToFunctionStatementBase();

			if (!old)
			{
				opError::RedefinitionError(name, oldnode, def);

				return true;
			}
		}

		NameMap[name] = def;

		return false;
	}

	// Returns true if the signature already exists, otherwise adds it.
	bool SignatureExists(const opString& signature, opNode* def)
	{
		if (SignatureMap.Contains(signature))
		{
			opError::RedefinitionError(signature, SignatureMap[signature], def);

			return true;
		}

		SignatureMap[signature] = def;

		return false;
	}

private:
	OPObjectNode*            Category;		// Pointer to the category node.
	opString                 CategoryName;	// Category name.
	opMap<opString, opNode*> NameMap;		// Map of all data/function names.
	opMap<opString, opNode*> SignatureMap;	// Map of all function signatures.
};

//==========================================
// ExtensionPointWalker
//==========================================

class ExtensionPointWalker
{
public:

	ExtensionPointWalker(opNode* node)
	{
		ParseChildren(node);
	}

private:

	void ParseChildren(opNode* node)
	{
		typedef ExtensionPoints< opNode > type;

		type* wrapped = static_cast<type*>( node );

		wrapped->FindExtensionPoints();

		opNode::iterator  it = node->GetBegin();
		opNode::iterator end = node->GetEnd();

		while (it != end)
		{
			if ( !node_cast< ExtendPointNode >(*it) )
				ParseChildren( *it );

			++it;
		}
	}
};

//==========================================
// NameResolverWalker
//
// This visitor resolves names inside 
// opobjects.
//==========================================

class NameResolverWalker
{
public:

	/*=== construction ===*/

	NameResolverWalker(opNode* root)
	{
		opNode::iterator i   = root->GetBegin();
		opNode::iterator end = root->GetEnd();

		while (i != end)
		{
			// Only resolve typedefs within opobject nodes.
			if ( OPObjectNode* node = node_cast<OPObjectNode>( *i ) )
			{
				visit( node );
				unvisit( node );

				ScopeMap.Clear();
			}

			++i;
		}
	}

private:

	/*=== utility ===*/

	// Registers a name/node pair in the map.
	void register_name(const opString& name, opNode* node)
	{
		opList<opNode*>& scopelist = ScopeMap[name];

		scopelist.push_back( node );
	}

	// Unregisters a name in the map.
	void unregister_name(const opString& name)
	{
		opList<opNode*>& scopelist = ScopeMap[name];

		scopelist.pop_back();
	}

	// Returns true if 'name' is registered.
	bool is_registered(const opString& name)
	{
		opList<opNode*>& scopelist = ScopeMap[name];
		
		return scopelist.size() > 0;
	}

	// This method merges scope nodes.
	void merge_scope_nodes(const opString& name, opNode* first, ScopeNode* scope)
	{
		opNode* current = *ScopeMap[ name ].Last();

		// If this node is a using node, we need to resolve
		// it differently.  We need to get the scope from the
		// using node instead of deriving it from the parents.
		if ( UsingNode* u = node_cast<UsingNode>( current ) )
		{
			ScopeNode* usingscope = u->GetScope();
			int        numscopes  = usingscope->GetScopeNum();

			numscopes -= 2;

			for (int i = numscopes; i > -1; i--)
			{
				stacked<opNode> cloned = usingscope->GetScopeNode( i )->CloneGeneric();

				cloned->CopyBasicsRecursive( first );

				scope->PrependScope( *cloned );
				scope->PrependNode( cloned );
			}
		}
		// Otherwise, merge scopes as normal.
		else
		{
			current = current->GetParent();

			while (true)
			{
				// If this is an opobject node, add it's name to the scope.
				if ( OPObjectNode* object = node_cast<OPObjectNode>( current ) )
				{
					TerminalNode* id = scope->PrependTerminalNode( object->GetName()->GetValue(), T_ID );

					id->CopyBasics(first);

					scope->PrependScope(id);
				}
				// If this is a filenode, we're done.
				else if ( OPFileNode* file = node_cast<OPFileNode>( current ) )
					break;

				current = current->GetParent();
			}
		}
	}

	// This method builds a scope node given an opNode representing the last
	// token in the new scope node.
	stacked<ScopeNode> get_scope_node(const opString& name, stacked<opNode>& first)
	{
		stacked<ScopeNode> scope = NEWNODE(ScopeNode());

		scope->CopyBasics( *first );

		merge_scope_nodes( name, *first, *scope );

		scope->AddScope( *first );
		scope->AppendNode( first );

		return scope;
	}

	//==========================================
	// Visiting methods.
	//==========================================

	/*=== generic (routing functions) ===*/

	void visit(opNode* node)
	{
		// opobjects
		if ( OPObjectStatementNode* n = node_cast<OPObjectStatementNode>(node) )
			visit( n->GetObject() );
		// openums
		else if ( OPEnumStatementNode* n = node_cast<OPEnumStatementNode>(node) )
			visit( n->GetEnum() );
		// statements
		else if ( StatementNode* n = node_cast<StatementNode>(node) )
		{
			StatementBase* statement = n->GetInnerStatement();

			// data statements
			if ( DataStatementNode* s = node_cast<DataStatementNode>( statement ) )
			{
				opNode* type = s->GetDataType();

				// id
				if ( TerminalNode* t = node_cast<TerminalNode>( type ) )
				{
					if ( ScopeNode* scope = visit( t ) )
						s->SetType( scope );
				}
				// scope
				else if ( ScopeNode* t = node_cast<ScopeNode>( type ) )
					visit( t );
				// pointers
				else if ( PointerNode* t = node_cast<PointerNode>( type ) )
					visit( t );
				// references
				else if ( ReferenceNode* t = node_cast<ReferenceNode>( type ) )
					visit( t );
				// template types
				else if ( TemplateTypeNode* t = node_cast<TemplateTypeNode>( type ) )
				{
					if ( ScopeNode* scope = visit( t ) )
						s->SetType( scope );
				}
			}
			// function prototype statement
			else if ( FuncPrototypeStatementNode* s = node_cast<FuncPrototypeStatementNode>( statement ) )
				visit( s->GetFunctionPrototype() );
			// using statement
			else if ( UsingStatementNode* s = node_cast<UsingStatementNode>( statement ) )
				visit( s->GetUsing() );
			// function pointers
			else if ( FuncPointerStatementNode* t = node_cast<FuncPointerStatementNode>( statement ) )
				visit( t->GetFunctionPointer() );
		}
		// typedef statement
		else if ( TypedefStatementNode* s = node_cast<TypedefStatementNode>( node ) )
			visit( s->GetTypedef() );
		// function definition statements
		else if ( FunctionDefinitionStatementNode* n = node_cast<FunctionDefinitionStatementNode>(node) )
			visit( n->GetFunctionDefinition() );
		// cpp construct statement
		else if ( CPPConstructStatementNode* n = node_cast<CPPConstructStatementNode>(node) )
			visit( n->GetConstruct() );
	}

	void unvisit(opNode* node)
	{
		// opobjects
		if ( OPObjectStatementNode* n = node_cast<OPObjectStatementNode>(node) )
			unvisit( n->GetObject() );
		// openums
		else if ( OPEnumStatementNode* n = node_cast<OPEnumStatementNode>(node) )
			unvisit( n->GetEnum() );
		// statements
		else if ( StatementNode* n = node_cast<StatementNode>(node) )
		{
			StatementBase* statement = n->GetInnerStatement();

			// using statement
			if ( UsingStatementNode* s = node_cast<UsingStatementNode>( statement ) )
				unvisit( s->GetUsing() );
		}
		// typedef statement
		else if ( TypedefStatementNode* s = node_cast<TypedefStatementNode>( node ) )
			unvisit( s->GetTypedef() );
		// cpp construct statement
		else if ( CPPConstructStatementNode* n = node_cast<CPPConstructStatementNode>(node) )
			unvisit( n->GetConstruct() );
	}

	/*=== opobjects ===*/

	void visit(OPObjectNode* node)
	{
		opString name = node->GetName()->GetValue();

		/*=== Register your name. ===*/

		register_name( name, node );

		/*=== Visit your children. ===*/

 		opNode::iterator i   = node->GetBody()->GetBegin();
 		opNode::iterator end = node->GetBody()->GetEnd();
		opNode::iterator next;
 
 		while (i != end)
 		{
			next = i;
			++next;

 			visit( *i );
 
 			i = next;
 		}

		/*=== Unvisit your children. ===*/

		i   = node->GetBody()->GetBegin();
		end = node->GetBody()->GetEnd();

		while (i != end)
		{
			next = i;
			++next;

			unvisit( *i );

			i = next;
		}
	}

	void unvisit(OPObjectNode* node)
	{
		unregister_name( node->GetName()->GetValue() );						
	}

	/*=== typedefs ===*/

	void visit(TypedefNode* node)
	{
		register_name( node->GetName()->GetValue(), node );
	}
	
	void unvisit(TypedefNode* node)
	{
		unregister_name( node->GetName()->GetValue() );
	}

	/*=== openums ===*/

	void visit(OPEnumNode* node)
	{
		register_name( node->GetName()->GetValue(), node );
	}

	void unvisit(OPEnumNode* node)
	{
		unregister_name( node->GetName()->GetValue() );
	}

	/*=== c++ constructs (class/struct/enum/union) ===*/

	void visit(CPPConstructNode* node)
	{
		TerminalNode* name = node->GetName();

		if (name)
			register_name( name->GetValue(), node );
	}

	void unvisit(CPPConstructNode* node)
	{
		TerminalNode* name = node->GetName();

		if (name)
			unregister_name( name->GetValue() );
	}

	/*=== usings ===*/

	void visit(UsingNode* node)
	{
		ScopeNode*    scope = node->GetScope();
		TerminalNode* id    = node_cast<TerminalNode>( scope->GetScopeNode( scope->GetScopeNum() - 1 ) );

		if ( id && id->GetId() == T_ID )
		{
			register_name( id->GetValue(), node );
		}
	}

	void unvisit(UsingNode* node)
	{
		ScopeNode*    scope = node->GetScope();
		TerminalNode* id    = node_cast<TerminalNode>( scope->GetScopeNode( scope->GetScopeNum() - 1 ) );

		if ( id && id->GetId() == T_ID )
		{
			unregister_name( id->GetValue() );
		}
	}

	/*=== ids ===*/

	ScopeNode* visit(TerminalNode* node)
	{
		// Only resolve ids.
		if ( node->GetId() != T_ID )
			return NULL;

		// If this name is registered, we need to resolve it.
		if ( is_registered( node->GetValue() ) )
		{
			opNode*            parent = node->GetParent();
			opNode::iterator   it     = parent->FindNodeIndex( node );

			++it;

			stacked<opNode>    first     = parent->PopNode<opNode>( node );
			stacked<ScopeNode> scopenode = get_scope_node( node->GetValue(), first );
			ScopeNode*         scope     = scopenode.GetNode();

			parent->InsertNode( scopenode, it );

			return scope;
		}

		return NULL;
	}

	/*=== scopes ===*/

	void visit(ScopeNode* node)
	{
		/*=== First, we need to see if we need to further resolve this scope. ===*/

		opNode*  first = node->GetScopeNode( 0 );
		opString name;

		// Case 1: id
		if ( TerminalNode* t = node_cast<TerminalNode>( first ) )
		{
			if ( t->GetId() == T_ID )
				name = t->GetValue();
		}
		// Case 2: template type
		else if ( TemplateTypeNode* t = node_cast<TemplateTypeNode>( first ) )
			name = t->GetName()->GetValue();

		// If this name is registered, resolve the scope.
		if ( is_registered( name ) )
			merge_scope_nodes( name, first, node );

		/*=== Now, we need to loop through every scope item. If  ===*/
		/*=== it's a template type, we need to look inside of    ===*/
		/*=== the <>'s and see if anything needs to be resolved. ===*/

		int numscopes = node->GetScopeNum();

		for (int i = 0; i < numscopes; i++)
		{
			opNode* current = node->GetScopeNode( i );

			if ( TemplateTypeNode* t = node_cast<TemplateTypeNode>( current ) )
				visit( t->GetBody() );
		}
	}

	/*=== pointers ===*/

	void visit(PointerNode* node)
	{
		opNode* inner = node->GetType();

		// Case 1: Pointer to an id.
		if ( TerminalNode* t = node_cast<TerminalNode>(inner) )
		{
			if ( ScopeNode* scope = visit( t ) )
				node->SetType( scope );
		}
		// Case 2: Pointer to a scope.
		else if ( ScopeNode* t = node_cast<ScopeNode>(inner) )
			visit( t );
		// Case 3: Pointer to a template type
		else if ( TemplateTypeNode* t = node_cast<TemplateTypeNode>(inner) )
		{
			if ( ScopeNode* scope = visit( t ) )
				node->SetType( scope );
		}
	}

	/*=== references ===*/

	void visit(ReferenceNode* node)
	{
		opNode* inner = node->GetType();

		// Case 1: Reference to an id.
		if ( TerminalNode* t = node_cast<TerminalNode>(inner) )
		{
			if ( ScopeNode* scope = visit( t ) )
				node->SetType( scope );
		}
		// Case 2: Reference to a scope.
		else if ( ScopeNode* t = node_cast<ScopeNode>(inner) )
			visit( t );
		// Case 3: Reference to a pointer.
		else if ( PointerNode* t = node_cast<PointerNode>(inner) )
			visit( t );
		// Case 4: Reference to a template type.
		else if ( TemplateTypeNode* t = node_cast<TemplateTypeNode>(inner) )
		{
			if ( ScopeNode* scope = visit( t ) )
				node->SetType( scope );
		}
	}

	/*=== template types ===*/

	ScopeNode* visit(TemplateTypeNode* node)
	{
		/*=== First, visit the body of this template. ===*/

		visit( node->GetBody() );

		/*=== Second, see if the template name needs to be resolved. ===*/

		TerminalNode* name = node->GetName();

		// If this name is registered, we need to resolve it.
		if ( is_registered( name->GetValue() ) )
		{
			opNode*            parent = node->GetParent();
			opNode::iterator   it     = parent->FindNodeIndex( node );

			++it;

			stacked<opNode>    first     = parent->PopNode<opNode>( node );
			stacked<ScopeNode> scopenode = get_scope_node( name->GetValue(), first );
			ScopeNode*         scope     = scopenode.GetNode();

			parent->InsertNode( scopenode, it );

			return scope;
		}

		return NULL;
	}

	/*=== template type body ===*/

	void visit(TemplateTypeBodyNode* node)
	{
		opNode::iterator i   = node->GetBegin();
		opNode::iterator end = node->GetEnd();
		opNode::iterator next;

		while (i != end)
		{
			opNode* current = *i;
			    
			next = i;
			++next;

			// Case 1: id
			if ( TerminalNode* t = node_cast<TerminalNode>( current ) )
				visit( t );
			// Case 2: scopes
			else if ( ScopeNode* t = node_cast<ScopeNode>( current ) )
				visit( t );
			// Case 3: template types
			else if ( TemplateTypeNode* t = node_cast<TemplateTypeNode>( current ) )
				visit( t );

			i = next;
		}
	}

	/*=== function definitions ===*/

	void visit(FunctionDefinitionNode* node)
	{
		opNode* returntype = node->GetReturn();

		// Case 1: id
		if ( TerminalNode* t = node_cast<TerminalNode>(returntype) )
		{
			if ( ScopeNode* scope = visit( t ) )
				node->SetReturn( scope );
		}
		// Case 2: scopes
		else if ( ScopeNode* t = node_cast<ScopeNode>(returntype) )
			visit( t );
		// Case 3: pointers
		else if ( PointerNode* t = node_cast<PointerNode>(returntype) )
			visit( t );
		// Case 4: references
		else if ( ReferenceNode* t = node_cast<ReferenceNode>(returntype) )
			visit( t );
		// Case 5: template types
		else if ( TemplateTypeNode* t = node_cast<TemplateTypeNode>(returntype) )
		{
			if ( ScopeNode* scope = visit( t ) )
				node->SetReturn( scope );
		}
	}

	/*=== function prototypes ===*/

	void visit(FunctionPrototypeNode* node)
	{
		opNode* returntype = node->GetReturn();

		// Case 1: id
		if ( TerminalNode* t = node_cast<TerminalNode>(returntype) )
		{
			if ( ScopeNode* scope = visit( t ) )
				node->SetReturn( scope );
		}
		// Case 2: scopes
		else if ( ScopeNode* t = node_cast<ScopeNode>(returntype) )
			visit( t );
		// Case 3: pointers
		else if ( PointerNode* t = node_cast<PointerNode>(returntype) )
			visit( t );
		// Case 4: references
		else if ( ReferenceNode* t = node_cast<ReferenceNode>(returntype) )
			visit( t );
		// Case 5: template types
		else if ( TemplateTypeNode* t = node_cast<TemplateTypeNode>(returntype) )
		{
			if ( ScopeNode* scope = visit( t ) )
				node->SetReturn( scope );
		}
	}

	/*=== function pointers ===*/

	void visit(FunctionPointerNode* node)
	{
		opNode* returntype = node->GetReturn();

		// Case 1: id
		if ( TerminalNode* t = node_cast<TerminalNode>(returntype) )
		{
			if ( ScopeNode* scope = visit( t ) )
				node->SetReturn( scope );
		}
		// Case 2: scopes
		else if ( ScopeNode* t = node_cast<ScopeNode>(returntype) )
			visit( t );
		// Case 3: pointers
		else if ( PointerNode* t = node_cast<PointerNode>(returntype) )
			visit( t );
		// Case 4: references
		else if ( ReferenceNode* t = node_cast<ReferenceNode>(returntype) )
			visit( t );
		// Case 5: template types
		else if ( TemplateTypeNode* t = node_cast<TemplateTypeNode>(returntype) )
		{
			if ( ScopeNode* scope = visit( t ) )
				node->SetReturn( scope );
		}
	}

	/*=== data ===*/

	opMap< opString, opList<opNode*> > ScopeMap;
};

} // end namespace walkers

