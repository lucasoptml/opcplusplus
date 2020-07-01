#include "main.h"

#include <iostream>
#include <sstream>
#include <fstream>

using std::ofstream;
using std::cout;

using opcpp::visitors::xml_save_archiver;
using opcpp::visitors::text_save_archiver;
using opcpp::visitors::binary_save_archiver;
using opcpp::visitors::xml_load_archiver;
using opcpp::visitors::text_load_archiver;
using opcpp::visitors::binary_load_archiver;


using std::stringstream;

// Entry point into the application.
int main()
{
	bool bErrors = false;
	
	stringstream bin_stream;

	testmatrixclass bin_save_object;
	bin_save_object.Initialize();
	{
		//save binary
		binary_save_archiver saver(bin_stream);
		saver << bin_save_object;
	}

	testmatrixclass bin_load_object;
	{
		//load binary
		binary_load_archiver loader(bin_stream);
		loader >> bin_load_object;
	}
	
	if(bin_save_object != bin_load_object)
	{
		cout << "bin load and save do not match" << endl;
		 bErrors = true;
	}
	else
		cout << "bin load and save matched!" << endl;
	
	stringstream text_stream;
	
	testmatrixclass text_save_object;
	text_save_object.Initialize();
	{
		//save text
		text_save_archiver saver(text_stream);
		saver << text_save_object;
	}	
	
	testmatrixclass text_load_object;
	{
		//load text
		text_load_archiver loader(text_stream);
		loader >> text_load_object;
	}

	if(text_save_object != text_load_object)
	{
		cout << "text load and save do not match" << endl;
		bErrors = true;
	}
	else
		cout << "text load and save matched!" << endl;
	
	stringstream xml_stream;
	
	testmatrixclass xml_save_object;
	xml_save_object.Initialize();
	{
		//save xml
		xml_save_archiver saver(xml_stream, "xmltest");
		saver.save(xml_save_object);

		ofstream file("test.xml");
		xml_save_archiver filesaver(file,"xmltest");
		filesaver.save(xml_save_object);
	}
	
	testmatrixclass xml_load_object;
	{
		//load xml
		xml_load_archiver loader(xml_stream, "xmltest");
		loader.load(xml_load_object);
	}
	
	if(xml_save_object != xml_load_object)
	{
		cout << "xml load and save do not match" << endl;
		bErrors = true;
	}
	else
		cout << "xml load and save matched!" << endl;
	
	//
	// Compare all
	//

	if(!bErrors)
	{
		cout << "all archivers succeeded" << endl;
	}


	return 0;
}

