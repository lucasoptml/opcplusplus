///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: MemoryTracker.h
/// Date: 01/05/2007
///
/// Description:
///
/// Debug Memory Tracker
///****************************************************************

using exceptions::opException;

//hash pointer functor
template<>
struct opHashFunction<opNode*>
{
	static size_t Hash(const opNode* const t)
	{
		return (size_t) t;
	}
};

namespace memory
{

#ifdef _DEBUG

class opMemoryTracker
{
public:
	opMemoryTracker();
	~opMemoryTracker();

	//TODO: anything we create on the heap with new we should add to this
	//		tracker - they don't need to necessarily have a NEWNODE style wrapper,
	//		we just want to be made aware of them..
	static void Register(opNode* newnode);
	static void UnRegister(opNode* newnode);
	
private:
	static opMemoryTracker* instance;
	
	opSet<opNode*> TrackedNodes;

	//TODO: add more types
};

#else

class opMemoryTracker
{
public:
	opMemoryTracker() {}
	~opMemoryTracker();

	static void Register(opNode* newnode) {}
	static void UnRegister(opNode* newnode) {}
};

#endif

template<class T>
class stackedcontext;


// smart pointer for parse nodes (opnodes)
// can copy etc safely, it deletes the node upon exceptions only
// because of this - only want one stacked<> pointing to a node at a time
// 
template<class T>
class stacked
{
protected:
	/**** construction / destruction ****/

	explicit stacked<T>(T* pointer)
		: node(pointer)
	{}

	friend opNode;
	friend opNodeBase;
	friend NewHandler;

public:
	
	// default constructor
	stacked<T>()
		: node(NULL)
	{

	}

	//copy constructor
	stacked<T>(const stacked<T>& pointer)
		: node(pointer.node)
	{}

	~stacked<T>() 
	{
		//int count = opException::GetExceptionCount();
		if(node)
		{
			if (opException::InException())
				delete node;
		}
	}

	void Delete()
	{
#ifdef _DEBUG
		assert(node);		
#endif
		delete node;
		node = NULL;
	}

	void Connected()
	{
		node = NULL;
	}

	T* operator->()
	{
#ifdef _DEBUG
		assert(node);//must be valid
#endif
		return node;
	}

	T* operator*()
	{
#ifdef _DEBUG
		assert(node);//must be valid
#endif
		return node;
}

	//the only public way to build stacked
	static stacked<T> buildstacked(T* node)
	{
		return stacked<T>(node);
	}

	T* GetNode() const
	{
		return node;
	}

	bool IsValid() const
	{
		return node != NULL;
	}

protected:
	T* node;

//make stackedcontext<T> conversions error
private:
	stacked<T>(const stackedcontext<T>& pointer)
		: node(pointer.GetNode())
	{
	}
};






//
// New Redirection Handling
//

class NewHandler
{
public:

	template<class T>
	static inline stacked<T> NewNode(T* newnode, const opString& file, int linenumber)
	{
#ifdef _DEBUG
		newnode->SetAllocationLocation(file + "(" + linenumber + ")");
#endif
		newnode->PostInit();
		return stacked<T>(newnode);
	}

};

//
// Clone Utility
//

template<class T>
inline stacked<T> CloneProcess(T* clonenode)
{
	stacked<T> newNode = NEWNODE( T );

	clonenode->CloneBasics(*newNode);
	clonenode->CloneNode(*newNode);

	return newNode;
}

class stackedgroup
{
public:
	~stackedgroup()
	{
		nodes.DeleteAll();
	}

	template<class T>
	void Push(stacked<T>& node)
	{
		nodes.PushBack(*node);
		node.Connected();
	}

private:
	opArray<opNode*> nodes;
};


} // end of namespace memory
