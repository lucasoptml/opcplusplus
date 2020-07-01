
// In this example, we use a category to reduce the code duplication.
category myclass
{
	location body
	{
		note getclassname;
		note getclasssize;
		note newop;
		note deleteop;

		functionmap mapa
		{

		}
	}
};

note myclass::body::mapa::mapping()
{

}

enumeration openum
{
	location body
	{
		note blah;

		enummap map;
	}
};

note openum::body::blah()
{

}

note openum::body::map::start()
{

}


note openum::body::map::name()
{

}


note openum::body::map::value()
{

}


note openum::body::map::end()
{

}



/*=== getclassname note code ===*/

note myclass::body::getclassname(class_name)
{
public:

	// Returns the class name.
	const char* GetClassName()				
	{										
		return ``class_name``;				
	}	
}

/*=== getclasssize note code ===*/

note myclass::body::getclasssize(class_name)
{
public:

	// Returns the size of class_name.
	size_t GetClassSize()					
	{										
		return sizeof(class_name);			
	}		
}

/*=== newop note code ===*/

note myclass::body::newop()
{
private:

	// Make new and delete private so you can't  
	// dynamically allocate an instance of class_name.
	static void* operator new(size_t size)		
	{											
		return NULL;							
	}		
}

/*=== deleteop note code ===*/

note myclass::body::deleteop()
{
private:

	// Make new and delete private so you can't  
	// dynamically allocate an instance of class_name.
	static void operator delete(void* object)	
	{											

	}		
}

/*=== notes ===*/

// Pros
// 1) categories are fully debuggable!
// 2) We have reduced code duplication and redundancy.
// 3) We can put comments into the note definitions.
// 4) It actually looks readable.
// 5) After you write the dialect, everything is automatic.
//    You never have to add code if you instantiate a myclass 
//    instance.										

