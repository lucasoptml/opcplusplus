--------------------------------------
 Current opC++ Benchmarks
--------------------------------------

These tests & benchmarks currently test opC++ versus:
 * C# (.NET 2.0)
 * Boost Libraries
 * Itself
 
It may take some work to get the Boost stuff
to compile - due to all the awful library dependencies.
 
Most importantly these benchmarks attempt to show
how differences in approach - dynamic language vs compile time,
vs a hybrid approach play out.

----------------
The Benchmarks:
----------------

Reflection : attempt to grab field names a many times.
 
 C# - utilize the standard reflection facility,
      but also attempt to cache it.
	- utilize a cached reflection emit approach.
	
 opC++ - utilize the standard dialect without
		 any caching or funny business.

 This test puts C# at a fundamental advantage by caching its
 fields - which is unlikely to be common practice.
 
 
Serialize : attempt to save and load objects from streams in memory.
 
 C# - read and write using BinaryFormatter.
    - read and write using XmlReader/Writer.
 
 Boost/C++ - read and write using binary.
           - read and write using xml.
		 
 opC++ - read and write using binary.
         read and write using Xml.
		 

ClassCast : benchmark the speed of dynamic casts

 C# - use as style casting (fast)
    - use c-style casting (slow)

 C++ - use dynamic_cast
 
 opC++ - use class_cast
 
C++ dynamic_cast is not constant speed - it has weak performance
even to start with.

C# as-style casts appear to be constant speed (?)

opC++ casts use a range-checking method - constant time and very fast.


Compiling : verifying compiling times for different C++ constructs.

It appears namespaced constructs are always a good idea, and can speed
up compile times, more than mangled names - this only becomes visible with
large amounts of generated code.
