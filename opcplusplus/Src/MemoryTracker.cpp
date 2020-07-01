///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: MemoryTracker.cpp
/// Date: 01/05/2007
///
/// Description:
///
/// Memory Tracker Source
///****************************************************************

#include "opCPP.h"

#ifdef _DEBUG

//memory tracker related from opnode
int opNode::StaticAllocationCounter = 0;

opMemoryTracker* opMemoryTracker::instance;

void opMemoryTracker::Register(opNode* newnode)
{
	instance->TrackedNodes.Insert(newnode);
}

void opMemoryTracker::UnRegister(opNode* newnode)
{
	//NOTE: this is causing the debug shutdown to be very slow.
	//      perhaps should use a set instead...
	instance->TrackedNodes.Erase(newnode);
}

opMemoryTracker::opMemoryTracker()
{
	instance = this;
}

opMemoryTracker::~opMemoryTracker()
{
	//unregister all reachable nodes
	FileNode::UnRegisterLoadedFiles();

	//walk through the hash set.
	//print all nodes remaining as leaks
	opSet<opNode*>::iterator begin = TrackedNodes.Begin();
	opSet<opNode*>::iterator end   = TrackedNodes.End();
	
	for (opSet<opNode*>::iterator it = begin; it != end; ++it)
	{
		opNode* node = (*it);
		opString classname = node->GetNodeType();
		opString location  = node->GetAllocationLocation();
		
		//we want to debug before it's created
		int number = node->GetAllocationNumber();
		
		opNode*  parent      = node->GetParent();
		opString parentvalid = parent ? TrackedNodes.Contains(parent) ? "parent exists" : "parent deleted!" : "parent NULL";
		
		Log(opString(location + " : error : leaked \"") + classname + "\", debug before allocation with DebugAllocationNumber(" + number + "); " + parentvalid);
		
		if(node->GetFile())
		{
			opString tokenlocation = node->GetFile()->GetInputName() + "(" + node->GetLine() + ")";
			Log(opString(tokenlocation + " : original location"));
		}
	}
	
	TrackedNodes.Clear();
	
	//try to cleanup memory first
	FileNode::DeleteLoadedFiles();
	

	//TODO: need a function for this in a wrapper :P opLog(opString("final number of buckets: ") + hash._Maxidx)
}

#else

opMemoryTracker::~opMemoryTracker()
{
	//cleanup stuff
	FileNode::DeleteLoadedFiles();
}

#endif

