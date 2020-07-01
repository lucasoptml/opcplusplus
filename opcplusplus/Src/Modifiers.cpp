///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Modifiers.cpp
/// Date: 10/10/2007
///
/// Description:
///
/// Modifier Listing Source
///****************************************************************

DataModifierSet			DataModifierSet::Instance;
FunctionModifierSet		FunctionModifierSet::Instance;
CategoryModifierSet		CategoryModifierSet::Instance;
EnumModifierSet			EnumModifierSet::Instance;
StatementModifierSet	StatementModifierSet::Instance;
ModifierDescriptions	ModifierDescriptions::Instance;

DataModifierSet::DataModifierSet()
{
	//add all basic modifiers
	BasicModifiers.Insert("data_statement");
	BasicModifiers.Insert("data_reference");
	BasicModifiers.Insert("data_pointer");
	BasicModifiers.Insert("data_template");
	BasicModifiers.Insert("data_qualified");
	BasicModifiers.Insert("data_basic");
	BasicModifiers.Insert("data_function_pointer");
	BasicModifiers.Insert("data_array");
	BasicModifiers.Insert("const");
	BasicModifiers.Insert("volatile");
	BasicModifiers.Insert("static");
	BasicModifiers.Insert("mutable");
	
	//add all value modifiers
	ValueModifiers.Insert("data_type");
	ValueModifiers.Insert("data_array_commas");
	ValueModifiers.Insert("data_array_brackets");
	ValueModifiers.Insert("data_base_type");
	ValueModifiers.Insert("data_full_type");
	ValueModifiers.Insert("data_bits");
	ValueModifiers.Insert("data_initialized");
	ValueModifiers.Insert("data_pointer_level");
}

FunctionModifierSet::FunctionModifierSet()
{
	//add all basic modifiers
	BasicModifiers.Insert("function_statement");
	BasicModifiers.Insert("function_constructor");
	BasicModifiers.Insert("function_destructor");
	BasicModifiers.Insert("function_pure");
	BasicModifiers.Insert("function_prototype");
	BasicModifiers.Insert("function_definition");
	BasicModifiers.Insert("function_operator");
	BasicModifiers.Insert("function_const");
	BasicModifiers.Insert("virtual");
	BasicModifiers.Insert("const");
	BasicModifiers.Insert("volatile");
	BasicModifiers.Insert("explicit");
	BasicModifiers.Insert("uninline");
	BasicModifiers.Insert("inline");
	BasicModifiers.Insert("static");
	
	//add all value modifiers
	ValueModifiers.Insert("function_return_type");
	ValueModifiers.Insert("function_arguments");
	ValueModifiers.Insert("function_body");
	ValueModifiers.Insert("function_signature");
	ValueModifiers.Insert("function_operator_type");
	ValueModifiers.Insert("function_arguments_defaults");
	ValueModifiers.Insert("function_arguments_count");
	ValueModifiers.Insert("function_arguments_signature");
}

StatementModifierSet::StatementModifierSet()
{
	// basic	
	BasicModifiers.Insert("public");
	BasicModifiers.Insert("private");
	BasicModifiers.Insert("protected");
	BasicModifiers.Insert("default");

	// valued
	ValueModifiers.Insert("member_name");
	ValueModifiers.Insert("all_modifiers");
	ValueModifiers.Insert("visibility_mode");
}

CategoryModifierSet::CategoryModifierSet()
{
	//add all basic modifiers
	BasicModifiers.Insert("category_abstract");
	BasicModifiers.Insert("category_baseclass");
	BasicModifiers.Insert("category_single_inheritance");
	BasicModifiers.Insert("category_multiple_inheritance");
	BasicModifiers.Insert("category_has_constructor");
	BasicModifiers.Insert("category_has_destructor");
	BasicModifiers.Insert("category_has_simple_constructor");
	BasicModifiers.Insert("category_has_complex_constructor");
	BasicModifiers.Insert("inner_type");
	
	//add all value modifiers
	ValueModifiers.Insert("scope");
	ValueModifiers.Insert("class_name");
	ValueModifiers.Insert("parent_name");
}

EnumModifierSet::EnumModifierSet()
{
	// basic
	BasicModifiers.Insert("inner_type");

	// valued
	ValueModifiers.Insert("scope");
	ValueModifiers.Insert("enum_type");
}

//
// Descriptions
//

ModifierDescriptions::ModifierDescriptions()
{
	/*=== data ===*/

	// basic
	AddBasicDescription("data_statement",        "Specifies that the statement is a data statement.");
	AddBasicDescription("data_reference",        "Specifies that the data type is a reference.");
	AddBasicDescription("data_pointer",          "Specifies that the data type is a pointer.");
	AddBasicDescription("data_template",         "Specifies that the data type is a template type.");
	AddBasicDescription("data_qualified",        "Specifies that the data type contains a qualified name.");
	AddBasicDescription("data_basic",            "Specifies that the data type is a basic type. (e.g., int, double).");
	AddBasicDescription("data_function_pointer", "Specifies that the data type is a function pointer.");
	AddBasicDescription("data_array",            "Specifies that the data type is an array.");

	// valued
	AddValuedDescription("data_type",           "Modifier value that contains the data type.");
	AddValuedDescription("data_array_commas",   "Modifier value that contains the dimensions of an array, separated by commas (e.g., A[3][4] -> 3,4).");
	AddValuedDescription("data_array_brackets", "Modifier value that contains the brackets from the array declaration (e.g., A[3][4] -> [3][4]).");
	AddValuedDescription("data_base_type",      "Modifier value that contains the base data type (e.g., A* -> A).");
	AddValuedDescription("data_full_type",      "Modifier value that contains the data type, including array brackets (e.g., int A[10] -> int[10]).");
	AddValuedDescription("data_bits",           "Modifier value that contains the number of bits in a bit specifier (e.g., int A:3; -> 3).");
	AddValuedDescription("data_initialized",    "Modifier value that contains the expression on the right side of an assignment statement (e.g., int x = 3; -> 3).");
	AddValuedDescription("data_pointer_level",  "Modifier value that contains the number of indirections if the data type is a pointer (e.g., A*** -> 3).");

	/*=== function ===*/

	// basic
	AddBasicDescription("function_statement",   "Specifies that the statement is a function statement.");
	AddBasicDescription("function_constructor", "Specifies that the function is a constructor.");
	AddBasicDescription("function_destructor",  "Specifies that the function is a destructor.");
	AddBasicDescription("function_pure",        "Specifies that the function is a pure function (e.g., virtual void Foo() = 0;).");
	AddBasicDescription("function_prototype",   "Specifies that the function is a function prototype.");
	AddBasicDescription("function_definition",  "Specifies that the function is a function definition.");
	AddBasicDescription("function_operator",    "Specifies that the function is an operator overload.");
	AddBasicDescription("function_const",       "Specifies that the function is a constant function (e.g., void Foo() const).");
	AddBasicDescription("virtual",              "Specifies that the function is virtual.");
	AddBasicDescription("explicit",             "Specifies that the function is explicit.");
	AddBasicDescription("uninline",             "Specifies that the function is uninline (function definition will be moved to source).");
	AddBasicDescription("inline",               "Specifies that the function is inline.");

	// valued
	AddValuedDescription("function_return_type",         "Modifier value that contains the function`s return type.");
	AddValuedDescription("function_arguments",           "Modifier value that contains the function`s argument list (e.g., void Foo(bool b = true) -> bool b).");
	AddValuedDescription("function_body",                "Modifier value that contains the function`s body.");
	AddValuedDescription("function_signature",           "Modifier value that contains the function`s signature.");
	AddValuedDescription("function_operator_type",       "Modifier value that contains an operator overload`s operator type (e.g., bool operator== ( ... ) { ... } -> ==).");
	AddValuedDescription("function_arguments_defaults",  "Modifier value that contains the function`s argument list, including default arguments (e.g., void Foo(bool b = true) -> bool b = true).");
	AddValuedDescription("function_arguments_count",     "Modifier value that contains the number of function arguments.");
	AddValuedDescription("function_arguments_signature", "Modifier value that contains the function`s argument signature (e.g., void Foo(int x, int y) -> (int,int)).");

	/*=== statement ===*/

	// basic	
	AddBasicDescription("public",    "Specifies that the statement has public visibility.");
	AddBasicDescription("private",   "Specifies that the statement has private visibility.");
	AddBasicDescription("protected", "Specifies that the statement has protected visibility.");
	AddBasicDescription("default",   "Specifies that the statement has the default visibility.");
	AddBasicDescription("const",     "Specifies that the statement is const.");
	AddBasicDescription("volatile",  "Specifies that the statement is volatile.");
	AddBasicDescription("static",    "Specifies that the statement is static.");
	AddBasicDescription("mutable",   "Specifies that the statement is mutable.");

	// valued
	AddValuedDescription("member_name",     "Modifier value containing the name of the statement (e.g., void Foo() { ... } -> Foo).");
	AddValuedDescription("all_modifiers",   "Modifier value that contains all modifiers attached to the statement (e.g., private native int x; -> private native).");
	AddValuedDescription("visibility_mode", "Modifier value containing the visibility of the statement (e.g., private int x; -> private).");

	/*=== category ===*/

	// basic
	AddBasicDescription("category_abstract",             "Specifies that the category is abstract (has a pure virtual function).");
	AddBasicDescription("category_baseclass",            "Specifies that the category is a base class (no inheritance).");
	AddBasicDescription("category_single_inheritance",   "Specifies that the category has single inheritance.");
	AddBasicDescription("category_multiple_inheritance", "Specifies that the category has multiple inheritance.");
	AddBasicDescription("category_has_constructor",      "Specifies that the category has a constructor.");
	AddBasicDescription("category_has_destructor",       "Specifies that the category has a destructor.");
	AddBasicDescription("category_has_simple_constructor",   "Specifies that the category has a simple constructor.");
	AddBasicDescription("category_has_complex_constructor",  "Specifies that the category has a complex constructor.");
	AddBasicDescription("inner_type",                    "Specifies that the construct is an inner type of another category.");

	// valued
	AddValuedDescription("scope",       "Modifier value containing the scope of the object (e.g., namespace A { opclass B { ... } } -> A::).");
	AddValuedDescription("class_name",  "Modifier value containing the name of the category.");
	AddValuedDescription("parent_name", "Modifier value containing the name of the category`s parents, separated by commas (e.g., opclass A : public B -> B).");

	/*=== enumeration ===*/

	// valued
	AddValuedDescription("enum_type",  "Modifier value containing the type of the enumeration (e.g., openum Colors { ... } -> Colors).");
	AddValuedDescription("enum_name",  "Modifier value containing the name of an enumeration element (e.g., Red, -> Red).");
	AddValuedDescription("enum_value", "Modifier value containing the expression on the right side of an enumeration element assignment statement (e.g., Red = Blue, -> Blue).");

	/*=== global ===*/

	// arguments
	AddArgumentDescription("namespace_start",     "Argument specifying the start of all surrounding namespaces (e.g., namespace A { namespace B { } } -> namespace A { namespace B {).  Use in conjunction with namespace_end.");
	AddArgumentDescription("namespace_end",       "Argument specifying the end of all surrounding namespaces.");
	AddArgumentDescription("source_line",         "Argument specifying the source line of the category (notes) or the data/function member (maps).");
	AddArgumentDescription("source_file",         "Argument specifying the source file of the category (notes) or the data/function member (maps).");
	AddArgumentDescription("alt_scope",           "Argument specifying the scope of the object prefixed with underscores (e.g., namespace A { opclass B { ... } } -> _A::).");
	AddArgumentDescription("alt_namespace_start", "Argument specifying the start of all surrounding namespaces, prefixed with underscores (e.g., namespace A { namespace B { } } -> namespace _A { namespace _B {).  Use in conjunction with alt_namespace_end.");
	AddArgumentDescription("alt_namespace_end",   "Argument specifying the end of all surrounding namespaces.");
	AddArgumentDescription("alt_class_name",      "Argument specifying the name of the object, prefixed with underscores (e.g., opclass A -> _A).");
	AddArgumentDescription("map_index",           "Argument specifying the index of the current statement in a map.");
	AddArgumentDescription("map_count",           "Argument specifying the number of statements in a map.");
}