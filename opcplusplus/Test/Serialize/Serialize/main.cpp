
#include <stack>

#include "main.h"

#include <fstream>

using namespace std;


// Entry point into the application.
int main()
{

	//example: text save/load
	cout << "saving test.txt" << endl;

	TestClass savetest;
	savetest.Initialize();

	{
		//test saving
		std::ofstream savefile("test.txt");

		opcpp::visitors::TextSaveVisitor savevisitor(savefile,&savetest);

		//savetest.visit_data_members(savevisitor);
	}
	
	cout << "loading test.txt" << endl;
	
	TestClass loadtest;

	{
		//test loading
		std::ifstream loadfile("test.txt");
		
		opcpp::visitors::TextLoadVisitor loadvisitor(loadfile,&loadtest);
		
		//loadtest.visit_data_members(loadvisitor);
	}
	
	// example: binary save/load
	cout << "saving test.bin" << endl;
	
	TestClass binsavetest;
	binsavetest.Initialize();
	
	{
		//test saving
		std::ofstream binsavefile("test.bin", ios::out | ios::binary);
		
		opcpp::visitors::BinarySaveVisitor savevisitor(binsavefile,&binsavetest);
		
		//savetest.visit_data_members(savevisitor);
	}
	
	cout << "loading test.bin" << endl;
	
	TestClass binloadtest;
	
	{
		//test loading
		std::ifstream binloadfile("test.bin");

		opcpp::visitors::BinaryLoadVisitor loadvisitor(binloadfile,&binloadtest);
	}


	// example: clone_object via reflection.
	TestClass* clone = opcpp::functions::clone_object(&binsavetest);


	
	return 0;
}


// Include the generated source index.
// NOTE: Keep the project name and this path in sync.
#include "generated/Serialize/generated.ocppindex"


