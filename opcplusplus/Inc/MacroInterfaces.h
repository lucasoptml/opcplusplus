///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: MacroInterfaces.h
/// Date: 10/04/2007
///
/// Description:
///
/// Macro interfaces.
///****************************************************************

///==========================================
/// MacroConcatenations
///==========================================

template<class Parent>
class MacroConcatenations : public Parent
{
public:
	IMPLEMENTS_INTERFACE(MacroConcatenations)

	// Finds ID@ID
	void FindConcatenations()
	{
		LOOP_START(G_CONCATENATION_OPERATOR);
		{
			HIT(T_SPACER)
			{
				stackedcontext<ConcatenationOperatorNode> newNode = opNode::Make<ConcatenationOperatorNode>(T_SPACER);
			
				stacked<TerminalNode> spacer = opNode::Expect<TerminalNode>(T_SPACER);

				stacked<opNode> left = opNode::ReverseExpectOr<opNode>(T_ID,
					                                                   G_CONCATENATION_OPERATOR,
																	   *spacer);

				spacer.Delete();
				
				//manual reverse make (basically)
				stacked<ConcatenationArgumentOperatorNode> leftop = NEWNODE(ConcatenationArgumentOperatorNode);
				leftop->CopyBasics(*left);

				leftop->AppendNode(left);
				newNode->SetLeft(*leftop);
				newNode->AppendNode(leftop);

				stacked<ConcatenationArgumentOperatorNode> rightop = opNode::Make<ConcatenationArgumentOperatorNode>(T_ID);
				stacked<opNode> right = opNode::ExpectOr(T_ID,G_CONCATENATION_OPERATOR);
				rightop->AppendNode(right);
				newNode->SetRight(*rightop);
				newNode->AppendNode(rightop);
				
				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

///==========================================
/// MacroSingleQuotes
///==========================================

template<class Parent>
class MacroSingleQuotes : public Parent
{
public:
	IMPLEMENTS_INTERFACE(MacroSingleQuotes)

	// Finds `text`
	void FindSingleQuotes()
	{
		LOOP_START(G_SINGLE_QUOTE_OPERATOR);
		{
			HIT(T_ACCENT)
			{
				stackedcontext<SingleQuoteOperatorNode> newNode = opNode::Make<SingleQuoteOperatorNode>(T_ACCENT);

				Erase(T_ACCENT);

				opNode::PushUntilAdd(*newNode, T_ACCENT);

				Erase(T_ACCENT);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

///==========================================
/// MacroDoubleQuotes
///==========================================

template<class Parent>
class MacroDoubleQuotes : public Parent
{
public:
	IMPLEMENTS_INTERFACE(MacroDoubleQuotes)

	// Finds ``text``
	void FindDoubleQuotes()
	{
		LOOP_START(G_DOUBLE_QUOTE_OPERATOR);
		{
			HIT(T_DOUBLE_ACCENT)
			{
				stackedcontext<DoubleQuoteOperatorNode> newNode = opNode::Make<DoubleQuoteOperatorNode>(T_DOUBLE_ACCENT);

				Erase(T_DOUBLE_ACCENT);

				opNode::PushUntilAdd(*newNode, T_DOUBLE_ACCENT);

				Erase(T_DOUBLE_ACCENT);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};