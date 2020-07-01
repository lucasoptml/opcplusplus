///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Inlines.h
/// Date: 11/26/2006
///
/// Description:
///
/// This header contains various inline methods.
///****************************************************************

//printing for opNode
inline opStringStream& operator << (opStringStream &oin, TerminalNode* node)
{
	Token id = node->GetId();

	// handle compact mode and force comments
	if ( (id == T_COMMENT || id == T_CCOMMENT) 
	&&   opParameters::Get().Compact 
	&&   !opStringStream::GetForceComments() )
	{
		return oin;
	}

	oin.NoteLineNumber(node->GetFile(), node->GetLine());
	oin << node->GetValue();

	return oin;
}

template<>
inline FileWriteStream& operator << <endlinestruct> (FileWriteStream& stream, const endlinestruct&)
{
	stream.Write('\n');

	return stream;
}