// The Project Dialect

///==========================================
/// support code
///==========================================

#ifndef NULL
	#define NULL 0
#endif

// Here we extract all the initialization code into
// a namespace separate from the class.  This increases
// modularity and and reduces code bloat in object
// instances.
namespace defaults
{
	/*=== specialize, meta-program for pointers ===*/

	template<class T>
	struct PointerUtil
	{
		static void Run(T* t)
		{
			//do nothing, its an unknown type			
		}
	};

	// it is a pointer, set it to NULL
	template<class T>
	struct PointerUtil<T*>
	{
		static void Run(T** t)
		{
			*t = NULL;
		}		
	};

	// base template method for initializing data
	template<class T>
	void SetToDefault(T* t)
	{
		PointerUtil<T>::Run(t);
	}

	/*=== specialize for a few basic types (should do them all) ===*/

	// int
	template<>
	void SetToDefault<int>(int* t)
	{
		*t = 0;
	}

	// float
	template<>
	void SetToDefault<float>(float* t)
	{
		*t = 0.0f;
	}

	// double
	template<>
	void SetToDefault<double>(double* t)
	{
		*t = 0.0;
	}

	// bool
	template<>
	void SetToDefault<bool>(bool* t)
	{
		*t = false;
	}
}

///==========================================
/// fish 
///==========================================

// Here we make a simple category for representing fish.
// The point of this category is to show how we can automatically
// set data members to defaults via template specialization using
// the defaults namespace above.
category fish
{
	location body
	{
		// Function prototype for the Defaults() method.
		note defaultsprototype;

		// Let's declare a constructor that calls the Defaults() method.
		note constructor;
	}

	location footer
	{
		// This is the actual map that initializes the data.
		// It's implementation is actually in the defaults
		// namespace above.
		datamap defaultsmap
		{
		}
		
		// Actual implementation of the Defaults() method.
		note defaultsdefinition;
	}
};

/*=== defaultsprototype/defaultsdefinition note code ===*/

// This note sets up the function prototype for
// the Defaults() method.  We have to declare the
// defaults::SetToDefault<>() template method as
// a friend so that it can initialize a fish's
// data members directly.
note fish::body::defaultsprototype(class_name)
{
public:
	void Defaults();

	friend void defaults::SetToDefault<class_name>(class_name*);
}

// Here we define the actual Defaults() method.
note fish::footer::defaultsdefinition(class_name)
{
	inline void class_name::Defaults()
	{
		// Call the SetToDefault() template method 
		// in the defaults namespace above to initialize
		// all of this's members to their defaults.
		defaults::SetToDefault(this);
	}
}

/*=== constructor note code ===*/

// Here we write a note to generate a default constructor.
// We do this by passing in the class_name automatic modifier.
// Then notice that we call the Defaults() method in the 
// constructor.
note fish::body::constructor(class_name)
{
// Make the constructor public.
public:

	class_name()
	{
		// Call the defaults method.
		Defaults();
	}
}

/*=== defaultsmap note code ===*/

// In the start of the map, we setup the template
// function definition and pass ourself in.  Notice
// we're actually generating a specialization of the
// SetToDefault<>() template method for fish objects.
// By doing this, all fish know how to set their
// own defaults.  This also means fish can have other
// fish as data members and all the defaults are set
// recursively, yet statically.  The object type we're
// passing is is named 'o'.
note fish::footer::defaultsmap::start(class_name)
{
	namespace defaults
	+{}
		template<>
		void SetToDefault<class_name>(class_name* o)
		+{}
}

// Here, we pass each of o's members to the template
// SetToDefault<>() method in the defaults namespace.
// Remember, 'o' came from the start of the defaultsmap
// above.
note fish::footer::defaultsmap::mapping(member_name)
{
	{
		SetToDefault(&o->member_name);
	}
}

// End the template definition.
note fish::footer::defaultsmap::end(class_name)
{
		-{}
	-{}
}
