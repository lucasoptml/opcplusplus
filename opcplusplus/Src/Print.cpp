///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Print.cpp
/// Date: 11/26/2006
///
/// Description:
///
/// Contains the Print(...) source code for all grammar nodes.
///****************************************************************

#include "opCPP.h"

//
// opNode
//

void opNode::PrintOriginalChildren( opSectionStream& stream )
{
	iterator i    = GetBegin();
	iterator end  = GetEnd();
	iterator last = GetLast();

	while (i != end) 
	{
		i->PrintOriginal(stream);

		if (i != last)
			stream << ' ';

		++i;
	}
}

void opNode::PrintOriginalUnspacedChildren(opSectionStream& stream )
{
	iterator i    = GetBegin();
	iterator end  = GetEnd();

	while (i != end)
	{
		i->PrintOriginal(stream);

		++i;
	}
}

void opNode::PrintTransformedChildren( opSectionStream& stream )
{
	iterator i    = GetBegin();
	iterator end  = GetEnd();
	iterator last = GetLast();

	while (i != end)
	{
		i->PrintTransformed(stream);

		if (i != last)
			stream << ' ';

		++i;
	}
}

void opNode::PrintNodeChildren( opFileStream& stream )
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		i->PrintNode(stream);

		++i;
	}
}

void opNode::PrintNodeChildren( opDialectStream& stream )
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		i->PrintDialectNode(stream);

		++i;
	}
}

void opNode::MacroPrintChildren(opSectionStream& stream, int& charnum)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		i->MacroPrint(stream, charnum);

		++i;
	}
}

void opNode::PrintStringChildren(opString& s)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		i->PrintString(s);
		
		++i;

		if(i != end)
			s += ' ';
	}
}

void opNode::PrintStringUnspacedChildren(opString& s)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		i->PrintString(s);

		++i;
	}
}

void opNode::PrintXmlChildren(opXmlStream& stream)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		i->PrintXml(stream);
		++i;
	}
}


//
// FileNode
//

// generates comments at the top of the specified
// file, including date, etc.
opString FileNode::WriteFileHeader(stringstream& o, opString filename)
{
	//write normal information
	o << "/*" << endl;
	o << "\tFile:          " << filename << endl;
	o << "\topCPP Version: " << opString( opVersion::GetVersion() ) << endl;
	o << "\tBuild Date:    " << __DATE__ << " at " << __TIME__ << endl;

	//demo banner
	o << opDemoSupport::GetDemoBanner();

	o << "*/" << endl << endl;

	//now write include once preprocessor directives
	//figure out the macro string
	filename.ToUpper();
	filename.Replace(' ','_');
	filename.Replace('.','_');
	filename.Replace('\\','_');
	filename.Replace('/','_');
	filename.Replace(':','_');
	filename.Replace('+',"PLUS");
	filename.Replace('-',"DASH");

	//build the macro name then print it;
	opString macroname;
	macroname = "__";
	macroname += filename;
	macroname += "__";

	return macroname;
}

void FillSectionStreams(opSectionStream& section, const opString& type, const opString& basemacro, const opString& bigname, const opString& smallname)
{
	if (!opParameters::Get().Compact)
		section.headingstring = opString("\n\t// == ") + smallname + " section start == //";
	
	section.headingstring += opString("\n\t#ifdef OPCOMPILE_") + type + "_" + bigname;
	
	section.headingstring += "\n\t\t#ifndef ";
	section.headingstring += basemacro + bigname;
	section.headingstring += "\n\t\t#define ";
	section.headingstring += basemacro + bigname + "\n";
	section.footerstring = opString("\n\t\t#endif");
	section.footerstring += opString("\n\t#endif\n");

	if (!opParameters::Get().Compact)
		section.footerstring += opString("\t// == ") + smallname + " section end == //\n";
};

void OPFileNode::PrintNode(opFileStream& stream )
{
	// generate comments at top of driver
	stringstream headerheading;
	stringstream sourceheading;
	stringstream sourcefooter;
	stringstream headerfooter;
	opString hmacro = WriteFileHeader(headerheading,HeaderFile);
	opString smacro = WriteFileHeader(sourceheading,SourceFile);

	//special source header
	sourceheading << "\t#ifdef OPCOMPILE_SOURCE" << endl << endl;

	PrintNodeChildren(stream);

	//write file footers
	//header
	headerfooter << endl << endl;
	//	headerfooter << "#endif //header macro" << endl;

	//source
	sourcefooter << "\n\t#endif //OPCOMPILE_SOURCE" << endl;
	//	sourcefooter << "#endif //source macro" << endl;

	stream.header.headingstring = headerheading.str();
	stream.header.footerstring = headerfooter.str();

	stream.source.headingstring = sourceheading.str();
	stream.source.footerstring = sourcefooter.str();

	//now write all our section heading strings
	//and footer strings

	FillSectionStreams(stream.header.heading,"OOH",hmacro,"HEADING","heading");
	FillSectionStreams(stream.header.body,"OOH",hmacro,"BODY","body");
	FillSectionStreams(stream.header.footer,"OOH",hmacro,"FOOTER","footer");

	FillSectionStreams(stream.source.heading,"OCPP",smacro,"HEADING","heading");
	FillSectionStreams(stream.source.body,"OCPP",smacro,"BODY","body");
	FillSectionStreams(stream.source.footer,"OCPP",smacro,"FOOTER","footer");
}

void OPFileNode::PrintXml(opXmlStream& stream)
{
	//TODO: will need to add some special stuff here... (xslt), ?xml
	stream.Write("<?xml-stylesheet href=\"");

	//TODO: should really use a web url...maybe
	opString filedir = "file://" + path((opPlatform::GetOpCppDirectory() + "/../../../Css/").GetString()).string() + "/";
	stream << filedir + "visualize.xsl";
	stream.Write("\" type=\"text/xsl\"?>");
	stream << endl << endl;
	
	opXmlTag codetag(stream,"Code");
	
	{
		opXmlTextTag styletag(stream,"Stylesheet");
		stream << filedir + "visualize.css";
	}

	{
		opXmlTextTag dtag(stream,"Directory");
		stream << filedir;
	}

	PrintXmlChildren(stream);
}

void DialectFileNode::PrintXml(opXmlStream& stream )
{
	//TODO: will need to add some special stuff here... (xslt), ?xml
	stream.Write("<?xml-stylesheet href=\"");

	//TODO: should really use a web url...maybe
	opString filedir = "file://" + path((opPlatform::GetOpCppDirectory() + "/../../../Css/").GetString()).string() + "/";
	stream << filedir + "dialect.xsl";
	stream.Write("\" type=\"text/xsl\"?>");
	stream << endl << endl;

	opXmlTag codetag(stream,"Dialect");

	{
		opXmlTextTag styletag(stream,"Stylesheet");
		stream << filedir + "visualize.css";
	}

	{
		opXmlTextTag directory(stream,"Directory");
		stream << filedir;
	}

	PrintXmlChildren(stream);
}

void DialectFileNode::PrintDialectNode(opDialectStream& stream)
{
// 	stringstream fileheading;
// 	opString macrostring = WriteFileHeader(fileheading,OutputFileName);
// 	
// 	stream.headingstring = fileheading.str();
// 	stream << "#ifndef " << macrostring << endl;
// 	stream << "#define " << macrostring << endl << endl;
// 	
// 	PrintNodeChildren(stream);
// 	
// 	stream << endl << endl;
// 	stream << "#endif //" << macrostring << endl << endl;

	// generate comments at top of driver
	stringstream headerheading;
	stringstream sourceheading;
	stringstream sourcefooter;
	stringstream headerfooter;
	opString hmacro = WriteFileHeader(headerheading,HeaderFile);
	opString smacro = WriteFileHeader(sourceheading,SourceFile);

	//special source header
	//sourceheading << "\t#ifdef OPCOMPILE_SOURCE" << endl << endl;

	PrintNodeChildren(stream);

	if(opParameters::Get().Highlighting)
		PrintHighlighting(stream.header.heading);

	//write file footers
	//header
	headerfooter << endl << endl;
	//	headerfooter << "#endif //header macro" << endl;

	//source
	//sourcefooter << "\n\t#endif //OPCOMPILE_SOURCE" << endl;
	//	sourcefooter << "#endif //source macro" << endl;

	stream.header.headingstring = headerheading.str();
	stream.header.footerstring = headerfooter.str();

	stream.source.headingstring = sourceheading.str();
	stream.source.footerstring = sourcefooter.str();

	//now write all our section heading strings
	//and footer strings

	FillSectionStreams(stream.header.heading,"OOH",hmacro,"HEADING","heading");
	FillSectionStreams(stream.header.body,"OOH",hmacro,"BODY","body");
	FillSectionStreams(stream.header.footer,"OOH",hmacro,"FOOTER","footer");

	FillSectionStreams(stream.source.heading,"OCPP",smacro,"HEADING","heading");
	FillSectionStreams(stream.source.body,"OCPP",smacro,"BODY","body");
	FillSectionStreams(stream.source.footer,"OCPP",smacro,"FOOTER","footer");
}

void HighlightKeyword(opSectionStream& stream, const opString& keyword)
{
	stream << "#define " << keyword << " class" << endl;
}

void DialectFileNode::PrintHighlighting(opSectionStream& stream)
{
	stream << endl << "// HIGHLIGHTING";
	stream << endl << "#if 0" << endl;

	/*=== opCpp keywords ===*/

	int start = ((int) OPCPP_KEYWORDS_START) + 1;
	int end   = (int) OPCPP_KEYWORDS_END;

	for (int i = start; i < end; i++)
	{
		Token    t    = (Token) i;
		opString name = opTokenMap::GetString(t);

		if (name != "@")
			HighlightKeyword(stream, name);
	}

	/*=== opCpp dialect keywords ===*/

	start = ((int) OPCPP_DIALECT_KEYWORDS_START) + 1;
	end   = (int) OPCPP_DIALECT_KEYWORDS_END;

	for (int i = start; i < end; i++)
	{
		Token    t    = (Token) i;
		opString name = opTokenMap::GetString(t);

		if (name != "c++")
			HighlightKeyword(stream, name);
	}

	/*=== opCpp dialect modifiers ===*/

	start = ((int) OPCPP_DIALECT_MODIFIERS_START) + 1;
	end   = (int) OPCPP_DIALECT_MODIFIERS_END;

	for (int i = start; i < end; i++)
	{
		Token    t    = (Token) i;
		opString name = opTokenMap::GetDialectModifierString(t);

		HighlightKeyword(stream, name);
	}

	/*=== prefixes ===*/

	const opSet<opString>&          prefixes = DialectTracker::GetAllPrefixes();
	opSet<opString>::const_iterator it       = prefixes.begin();
	opSet<opString>::const_iterator pend     = prefixes.end();

	while (it != pend)
	{
		HighlightKeyword(stream, *it);

		++it;
	}

	stream << "#endif" << endl;
}

//
// NamespaceNode
//

//NOTE: can add back the alternate (scoped) option later.
void NamespaceNode::PrintNode( opFileStream& stream )
{
	int numscopes = 0;
	PrintTop(stream,numscopes);
	
	Body->PrintNode(stream);
	
	for(int i = 0; i < numscopes; i++)
	{
		stream.header.DeIndent();
		stream.header << "};" << endl << endl;
// 		stream.source.DeIndent();
// 		stream.source << "};" << endl << endl;
	}
}

void NamespaceNode::PrintXml(opXmlStream& stream)
{
	opXmlTag namespacetag(stream,"Namespace");

	{
		opXmlTextTag nametag(stream,"Name");

		stream << Name->GetValue();
	}

	PrintXmlChildren(stream);
}

void NamespaceNode::PrintTop(opFileStream& stream, int& numscopes)
{
// 	//scoped name case
// 	if(ScopeNode* scope = Decl->GetScopeName())
// 	{
// 		int scopenum = scope->GetScopeNum();
// 		for(int i = 0; i < scopenum; i++)
// 		{
// 			stream.header << "namespace ";
// 			scope->GetScopeNode(i)->PrintOriginal(stream.header.body);
// 			stream.header << endl << '{'; 
// 			stream.header.Indent();
// 			stream.header << endl;
// 
// 			stream.source << "namespace ";
// 			scope->GetScopeNode(i)->PrintOriginal(stream.source.body);
// 			stream.source << endl << '{';
// 			stream.source.Indent();
// 			stream.source << endl;
// 		}
// 
// 		numscopes = scopenum;
// 	}
// 	else
// 	{
// 		//name case
// 		numscopes = 1;
// 
// 		stream.header << "namespace ";
// 		Decl->PrintOriginal(stream.header.body);//F really correct
// 		stream.header << endl << '{'; 
// 		stream.header.Indent();
// 		stream.header << endl;
// 
// 		stream.source << "namespace ";
// 		Decl->PrintOriginal(stream.source.body);//not really correct
// 		stream.source << endl << '{';
// 		stream.source.Indent();
// 		stream.source << endl;
// 	}

	stream.header << "namespace ";
	Name->PrintOriginal(stream.header.body);
	stream.header << endl << '{'; 
	stream.header.Indent();
	stream.header << endl;

	numscopes = 1;

// 	stream.source << "namespace ";
// 	Name->PrintOriginal(stream.source.body);
// 	stream.source << endl << '{';
// 	stream.source.Indent();
// 	stream.source << endl;
// 
}

//
// NamespaceAliasNode
//

void NamespaceAliasNode::PrintNode(opFileStream& stream )
{
	stream.header.body << "namespace ";
	stream.header.body << Name;
	stream.header.body << " = ";

	int num = Scopes.Size();
	for(int i = 0; i < num; i++)
	{
		stream.header.body << Scopes[i];
		
		if(i + 1 < num)
			stream.header.body << "::";
	}

	stream.header.body << ";";
}

///==========================================
/// UsingNode
///==========================================

void UsingNode::PrintTransformed( opSectionStream& stream )
{
	PrintOriginal(stream);
}

void UsingNode::PrintOriginal(opSectionStream& stream )
{
	stream << "using ";

	Scope->PrintOriginal(stream);

//	stream << ";";
}

void UsingNode::PrintString(opString& s)
{
	s += "using ";

	Scope->PrintString(s);
}

// 
// BraceBlockNode
//

void BraceBlockNode::PrintTransformed( opSectionStream& stream )
{
	stream << '{';
	PrintTransformedChildren(stream);
	stream << '}';
}

void BraceBlockNode::PrintOriginal( opSectionStream& stream )
{
	stream << '{';
	PrintOriginalUnspacedChildren(stream);
	stream << '}';
}

void BraceBlockNode::PrintOriginalTrimmed(opSectionStream& stream)
{
	stream << '{';
	PrintOriginalUnspacedChildren(stream);
	stream.TrimLineEnd();
	stream << '}';
}

void BraceBlockNode::PrintString(opString& s)
{
	s += '{';
	PrintStringChildren(s);
	s += '}';
}

void BraceBlockNode::MacroPrint(opSectionStream& stream, int& charnum)
{
	stream << '{';
	charnum += 1;
	MacroPrintChildren(stream,charnum);
	stream << '}';
	charnum += 1;
}

// 
// ParenBlockNode
//

void ParenBlockNode::PrintTransformed( opSectionStream& stream )
{
	stream << '(';
	PrintTransformedChildren(stream);
	stream << ')';
}

void ParenBlockNode::PrintOriginal( opSectionStream& stream )
{
	stream << '(';
	PrintOriginalUnspacedChildren(stream);
	stream << ')';
}

void ParenBlockNode::PrintString(opString& s)
{
	s += '(';
	PrintStringUnspacedChildren(s);
	s += ')';
}

void ParenBlockNode::MacroPrint(opSectionStream& stream, int& charnum)
{
	stream << '(';
	charnum += 1;	
	MacroPrintChildren(stream,charnum);
	stream << ')';
	charnum += 1;	
}

// 
// BracketBlockNode
//

void BracketBlockNode::PrintTransformed( opSectionStream& stream )
{
	stream << '[';
	PrintTransformedChildren(stream);
	stream << ']';
}

void BracketBlockNode::PrintOriginal( opSectionStream& stream )
{
	stream << '[';
	PrintOriginalUnspacedChildren(stream);
	stream << ']';
}

void BracketBlockNode::PrintString(opString& s)
{
	s += '[';
	PrintStringUnspacedChildren(s);
	s += ']';
}

void BracketBlockNode::MacroPrint(opSectionStream& stream, int& charnum)
{
	stream << '[';
	charnum += 1;	
	MacroPrintChildren(stream,charnum);
	stream << ']';
	charnum += 1;	
}

// 
// AngledBlockNode
//

void AngledBlockNode::PrintTransformed( opSectionStream& stream )
{
	//I add a space due to c++'s weird angled bracket handling w/ templates
	stream << "< ";
	PrintTransformedChildren(stream);
	stream << " >";
}

void AngledBlockNode::PrintOriginal( opSectionStream& stream )
{
	stream << "< ";
	PrintOriginalChildren(stream);
	stream << " >";
}

void AngledBlockNode::PrintString(opString& s)
{
	s += "< ";
	//PrintStringUnspacedChildren(s);
	PrintStringChildren(s);
	s += " >";
}

//
// PointerNode
//

void PointerNode::PrintTransformed( opSectionStream& stream )
{
	PrintOriginal(stream);
}

void PointerNode::PrintOriginal(opSectionStream& stream )
{
	Type->PrintOriginal(stream);

	int num = GetNumStars();
	for (int i = 0; i < num; i++)
	{
		StarType type = Stars[i];
			
		stream << '*';
		
		if(type == Const)
			stream << " const";
		else if(type == Volatile)
			stream << " volatile";
		else if(type == ConstVolatile)
			stream << " const volatile";
	}
}

void PointerNode::PrintString(opString& s)
{
	Type->PrintString(s);

	int num = GetNumStars();
	for (int i = 0; i < num; i++)
	{
		StarType type = Stars[i];

		s += '*';

		if(type == Const)
			s += " const";
		else if(type == Volatile)
			s += " volatile";
		else if(type == ConstVolatile)
			s += " const volatile";
	}
}



///
/// OperatorNode
///

void OperatorNode::PrintOriginal( opSectionStream& stream )
{
	stream << "operator ";
	OperatorType->PrintOriginal(stream);
}

void OperatorNode::PrintString(opString& s)
{
	s += "operator ";
	
	//PrintStringChildren(s);
	OperatorType->PrintString(s);
}

//
// FunctionNode
//

void FunctionNode::PrintTransformed( opSectionStream& stream )
{
	PrintOriginal(stream);
}

void FunctionNode::PrintOriginal(opSectionStream& stream )
{
	Name->PrintOriginal(stream);
	Arguments->PrintOriginal(stream);
	
	if(bConst)
		stream << " const";

	if(Assignment)
		stream << " = " << Assignment;
}

void FunctionNode::PrintString(opString& s)
{
	Name->PrintString(s);
	Arguments->PrintString(s);

	if(bConst)
		s += " const";

	if(Assignment)
		s += " = " + Assignment->GetValue();

}
//printed with definitions, never with prototypes.
void FunctionNode::PrintDefaultless(opSectionStream& stream )
{
	Name->PrintOriginal(stream);
	Arguments->PrintDefaultless(stream);
	
	if(bConst)
		stream << " const";
	
// 	if(Assignment)
// 		stream << " = " << Assignment;
}

//
// FunctionArgumentListNode
//

void FunctionArgumentListNode::PrintOriginal(opSectionStream& stream )
{
	stream << '(';

	iterator i    = GetBegin();
	iterator last = GetLast();
	iterator end  = GetEnd();

	while (i != end)
	{
		i->PrintOriginal(stream);

		if (i != last)
			stream << ", ";

		++i;
	}

	stream << ')';
}

void FunctionArgumentListNode::PrintString( opString& s )
{
	s += '(';

	iterator i    = GetBegin();
	iterator last = GetLast();
	iterator end  = GetEnd();

	while (i != end)
	{
		i->PrintString(s);

		if (i != last)
			s += ", ";

		++i;
	}

	s += ')';
}

void FunctionArgumentListNode::PrintDefaultless(opSectionStream& stream)
{
	stream << '(';

	int size = (int)Arguments.size();
	for (int i = 0; i < size; i++)
	{
		Arguments[i]->GetDefinition()->PrintOriginal(stream);

		if(i < size-1)
			stream << ", ";
	}

	stream << ')';
}

void FunctionArgumentListNode::PrintNameOnly(opSectionStream& stream)
{
	stream << '(';

	int size = (int)Arguments.size();
	for (int i = 0; i < size; i++)
	{
		DataStatementNode* datastatement = Arguments[i]->GetDefinition()->GetInnerStatement();
		
		datastatement->GetName()->PrintOriginal(stream);

		if(i < size-1)
			stream << ", ";
	}

	stream << ')';
}

void FunctionArgumentListNode::PrintSignature(opSectionStream& stream)
{
	stream << '(';

	int size = (int)Arguments.size();
	for (int i = 0; i < size; i++)
	{
		Arguments[i]->GetDefinition()->GetInnerStatement()->GetType()->PrintOriginal(stream);

		if(i < size-1)
			stream << ", ";
	}

	stream << ')';
}

void FunctionArgumentListNode::PrintSignature(opString& s)
{
	s += '(';

	int size = (int)Arguments.size();
	for (int i = 0; i < size; i++)
	{
		ModifiersNode* mods = Arguments[i]->GetDefinition()->GetInnerStatement()->GetModifiers();
		if(mods)
			mods->PrintBuiltInArgument(s);
		
		Arguments[i]->GetDefinition()->GetInnerStatement()->GetType()->PrintString(s);

		if(i < size-1)
			s += ',';
	}

	s += ')';
}

//
// FunctionArgumentNode
//

void FunctionArgumentNode::PrintOriginal(opSectionStream& stream)
{
	if(Definition)
		Definition->PrintOriginal(stream);

	if(Default)
	{
		stream << " = ";
		Default->PrintOriginal(stream);
	}
}

void FunctionArgumentNode::PrintString(opString& s)
{
	if(Definition)
		Definition->PrintString(s);

	if(Default)
	{
		s += " = ";
		Default->PrintString(s);
	}
}

//
// FunctionPrototypeNode
//

void FunctionPrototypeNode::PrintTransformed( opSectionStream& stream )
{
	//header << "prototype ";
	Return->PrintOriginal(stream);
	stream << ' ';
	Function->PrintOriginal(stream);
}

void FunctionPrototypeNode::PrintOriginal(opSectionStream& stream )
{
	PrintOriginalChildren(stream);
}

void FunctionPrototypeNode::PrintString(opString& s)
{
	Return->PrintString(s);
	s += ' ';
	Function->PrintString(s);
}
//
// FunctionDeclNode
//

void FunctionDefinitionNode::PrintTransformed( opSectionStream& stream )
{
	//header << "declaration ";
	Return->PrintOriginal(stream);
	stream << ' ';
	Function->PrintOriginal(stream);
	stream << endl;
	stream << '{';
	stream.Indent();
	stream << endl;
	Body->PrintOriginalChildren(stream);
	stream.DeIndent();
	stream << '}';
	stream << endl << endl;
}

void FunctionDefinitionNode::PrintOriginal(opSectionStream& stream )
{
	PrintTransformed(stream);
}

//
// OPEnumNode
//

void OPEnumNode::PrintNode( opFileStream& stream )
{
	DialectEnumeration* enumsettings = GetEnumSettings();
	
	//print the enum identifier note
	DialectNote& identifiernote = enumsettings->GetIdentifierNote();
	
	stream.header.body.PrintComment("//special note: " + enumsettings->GetName() + "::identifier");
	PrintNote(identifiernote,stream.header.body);
	
	//print name
	stream.header.body << endl;
	Name->PrintOriginal( stream.header.body );
	stream.header.body << endl;
	//stream.header.body << endl << Name->GetValue() << endl;
	
	//print the enum parent note
	DialectNote& parentnote = enumsettings->GetParentNote();
	
	stream.header.body.PrintComment("//special note: " + enumsettings->GetName() + "::parent");
	PrintNote(parentnote,stream.header.body);
	
	stream.header.body << endl << "{" << endl;
	stream.header.body.Indent();
	
	//Print Body Location
	DialectEnumLocation* bodylocationsettings = enumsettings->FindLocation(DialectLocation::body);
	PrintLocationNotes(bodylocationsettings,stream.header.body);

	stream.header.body.DeIndent();
	stream.header.body << "};" << endl << endl;

	//Print Post Location
	DialectEnumLocation* postlocationsettings = enumsettings->FindLocation(DialectLocation::post);
	PrintLocationNotes(postlocationsettings,stream.header.body);

	//Print Header Location
//	if(!IsInside(G_OPOBJECT))
	{
		DialectEnumLocation* headinglocationsettings = enumsettings->FindLocation(DialectLocation::heading);
		PrintLocationNotes(headinglocationsettings,stream.header.heading);
	}
	
	//Print Footer Location
	DialectEnumLocation* footerlocationsettings = enumsettings->FindLocation(DialectLocation::footer);
	PrintLocationNotes(footerlocationsettings,stream.header.footer);
	
	//Print Source Location
	DialectEnumLocation* sourcelocationsettings = enumsettings->FindLocation(DialectLocation::source);
	PrintLocationNotes(sourcelocationsettings,stream.source.body);
	
	//Print SourceHeading Location
	DialectEnumLocation* sourceheadinglocationsettings = enumsettings->FindLocation(DialectLocation::sourceheading);
	PrintLocationNotes(sourceheadinglocationsettings,stream.source.heading);
	
	//Print SourceFooter Location
	DialectEnumLocation* sourcefooterlocationsettings = enumsettings->FindLocation(DialectLocation::sourcefooter);
	PrintLocationNotes(sourcefooterlocationsettings,stream.source.footer);
}

//
// OPEnumElementNode
//

void OPEnumElementNode::PrintTransformed( opSectionStream& stream )
{
	PrintOriginal(stream);
}

void OPEnumElementNode::PrintOriginal(opSectionStream& stream )
{
	stream << Name;

	if(Initialization)
	{
		stream << " = ";
		Initialization->PrintOriginal(stream);
	}
}

///
/// OPObjectNode
///

opString OPObjectNode::GetParentName()
{
	opString str;
	
	if(Inheritance)
		Inheritance->PrintParentNames(str);
	
	return str;
}

void OPObjectNode::PrintInheritance(opSectionStream& stream)
{
	DialectNote& parentnote = GetCategorySettings()->GetParentNote();
	DialectNote& noparentnote = GetCategorySettings()->GetNoParentNote();
	bool		 bprintparent = parentnote.GetNoteDefinition()? !parentnote.GetNoteDefinition()->IsBodyEmpty() : false;
	bool		 bprintnoparent = noparentnote.GetNoteDefinition()? !noparentnote.GetNoteDefinition()->IsBodyEmpty() : false;
	
	if(bprintnoparent)
		bprintparent = false;
	
	//print inheritance with injected notes.
	if(Inheritance)
	{
		stream << ' ';
		
		Inheritance->PrintInheritance(stream);
		
		//if we have a parent note we'll need to add a comma
		if(bprintparent)
		{
			stream << ", ";
		}
	}
	else if(bprintparent || bprintnoparent)
	{
		stream << " : ";
		
		if(bprintnoparent)
		{
			PrintNote(noparentnote,stream);
		}
	}
	
	if(bprintparent)
	{
		//print the parent note
		PrintNote(parentnote,stream);
	}
}

void OPObjectNode::PrintNode( opFileStream& stream )
{	
	if(opParameters::Get().Notations
	|| opParameters::Get().PrintXml)
	{
		FetchAllModifiers();

		opNode::iterator it = Body->GetBegin();
		opNode::iterator end = Body->GetEnd();

		while(it != end)
		{
			if(StatementBase* statement = it->ToStatementBase())
				statement->FetchAllModifiers();
			++it;
		}
	}

	//The opObject printing process:
	//1. print out the body -category note-, the name, the inheritance, brace
	//2. print out the body notes
	//3. print out the transformed body
	//4. print out heading maps (conditional)
	//5. print out the footer maps
	
	DialectCategory* categorysettings = GetCategorySettings();
	
	DialectNote& identifiernote = categorysettings->GetIdentifierNote();
	
	//print the category identifier note
	stream.header.body.PrintComment("//special note: " + categorysettings->GetName() + "::identifier");
	PrintNote(identifiernote,stream.header.body);
	
	if (MiddleModifiers)
	{
		stream.header.body << ' ';
		MiddleModifiers->PrintNode(stream);
		stream.header.body << ' ';
	}

	//print name, inheritance
	//TODO: add baseclass note expansion (a special case issue)
	//stream.header.body << GetClassName();
	Name->PrintOriginal( stream.header.body );
	PrintInheritance(stream.header.body);
	stream.header.body << endl;
	
	//print header maps
	DialectLocation* headinglocationsettings = categorysettings->FindLocation(DialectLocation::heading);
	PrintLocationNotes(headinglocationsettings,stream.header.heading);
	
	//print footer maps
	DialectLocation* footerlocationsettings = categorysettings->FindLocation(DialectLocation::footer);
	PrintLocationNotes(footerlocationsettings,stream.header.footer);
	
	//print source maps
	DialectLocation* sourceheadinglocationsettings = categorysettings->FindLocation(DialectLocation::sourceheading);
	PrintLocationNotes(sourceheadinglocationsettings,stream.source.heading);	

	//print source maps
	DialectLocation* sourcelocationsettings = categorysettings->FindLocation(DialectLocation::source);
	PrintLocationNotes(sourcelocationsettings,stream.source.body);	

	//print source maps
	DialectLocation* sourcefooterlocationsettings = categorysettings->FindLocation(DialectLocation::sourcefooter);
	PrintLocationNotes(sourcefooterlocationsettings,stream.source.footer);	
	
	//print start brace & indent
	stream.header.body << '{' << endl;	
	stream.header.body.Indent();
	
	// Print body notes & maps:
	DialectLocation* bodylocationsettings = categorysettings->FindLocation(DialectLocation::body);
	PrintLocationNotes(bodylocationsettings,stream.header.body);
	
	// Print opObject Body Start //
	
	// Print transformed members:
	Body->PrintTransformedBody(stream);

	//De-indent header body
	stream.header.body.DeIndent();
	stream.header.body << "};" << endl << endl;
	
	// Print Demo Protection
	opDemoSupport::PrintDemoString(stream,GetClassName(),this);
	
	//print post body notes
	DialectLocation* postlocationsettings = categorysettings->FindLocation(DialectLocation::post);
	PrintLocationNotes(postlocationsettings,stream.header.body);
	
	// Print opObject Body End //
	
//inner printing disabled:
//  print any inner opobject notes
// 	for(size_t i = 0; i < Body->OPObjectStatements.size(); i++)
// 	{
// 		OPObjectNode* o = Body->OPObjectStatements[i]->GetObject();
// 		o->PrintMacroNote(stream.header.body,o->GetMacroTypeName(),"INNER_NOTE");
// 	}
//states disabled:
//  print state maps
//  Body->PrintStateMaps(stream,MacroTypeName);
	
	//print heading maps, only if an outer class
	//if(!IsInsideClass())
	
}

void OPObjectNode::PrintXml(opXmlStream& stream)
{
	opXmlTag objecttag(stream,"Object");

	{
		opXmlTextTag nametag(stream,"Name");

		stream << Name->GetValue();
	}

	{
		opXmlTag nametag(stream,"Category");

		stream << GetCategoryName();
	}

	PrintXmlChildren(stream);
}

void DataStatementBase::PrintXml(opXmlStream& stream)
{
	opXmlTag datatag(stream, "Data");

	StatementModifierBase::PrintXml(stream);
}

void FunctionStatementBase::PrintXml(opXmlStream& stream)
{
	opXmlTag functiontag(stream, "Function");

	StatementModifierBase::PrintXml(stream);
}

bool DataStatementBase::GetModifierDescription(const opString& name, opString& description)
{
	if(name.EndsWith("()"))
	{
		if(ModifierDescriptions::GetValuedModifier(name,description))
			return true;

		if(DataModifierNode* node = GetCategorySettings()->FindValueDataModifier(name.RLeft(2)))
		{
			if(node->GetDescription(description))
				return true;
		}
	}
	else
	{
		if(ModifierDescriptions::GetModifier(name,description))
			return true;

		if(DataModifierNode* node = GetCategorySettings()->FindDataModifier(name))
		{
			if(node->GetDescription(description))
				return true;
		}
	}

	return false;
}


bool FunctionStatementBase::GetModifierDescription(const opString& name, opString& description)
{
	if(name.EndsWith("()"))
	{
		if(ModifierDescriptions::GetValuedModifier(name,description))
			return true;

		if(FunctionModifierNode* node = GetCategorySettings()->FindValueFunctionModifier(name.RLeft(2)))
		{
			if(node->GetDescription(description))
				return true;
		}
	}
	else
	{
		if(ModifierDescriptions::GetModifier(name,description))
			return true;

		if(FunctionModifierNode* node = GetCategorySettings()->FindFunctionModifier(name))
		{
			if(node->GetDescription(description))
				return true;
		}	
	}



	return false;
}

void StatementModifierBase::PrintXml(opXmlStream& stream)
{
	{
		opXmlTextTag nametag(stream,"Name");

		stream << GetMemberName()->GetStringValue();
	}
	
	{
		opXmlTag nametag(stream,"Definition");

		opString original;
		PrintStatementString(original);

		stream << original;
	}
	
	{
		ModifiersNode* mods = GetModifiers();

		if(mods)
		{		
			opXmlTag nametag(stream,"Modifiers");
			
			opNode::iterator it = mods->GetBegin();
			opNode::iterator end = mods->GetEnd();
			
			while(it != end)
			{
				opXmlTag modtag(stream,"Modifier");

				TerminalNode*		basic	= node_cast<TerminalNode>(*it);
				ValuedModifierNode*	valued	= node_cast<ValuedModifierNode>(*it);

				{
					opXmlTextTag nametag(stream,"Name");

					if(basic)
						stream << basic->GetValue();
					if(valued)
						stream << valued->GetModifier()->GetValue();
				}

				if(valued)
				{
					opXmlTextTag nametag(stream,"Value");

					stream << valued->GetArguments()->GetStringValue();
				}
				
				//attempt to get a description
				opString modname;
				if(basic)
					modname = basic->GetValue();
				if(valued)
					modname = valued->GetModifier()->GetValue() + "()";

				opString description;

				if(GetModifierDescription(modname,description))
				{
					opXmlTextTag d(stream,"Description");

					stream << description;
				}

				++it;
			}
		}
	}
	
	{
		ModifiersNode* mods = GetAutoModifiers();

		if(mods)
		{
			opXmlTag nametag(stream,"Automatics");

			opNode::iterator it = mods->GetBegin();
			opNode::iterator end = mods->GetEnd();

			while(it != end)
			{
				opXmlTag modtag(stream,"Modifier");

				TerminalNode*		basic	= node_cast<TerminalNode>(*it);
				ValuedModifierNode*	valued	= node_cast<ValuedModifierNode>(*it);

				{
					opXmlTextTag nametag(stream,"Name");

					if(basic)
						stream << basic->GetValue();
					if(valued)
						stream << valued->GetModifier()->GetValue();
				}

				if(valued)
				{
					opXmlTextTag nametag(stream,"Value");

					stream << valued->GetArguments()->GetStringValue();
				}
				
				//attempt to get a description
				opString modname;
				if(basic)
					modname = basic->GetValue();
				if(valued)
					modname = valued->GetModifier()->GetValue() + "()";

				opString description;

				if(GetModifierDescription(modname,description))
				{
					opXmlTextTag d(stream,"Description");

					stream << description;
				}

				++it;
			}
		}
	}
	
	PrintXmlChildren(stream);
}

void OPObjectStatementNode::PrintXml(opXmlStream& stream)
{
	PrintXmlChildren(stream);
}

///
/// OPObjectInheritanceNode
///

void OPObjectInheritanceNode::PrintTransformed( opSectionStream& stream )
{
	PrintTransformedChildren(stream);
}

void OPObjectInheritanceNode::PrintParentNames(opString& s)
{
	s += " : ";
	
	int num = Parents.Size();
	for(int i = 0; i < num; i++)
	{
		Parent& p = Parents[i];
		
		if(p.Virtual)
		{
			p.Virtual->PrintString(s);		
			s += " ";
		}

		if(p.Visibility)
		{
			p.Visibility->PrintString(s);
			s += " ";
		}

		p.Type->PrintString(s);

		if(i + 1 < num)
			s += ", ";
	}
}

void OPObjectInheritanceNode::PrintInheritance(opSectionStream& stream)
{
	PrintOriginalChildren(stream);
}

void OPObjectInheritanceNode::PrintInheritance(opString& s)
{
	PrintStringChildren(s);
}

///
/// OPObjectBodyNode
///

void OPObjectBodyNode::PrintTransformedBody(opFileStream& stream)
{
	iterator       i           = GetBegin();
	iterator       end         = GetEnd();
	bool           bFoundFirst = false;
	VisibilityMode lastlabel   = vismode_default;
	VisibilityMode lastprinted;
	
	while (i != end)
	{
		if (StatementBase* statement = i->ToStatementBase())
		{
			if (VisibilityStatementNode* vis = statement->ToVisibilityStatementNode())
			{
				lastlabel = vis->GetVisibilityMode();

				if (!bFoundFirst)
				{
					Object->PrintVisibility(lastlabel, stream.header.body);					

					lastprinted = lastlabel;
					bFoundFirst = true;
				}
				else if (lastlabel != lastprinted)
				{
					Object->PrintVisibility(lastlabel, stream.header.body);					

					lastprinted = lastlabel;
				}
			}
			else
			{
				VisibilityMode mode = statement->GetVisibilityMode();

				if (!bFoundFirst)
				{
					if (mode == vismode_none)
						mode = vismode_default;

					Object->PrintVisibility(mode, stream.header.body);

					lastprinted = mode;
					bFoundFirst = true;
				}
				else if (mode        == vismode_none
				&&       lastprinted != lastlabel)
				{
					Object->PrintVisibility(lastlabel, stream.header.body);

					lastprinted = lastlabel;
				}
				else if (mode != vismode_none
					&&   mode != lastprinted)
				{
					Object->PrintVisibility(mode, stream.header.body);
					lastprinted = mode;
				}
			}
		}

		i->PrintNode(stream);

		++i;
	}
}

//
// StateNode
//

void StateNode::PrintTransformed( opSectionStream& stream )
{
	//this isn't really right (just for printing now..)
	stream << "state ";
	stream << Name;
	
	stream << endl;
	stream << '{' << endl;
	
	stream.Indent();
	Body->PrintTransformed(stream);
	stream.DeIndent();

	stream << "};" << endl << endl;
}

//
// StateBlockNode
//

void StateBlockNode::PrintTransformed( opSectionStream& stream )
{
	PrintTransformedChildren(stream);
}

//
// TemplateDecl
//

void TemplateDeclNode::PrintTransformed( opSectionStream& stream )
{
	stream << "template";
	braces->PrintTransformed(stream);
}

//
// TemplateType
//

void TemplateTypeNode::PrintTransformed( opSectionStream& stream )
{
	PrintOriginal(stream);
}

void TemplateTypeNode::PrintOriginal(opSectionStream& stream )
{
	stream << Name;
	Braces->PrintOriginal(stream);
	stream << " ";
}

void TemplateTypeNode::PrintString(opString& s)
{
	Name->PrintString(s);
	Braces->PrintString(s);
}

//
// Array
//

void ArrayNode::PrintTransformed( opSectionStream& stream )
{
	PrintOriginal(stream);
}

void ArrayNode::PrintOriginal(opSectionStream& stream )
{
	stream << Name;

	for(int i = 0; i < (int)Brackets.size(); i++)
		Brackets[i]->PrintOriginal(stream);
}

void ArrayNode::PrintString(opString& s)
{
	s += Name->GetValue();

	for(int i = 0; i < (int)Brackets.size(); i++)
		Brackets[i]->PrintString(s);
}

//
// TypeArray
//

void TypeArrayNode::PrintTransformed( opSectionStream& stream )
{
	PrintOriginal(stream);
}

void TypeArrayNode::PrintOriginal(opSectionStream& stream )
{
	Type->PrintOriginal(stream);

	for(int i = 0; i < (int)Brackets.size(); i++)
		Brackets[i]->PrintOriginal(stream);
}

//
// Reference
//

void ReferenceNode::PrintTransformed( opSectionStream& stream )
{
	PrintOriginal(stream);
}

void ReferenceNode::PrintOriginal( opSectionStream& stream )
{
	Type->PrintOriginal(stream);
	stream << '&';
}

void ReferenceNode::PrintString(opString& s)
{
	Type->PrintString(s);
	s += '&';
}

//
// Visibility Label
//

void VisibilityLabelNode::PrintTransformed( opSectionStream& stream )
{
	//don't print anything
}

void VisibilityLabelNode::PrintOriginal( opSectionStream& stream )
{
	stream << Label << ":" << endl;
}

//
// ScopeNode
//

void ScopeNode::PrintTransformed( opSectionStream& stream )
{
	PrintOriginal(stream);
}

void ScopeNode::PrintOriginal(opSectionStream & stream)
{
	if(bGlobal)
		stream << "::";

	int size = (int) Scope.size() - 1;

	for (int i = 0; i < size; i++)
	{
		Scope[i]->PrintOriginal(stream);
		stream << "::";
	}

	Scope[size]->PrintOriginal(stream);
}

void ScopeNode::PrintString(opString& s)
{
	if(bGlobal)
		s += "::";

	int size = (int) Scope.size() - 1;

	for (int i = 0; i < size; i++)
	{
		Scope[i]->PrintString(s);
		s += "::";
	}

	Scope[size]->PrintString(s);
}


///
/// ScopePointerNode
///

void ScopePointerNode::PrintTransformed(opSectionStream & stream)
{
	PrintOriginal(stream);
}

void ScopePointerNode::PrintOriginal(opSectionStream & stream)
{
	if(bGlobal)
		stream << "::";

	int size = (int) Scope.size() - 1;

	for (int i = 0; i < size; i++)
	{
		Scope[i]->PrintOriginal(stream);
		stream << "::";
	}

	Scope[size]->PrintOriginal(stream);

	stream << "::";

	for(int i = 0; i < Stars; i++)
		stream << "*";
}

void ScopePointerNode::PrintString(opString& s)
{
	if(bGlobal)
		s += "::";

	int size = (int) Scope.size() - 1;

	for (int i = 0; i < size; i++)
	{
		Scope[i]->PrintString(s);
		s += "::";
	}

	Scope[size]->PrintString(s);

	s += "::";

	for(int i = 0; i < Stars; i++)
		s += "*";
}


///
/// PointerMemberNode
///

void PointerMemberNode::PrintOriginal( opSectionStream& stream )
{
	Type->PrintOriginal(stream);
	stream << ' ';
	Scope->PrintOriginal(stream);
}

void PointerMemberNode::PrintString(opString& s)
{
	Type->PrintString(s);
	s += ' ';
	Scope->PrintString(s);
}


///
/// TypenameNode
///

void TypenameNode::PrintTransformed( opSectionStream& stream )
{
	stream << "typename ";

	Name->PrintOriginal(stream);
}

///
/// FriendNode
///

void FriendNode::PrintOriginal(opSectionStream& stream )
{
	stream << "friend ";
	
	if(GetForwardDeclType() == Class)
		stream << "class ";
	else if(GetForwardDeclType() == Struct)
		stream << "struct ";

	Friend->PrintOriginal(stream);

	if (bHasSemicolon)
		stream << ';';
}

FriendNode::ForwardDeclType FriendNode::GetForwardDeclType()
{
	if(!ForwardDecl)
		return None;
	if(ForwardDecl->GetId() == T_STRUCT)
		return Struct;
	if(ForwardDecl->GetId() == T_CLASS)
		return Class;
	return None;
}


///
/// TypedefNode
///

void TypedefNode::PrintTransformed( opSectionStream& stream )
{
	stream << "typedef ";

	Type->PrintOriginal(stream);

	stream << ' ';

	Name->PrintOriginal(stream);

	stream << ';';
}

///
/// FunctionPointerNode
///

void FunctionPointerNode::PrintTransformed( opSectionStream& stream )
{
	PrintOriginal(stream);
}

void FunctionPointerNode::PrintOriginal(opSectionStream& stream )
{
	Return->PrintOriginal(stream);
	stream << ' ';
	Decl->PrintOriginal(stream);
	Arguments->PrintOriginal(stream);
}

///
/// FunctionPointerDeclNode
///

void FunctionPointerDeclNode::PrintTransformed(opSectionStream& stream)
{
	PrintOriginal(stream);
}

void FunctionPointerDeclNode::PrintOriginal(opSectionStream& stream)
{
	// might have to change this for function pointers
	// like void (A::*Hello)(float,float) ...
	// TODO: allow more complicated function pointers.
 	stream << "(";
	Scope->PrintOriginal(stream);
	stream << ' ';
	Name->PrintOriginal(stream);
	stream << ')';
}

void FunctionPointerDeclNode::PrintString(opString& s)
{
	s += '(';
	Scope->PrintString(s);
	s += ' ';
	Name->PrintString(s);
	s += ')';
}

///
/// OPMacroNode
///

void OPDefineNode::PrintTransformed( opSectionStream& stream )
{
	DirectiveOverride alter_redirection(false);

	//print the undef
	stream << endl;
	stream << "#undef ";
	Name->PrintOriginal(stream);
	stream << endl;

	//print the define
	stream << "#define ";
	
	//NOTE: uses some trickery to get it to macro print
	//		this first line
	int numchar = 8;
	Name->MacroPrint(stream,numchar);

	if (Arguments)
		Arguments->MacroPrint(stream,numchar);

	TerminalNode::MacroPrintEndl(stream,numchar);

	Body->PrintTransformed(stream);

	{
		ForceNewlines forceNewlines(stream.body);

		stream << ' ' << endl;
	}	
}

///
/// OPMacroBodyNode
///

void OPDefineBodyNode::PrintTransformed( opSectionStream& stream )
{
	int charnum = 0;
	MacroPrintChildren(stream,charnum);
}

///
/// PreprocessorNode
///

void PreprocessorNode::PrintTransformed( opSectionStream& stream )
{
	PrintOriginal(stream);
}

void PreprocessorNode::PrintOriginal(opSectionStream& stream )
{
	stream << '#';
	stream << DirectiveName;

	PrintOriginalUnspacedChildren(stream);

	{
		ForceNewlines forceNewlines(stream.body);

		stream << ' ' << endl;
	}
}

void PreprocessorNode::PrintString(opString& s)
{
	s += '#';
	s += DirectiveName;
	PrintStringUnspacedChildren(s);
	s += " \n";	
}

//
// OPIncludeNode
//

void OPIncludeNode::PrintNode(opFileStream& stream)
{
	stream.PrintComment("//opinclude " + FileName->GetValue());
	stream << endl;
	
	stream.header.heading << "#include \"" << opDriver::ToGeneratedPath(GetFullFileString()) << ".ooh" << "\"" << endl;
	stream.header.body    << "#include \"" << opDriver::ToGeneratedPath(GetFullFileString()) << ".ooh" << "\"" << endl;
	stream.header.footer  << "#include \"" << opDriver::ToGeneratedPath(GetFullFileString()) << ".ooh" << "\"" << endl;
	stream.source.heading << "#include \"" << opDriver::ToGeneratedPath(GetFullFileString()) << ".ooh" << "\"" << endl;
	stream.source.body	  << "#include \"" << opDriver::ToGeneratedPath(GetFullFileString()) << ".ooh" << "\"" << endl;
	stream.source.footer  << "#include \"" << opDriver::ToGeneratedPath(GetFullFileString()) << ".ooh" << "\"" << endl;

	//IncludedFile->PrintIncluded(stream);
	//IncludedFile->PrintNode(stream);
}

void OPIncludeNode::PrintDialectNode(opDialectStream& stream)
{
	stream.PrintComment("//opinclude " + FileName->GetValue());
	stream << endl;
	IncludedFile->PrintIncluded(stream);
}

void OPIncludeNode::PrintTransformed( opSectionStream& stream )
{
	//actually opinclude prints nothing...just a comment
	stream.PrintComment("//opinclude " + FileName->GetValue());
	stream << endl;
	//TODO: make it redirect to the expected output filename.
}

void OPIncludeNode::PrintOriginal(opSectionStream& stream )
{
	stream.PrintComment("//opinclude " + FileName->GetValue());
	stream << endl;
	//stream << "#include \"" << generatedpath.string() << "\"" << endl;
	
	//we do want to print inside code blocks
	if(IsInside(G_CODE))
		IncludedFile->PrintOriginal(stream);
}

//
// StatementNode
//

void StatementNode::PrintTransformed(opSectionStream& stream)
{
	if ( !node_cast< NullStatementNode >( GetInnerStatement() ) )
	{
		PrintTransformedChildren(stream);
		stream << ';' << endl;
	}
}

void StatementNode::PrintNode(opFileStream& stream)
{
	if ( !node_cast< NullStatementNode >( GetInnerStatement() ) )
	{
		PrintNodeChildren(stream);
		stream << ';' << endl;
	}
}

//
// DataStatementNode
//

void DataStatementNode::PrintTransformed(opSectionStream& stream)
{
	if(modifiers)
 		modifiers->PrintBuiltIn(stream);	
	
	Declaration->PrintTransformed(stream);

	//if we have static we need to print initialization, otherwise don't (hardcoded transform!)
	if(Initialization)
	{
		if(modifiers && modifiers->HasModifier(T_STATIC))
		{
			stream << " = ";
			Initialization->PrintTransformed(stream);
		}
	}
}

void DataStatementNode::PrintOriginal( opSectionStream& stream )
{
	if(modifiers)
	{
		modifiers->PrintOriginal(stream);
		stream << ' ';
	}

	Declaration->PrintOriginal(stream);

	if(Initialization)
	{
		stream << " = ";
		Initialization->PrintOriginal(stream);
	}
}

void DataStatementNode::PrintString(opString& s)
{
	if(modifiers)
	{
		modifiers->PrintString(s);
		s += ' ';
	}

	Declaration->PrintString(s);

	if(Initialization)
	{
		s += " = ";
		Initialization->PrintString(s);
	}
}

void DataStatementNode::PrintStatementString(opString& s)
{
	Declaration->PrintString(s);
}
//
// DataDeclarationNode
//

void DataDeclarationNode::PrintOriginal(opSectionStream& stream )
{
	Type->PrintOriginal(stream);
	stream << ' ';
	Name->PrintOriginal(stream);

	if(Bits)
	{
		stream << " : ";
		stream << Bits->GetNumber();
	}
}


void DataDeclarationNode::PrintString(opString& s)
{
	Type->PrintString(s);
	s += " ";
	Name->PrintString(s);

	if(Bits)
	{
		s += " : ";
		Bits->GetNumber()->PrintString(s);
	}
}

//
// ModifiersNode
//

//print c++ valid modifiers
void ModifiersNode::PrintBuiltIn(opSectionStream& stream)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		Token token = i->GetId();
		
		if(token == T_STATIC
		|| token == T_CONST
		|| token == T_INLINE
		|| token == T_VOLATILE
		|| token == T_VIRTUAL
		|| token == T_EXPLICIT
		|| token == G_CPLUSPLUS)
		{
			i->PrintOriginal(stream);
			stream << ' ';
		}
		//NOTE: special case for opstatic, need to add transforms!
		else if ( token == T_MODIFIER 
		&&        node_cast<TerminalNode>( *i )->GetValue() == "opstatic" )
		{
			stream << "static ";
		}

		++i;
	}
}

//TODO: this is a bit weird (used on state functions only)
//		need to verify functions differently if state functions
void ModifiersNode::PrintBuiltInReturnArgument(opString& s)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		Token token = i->GetId();

		if(token == T_CONST
		|| token == T_VOLATILE)
		{
			i->PrintString(s);
			s += ' ';
		}

		++i;
	}
}

void ModifiersNode::PrintBuiltInArgument(opString& s)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		Token token = i->GetId();

		if(token == T_CONST
		|| token == T_VOLATILE)//is volatile allowed?
		{
			i->PrintString(s);
			s += ' ';
		}

		++i;
	}
}

//NOTE: probably need one of these for data and functions each!
void ModifiersNode::PrintBuiltInSource(opSectionStream& stream)
{
	iterator i   = GetBegin();
	iterator end = GetEnd();

	while (i != end)
	{
		Token token = i->GetId();

		if(token == T_CONST 
		|| token == T_EXPLICIT
		)
		{
			i->PrintOriginal(stream);
			stream << ' ';
		}

		++i;
	}
}

//
// StateStatementNode
//

void StateStatementNode::PrintTransformed(opSectionStream& stream)
{
	//TODO: should use the visibility modifiers at least!
	State->PrintTransformed(stream);
}

void StateStatementNode::PrintNode(opFileStream& stream)
{
// 	vector<FunctionDefinitionStatementNode*>& FunctionStatements = 	GetState()->GetBody()->FunctionDefinitionStatements;
// 	//this prints the state functions within a state
// 
// 	size_t size = FunctionStatements.size();
// 	for(size_t i = 0; i < size; i++)
// 	{
// 		//now we want to print the function statements...
// 		//we want to do this specially..
// 		FunctionStatements[i]->PrintStateFunction(stream,this);
// 	}
}

//
// ConstructorDefinitionStatementNode
//

void ConstructorDefinitionStatementNode::PrintNode(opFileStream& file)
{
	// print the prototype
	PrintPrototype(file.header.body);

	// determine whether to inline or not
	bool bInline = GetInlineMode();

	// print the scoped definition 
	if(bInline)
		PrintScopedDefinition(file.header.footer);
	else
		PrintScopedDefinition(file.source.body);

}

void ConstructorDefinitionStatementNode::PrintPrototype(opSectionStream& stream)
{
	//print built in modifiers
	if(modifiers)
		modifiers->PrintBuiltIn(stream);
	
	//print function signature
	ConstructorDefinition->GetConstructor()->PrintOriginal(stream);
	stream << ';' << endl;
}

void ConstructorDefinitionStatementNode::PrintScopedDefinition(opSectionStream& stream)
{
	OPObjectNode*		object		= FindParent<OPObjectNode>();
	DialectNote&        prefixnote	= object->GetCategorySettings()->GetPrefixNote();
	DialectNote&        postfixnote = object->GetCategorySettings()->GetPostfixNote();
	ConstructorNode*	function    = ConstructorDefinition->GetConstructor();

	stream << endl;

	opString scope = GetClassScope();

	stream.PrintComment("//moved constructor definition: " 
		+ scope 
		+ ConstructorDefinition->GetConstructor()->GetName()->GetStringValue()
	);

	stream << endl;

	//print prefix note
	object->PrintNote(prefixnote,stream);

	//print source modifiers (const only!)
	if(modifiers)
		modifiers->PrintBuiltInSource(stream);
	
	stream << scope;

	object->PrintNote(postfixnote,stream);

	stream << "::";

	//print function signature (actually should be modified for default args)

	ConstructorDefinition->GetConstructor()->PrintDefaultless(stream);
	stream << endl;

	//print function initializers
	if(ConstructorDefinition->GetInitializers())
	{
		ConstructorDefinition->GetInitializers()->PrintOriginal(stream);
		stream << endl;
	}

	//print body
	ConstructorDefinition->GetBody()->PrintOriginalTrimmed(stream);
	stream << endl << endl;
}

void ConstructorDefinitionStatementNode::PrintOriginal(opSectionStream& stream)
{
	ConstructorDefinition->PrintOriginal(stream);
}



void ConstructorDefinitionStatementNode::PrintStatementString(opString& s)
{
	ConstructorDefinition->GetConstructor()->PrintString(s);
}
//
// ConstructorPrototypeStatementNode
//

void ConstructorPrototypeStatementNode::PrintNode(opFileStream& file)
{
	PrintOriginal(file.header.body);
}

void ConstructorPrototypeStatementNode::PrintOriginal(opSectionStream& stream)
{
	Constructor->PrintOriginal(stream);
}




void ConstructorPrototypeStatementNode::PrintStatementString(opString& s)
{
	Constructor->PrintString(s);
}
//
// ConstructorPrototypeNode
//

void ConstructorPrototypeNode::PrintOriginal(opSectionStream& stream)
{
	PrintOriginalChildren(stream);
}

//
// ConstructorDefinitionNode
//

void ConstructorDefinitionNode::PrintOriginal(opSectionStream& stream)
{
	Constructor->PrintOriginal(stream);
	
	if(Initializers)
		Initializers->PrintOriginal(stream);

	Body->PrintOriginal(stream);
}


//
// ConstructorNode
//

void ConstructorNode::PrintOriginal(opSectionStream& stream)
{
	PrintOriginalChildren(stream);
}
void ConstructorNode::PrintString(opString& s)
{
	function->PrintString(s);
}
//
// DestructorNode
//

void DestructorNode::PrintOriginal(opSectionStream& stream)
{
	stream << "~";
	PrintOriginalChildren(stream);
}

void DestructorNode::PrintString(opString& s)
{
	s += "~";
	function->PrintString(s);
}
//
// DestructorPrototypeNode
//

void DestructorPrototypeNode::PrintOriginal(opSectionStream& stream)
{
	Destructor->PrintOriginal(stream);
}

void DestructorPrototypeNode::PrintString(opString& s)
{
	Destructor->PrintString(s);
}
//
// DestructorDefinitionNode
//

void DestructorDefinitionNode::PrintOriginal(opSectionStream& stream)
{
	Destructor->PrintOriginal(stream);
	Body->PrintOriginal(stream);
}



//
// DestructorDefinitionStatementNode
//

void DestructorDefinitionStatementNode::PrintNode(opFileStream& file)
{
	// print the prototype
	PrintPrototype(file.header.body);

	// determine whether to inline or not
	bool bInline = GetInlineMode();

	// print the scoped definition 
	if(bInline)
		PrintScopedDefinition(file.header.footer);
	else
		PrintScopedDefinition(file.source.body);

}

void DestructorDefinitionStatementNode::PrintPrototype(opSectionStream& stream)
{
	//print built in modifiers
	if(modifiers)
		modifiers->PrintBuiltIn(stream);

	//print function signature
//	stream << '~';
	DestructorDefinition->GetDestructor()->PrintOriginal(stream);
	stream << ';' << endl;
}

void DestructorDefinitionStatementNode::PrintScopedDefinition(opSectionStream& stream)
{
	OPObjectNode*		object		= FindParent<OPObjectNode>();
	DialectNote&        prefixnote	= object->GetCategorySettings()->GetPrefixNote();
	DialectNote&        postfixnote = object->GetCategorySettings()->GetPostfixNote();
	DestructorNode*		function    = DestructorDefinition->GetDestructor();
	
	stream << endl;
	
	opString scope = GetClassScope();
	
	stream.PrintComment("//moved destructor definition: "
		+ scope
		+ DestructorDefinition->GetDestructor()->GetName()->GetStringValue()
		);
	
	stream << endl;
	
	//print prefix note
	object->PrintNote(prefixnote,stream);
	
	//print source modifiers (const only!)
	if(modifiers)
		modifiers->PrintBuiltInSource(stream);
	
	stream << scope;
	
	object->PrintNote(postfixnote,stream);
	
	stream << "::~";
	
	//print function signature (actually should be modified for default args)
	
	DestructorDefinition->GetDestructor()->PrintDefaultless(stream);
	stream << endl;
	
	//print body
	DestructorDefinition->GetBody()->PrintOriginalTrimmed(stream);
	stream << endl << endl;
}

void DestructorDefinitionStatementNode::PrintOriginal(opSectionStream& stream)
{
	DestructorDefinition->PrintOriginal(stream);
}



void DestructorDefinitionStatementNode::PrintStatementString(opString& s)
{
	DestructorDefinition->GetDestructor()->PrintString(s);
}
//
// DestructorPrototypeStatementNode
//

void DestructorPrototypeStatementNode::PrintNode(opFileStream& file)
{
	PrintOriginal(file.header.body);
}

void DestructorPrototypeStatementNode::PrintOriginal(opSectionStream& stream)
{
	//print built in modifiers
	if(modifiers)
		modifiers->PrintBuiltIn(stream);

	Destructor->PrintOriginal(stream);
}




void DestructorPrototypeStatementNode::PrintStatementString(opString& s)
{
	Destructor->PrintString(s);
}
//
// FunctionDefinitionStatementNode
//

void FunctionDefinitionStatementNode::PrintTransformed(opSectionStream& stream )
{
	if(modifiers)
		modifiers->PrintBuiltIn(stream);

	FunctionDefinition->PrintTransformed(stream);
}

void FunctionDefinitionStatementNode::PrintOriginal(opSectionStream& stream )
{
	if(modifiers)
	{
		modifiers->PrintOriginal(stream);
		stream << ' ';
	}
	
	FunctionDefinition->PrintOriginal(stream);
}

void FunctionDefinitionStatementNode::PrintNode(opFileStream& stream)
{
	PrintFunction(stream);
}



void FunctionDefinitionStatementNode::PrintStatementString(opString& s)
{
	FunctionDefinition->GetReturn()->PrintString(s);
	s += ' ';
	FunctionDefinition->GetFunction()->PrintString(s);
}


void FunctionDefinitionStatementNode::PrintFunction(opFileStream& stream)
{
	//inline modifier moves the function to the header footer
	//uninline modifier moves the function to the source body
	//uninline is by default enabled (if no inline specified)

	// Print the namespace_start in header.footer.
	// This is to help resolve moved functions in namespaced classes.
	OPObjectNode* parent = FindParent<OPObjectNode>();

	ValuedModifierNode* namespace_start = node_cast<ValuedModifierNode>(parent->GetContextArgument("namespace_start"));
	ValuedModifierNode* namespace_end   = node_cast<ValuedModifierNode>(parent->GetContextArgument("namespace_end"));

	bool bPrintNamespaces = namespace_start && namespace_end;

	bool bInline = GetInlineMode();
	
	PrintPrototype(stream.header.body);
	stream.header.body << ';' << endl;

	if(bInline)
	{
		if(bPrintNamespaces)
		{
			namespace_start->GetArguments()->PrintOriginal(stream.header.footer);
			stream.header.footer << endl;
		}

		//TODO: we only need class_scope, dont need to print the full namespace scope
		//now write the scoped signature & body to the header footer
		PrintScopedDefinition(stream.header.footer);

		if(bPrintNamespaces)
		{
			namespace_end->GetArguments()->PrintOriginal(stream.header.footer);
			stream.header.footer << endl;
		}
	}
	else
	{
		if(bPrintNamespaces)
		{
			namespace_start->GetArguments()->PrintOriginal(stream.source.body);
			stream.source.body << endl;
		}

		//write the scoped signature & body to the source body
		PrintScopedDefinition(stream.source.body);

		if(bPrintNamespaces)
		{
			namespace_end->GetArguments()->PrintOriginal(stream.source.body);
			stream.source.body << endl;
		}
	}
}

void FunctionDefinitionStatementNode::PrintPrototype(opSectionStream& stream)
{
	//print built in modifiers
	if(modifiers)
		modifiers->PrintBuiltIn(stream);
	
	//print return type
	FunctionDefinition->GetReturn()->PrintOriginal(stream);

	stream << ' ';

	//print function signature
	FunctionDefinition->GetFunction()->PrintOriginal(stream);
}

void FunctionDefinitionStatementNode::PrintScopedDefinition(opSectionStream& stream)
{
	OPObjectNode*		object		= FindParent<OPObjectNode>();
	DialectNote&        prefixnote	= object->GetCategorySettings()->GetPrefixNote();
	DialectNote&        postfixnote = object->GetCategorySettings()->GetPostfixNote();
	FunctionNode*		function    = FunctionDefinition->GetFunction();

	stream << endl;

	opString scope = GetClassScope();

	stream.PrintComment("//moved function definition: " 
		+ scope
		+ FunctionDefinition->GetFunction()->GetName()->GetStringValue()
		);

	stream << endl;

	//print prefix note
	object->PrintNote(prefixnote,stream);

	//print source modifiers (const only!)
	if(modifiers)
		modifiers->PrintBuiltInSource(stream);

	//print return type
	FunctionDefinition->GetReturn()->PrintOriginal(stream);
	stream << ' ' << scope;

	//print postfix note
	object->PrintNote(postfixnote,stream);

	stream << "::";

	//print function signature (actually should be modified for default args)
	
	FunctionDefinition->GetFunction()->PrintDefaultless(stream);
	stream << endl;

	//print body
	FunctionDefinition->GetBody()->PrintOriginalTrimmed(stream);
	stream << endl << endl;
}

//
// FunctionPointerStatementNode
//

void FuncPointerStatementNode::PrintTransformed(opSectionStream& stream)
{
	if(modifiers)
		modifiers->PrintBuiltIn(stream);

	FunctionPointer->PrintTransformed(stream);
}

void FuncPointerStatementNode::PrintOriginal(opSectionStream& stream )
{
	if(modifiers)
		modifiers->PrintOriginal(stream);

	FunctionPointer->PrintOriginal(stream);
}

void FuncPointerStatementNode::PrintString(opString& s)
{
	if(modifiers)
	{
		modifiers->PrintString(s);
		s += ' ';
	}

	FunctionPointer->PrintString(s);
}


void FuncPointerStatementNode::PrintStatementString(opString& s)
{
	FunctionPointer->PrintString(s);
}
//
// FunctionPrototypeStatementNode
//

void FuncPrototypeStatementNode::PrintTransformed(opSectionStream& stream)
{
	if(modifiers)
		modifiers->PrintBuiltIn(stream);

	FunctionPrototype->PrintTransformed(stream);
}

void FuncPrototypeStatementNode::PrintOriginal(opSectionStream& stream )
{
	if(modifiers)
	{
		modifiers->PrintOriginal(stream);
		stream << ' ';
	}

	FunctionPrototype->PrintOriginal(stream);
}


void FuncPrototypeStatementNode::PrintStatementString(opString& s)
{
	FunctionPrototype->PrintString(s);
}
//
// FunctionStatementBase
//

void FunctionStatementBase::PrintString(opString& s)
{
	if(modifiers)
	{
		modifiers->PrintString(s);
		s += ' ';
	}

	if(GetReturnType())
	{
		GetReturnType()->PrintString(s);
		s += ' ';
	}

	GetFunction()->PrintString(s);
}


//
// TemplateStatementNode
//

void TemplateStatementNode::PrintOriginal(opSectionStream& stream)
{
	Templated->PrintOriginal( stream );

	stream << ";";
}

//
// Dialect Printing
//

void CategoryNode::PrintDialectNode(opDialectStream& stream)
{
	stream.PrintComment("//category " + Name->GetValue());
	stream << endl;
	
	if(opParameters::Get().Highlighting)
	{
		stream << "#if 0" << endl;
		stream << "#define " << Name << " class" << endl;
		stream << "#endif" << endl;
	}
	
	//if ghosts enabled, print these..
	if(opParameters::Get().Ghosts)
	{
		stream << "#if 0" << endl;
		stream << "namespace " << Name << endl;
		stream << '{' << endl;
		stream.Indent();
	}
	
	Body->PrintDialectNode(stream);
	
	if(opParameters::Get().Ghosts)
	{
		stream.DeIndent();
		stream << endl;
		stream << '}' << endl;
		stream << "#endif" << endl;
	}
	
}


void EnumerationNode::PrintDialectNode(opDialectStream& stream)
{
	stream.PrintComment("//enumeration " + Name->GetValue());
	stream << endl;

	if(opParameters::Get().Highlighting)
	{
		stream << "#if 0" << endl;
		stream << "#define " << Name << " class" << endl;
		stream << "#endif" << endl;
	}
}

//TODO: figure out decent way to get intellisense w/ these
void DataModifierNode::PrintDialectNode(opDialectStream& stream)
{
	stream.PrintComment("//datamodifier " + Name->GetValue());
	stream << endl;
	
	if(opParameters::Get().Highlighting)
	{
		stream << "#if 0" << endl;
		stream << "#define " << Name << " const" << endl;
		stream << "#endif" << endl;
	}
}

void FunctionModifierNode::PrintDialectNode(opDialectStream& stream)
{
	stream.PrintComment("//functionmodifier " + Name->GetValue());
	stream << endl;

	if(opParameters::Get().Highlighting)
	{
		stream << "#if 0" << endl;
		stream << "#define " << Name << " const" << endl;
		stream << "#endif" << endl;
	}
}

void CategoryLocationNode::PrintDialectNode(opDialectStream& stream)
{
	stream.PrintComment("//location " + Name->GetStringValue());
//	Name->PrintNode(stream);
	stream << endl;

	if(opParameters::Get().Ghosts)
	{
		stream << "namespace ";
		Name->PrintDialectNode(stream);
		stream << endl;
		stream << '{' << endl;
		stream.Indent();
	}

	Body->PrintDialectNode(stream);

	if(opParameters::Get().Ghosts)
	{
		stream.DeIndent();
		stream << endl;
		stream << '}' << endl;
	}
}

void NoteNode::PrintDialectNode(opDialectStream& stream)
{
	stream.PrintComment("//note " + Name->GetValue());
	stream << endl;
}

void NoteNode::PrintString(opString& s)
{
	s += "note ";

	Name->PrintString(s);
}

void CategoryMapNode::PrintDialectNode(opDialectStream& stream)
{
	stream.PrintComment("//map " + Name->GetValue());
	stream << endl;
	if(opParameters::Get().Ghosts)
	{
		stream << "namespace " << Name << endl;
		stream << '{' << endl;
		stream.Indent();
	}
	
// 	stream << "inline note start() {}" << endl;
// 	stream << "inline note mapping() {}" << endl;
// 	stream << "inline note end() {}" << endl;
	
	if(Body)
		Body->PrintDialectNode(stream);
	
	if(opParameters::Get().Ghosts)
	{
		stream.DeIndent();
		stream << endl;
		stream << '}' << endl;
	}
}

//
// ExtensionPiontNode
//

void ExtensionPointNode::PrintDialectNode(opDialectStream& stream)
{
	PrintOriginal(stream.header.heading);
}

void ExtensionPointNode::PrintOriginal(opSectionStream& stream)
{
	ExtensionNode* extension = DialectTracker::GetExtension( Name->GetValue() );

	extension->PrintExtendPoints(stream);
}

//
// DialectNamespaceNode
//

void DialectNamespaceNode::PrintDialectNode(opDialectStream& stream)
{
	PrintOriginal(stream.header.heading);
}

void DialectNamespaceNode::PrintOriginal(opSectionStream& stream)
{
	stream << "namespace ";

	Name->PrintOriginal(stream);

	stream << endl;

	Body->PrintOriginal(stream);

	stream << endl;
}

//
// NoteArgumentListNode
//

void NoteArgumentListNode::PrintString(opString& s)
{
	s += '(';
	
	iterator end = GetEnd();
	for(iterator it = GetBegin(); it != end; ++it)
	{
		it->PrintString(s);
		
		iterator next = it;
		++next;
		
		if(next != end)
			s += ',';
	}

	s += ')';
}

void NoteArgumentNode::PrintString(opString& s)
{
	PrintStringChildren(s);	
}

//
// DataModifierNode
//

void DataModifierNode::PrintString(opString& s)
{
	s += "datamodifier ";
	Name->PrintString(s);

	if (Arguments)
		Arguments->PrintString(s);
}

///
/// FunctionModifierNode
///

void FunctionModifierNode::PrintString(opString& s)
{
	s += "functionmodifier ";
	Name->PrintString(s);

	if (Arguments)
		Arguments->PrintString(s);
}

//
// ModifierArgumentNode
//

void ModifierArgumentNode::PrintString(opString& s)
{
	s += "(";
	PrintStringChildren(s);
	s += ")";
}

//
// CriteriaArgumentNode
//


void CriteriaArgumentNode::PrintString(opString& s)
{
	s += "(";
	if(Argument)
		s += Argument->GetValue();
	s += ")";
}

//
// CriteriaValueModifierNode
//

void CriteriaValueModifierNode::PrintString(opString& s)
{
	Name->PrintString(s);
	Argument->PrintString(s);
}


///
/// Conditional Printing
///

int PreprocessorStatementNode::PrintCondition(opSectionStream& stream)
{
	int num = 0;
	
	if(Previous != NULL)
		num = Previous->PrintCondition(stream);

	//now print my condition (after all previous conditions)
	//if I'm an if, add to num
	PreprocessorNode* node = GetPreprocessor();
	
	node->PrintOriginal(stream);
	stream << endl;
	
	if(node_cast<PoundIfNode>(node)
	|| node_cast<PoundIfndefNode>(node)
	|| node_cast<PoundIfdefNode>(node))
		return num + 1;
	
	return num;
}

void DoubleQuoteOperatorNode::PrintOriginal(opSectionStream& stream)
{
	//stringize and print
	opString s;
	PrintStringChildren(s);

	stream << '"' << s << '"';
}

void SingleQuoteOperatorNode::PrintOriginal(opSectionStream& stream)
{
	//stringize and print
	opString s;
	PrintStringChildren(s);

	stream << "'" << s << "'";
}


void ConcatenationOperatorNode::PrintOriginal(opSectionStream& stream )
{
	//it just doesn't print a space in between
	Left->PrintOriginal(stream);
	Right->PrintOriginal(stream);
}

void ConcatenationOperatorNode::PrintString(opString& s)
{

}


