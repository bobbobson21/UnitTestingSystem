// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

//#include "utsNew/UnitTestingSystem.h"
#include "UnitTestingSystem.h"

void ExampleTest(uts::UTSTestSeverityCode* outCode, char** OutDescription, void* inArgs)
{
	(*outCode) = uts::UTSTestSeverityCode::TSCFail;
	(*OutDescription) = new char[8] {"you gay"};
}

void ExampleTestB(uts::UTSTestSeverityCode* outCode, char** OutDescription, void* inArgs)
{
	(*outCode) = uts::UTSTestSeverityCode::TSCPass;
}

void ExampleTestC(uts::UTSTestSeverityCode* outCode, char** OutDescription, void* inArgs)
{
	(*outCode) = uts::UTSTestSeverityCode::TSCPass;
}

void ExampleTestD(uts::UTSTestSeverityCode* outCode, char** OutDescription, void* inArgs)
{
	(*outCode) = uts::UTSTestSeverityCode::TSCWarning;
	(*OutDescription) = new char[40] {"This is an example of a description lol"};
}

int main()
{
	uts::UTSListConstructor mainTestTree = uts::UTSListConstructor();

	mainTestTree.AddTest("ExampleTest", ExampleTest);
	mainTestTree.AddTest("ExampleTestB", ExampleTestB);
	mainTestTree.AddNotice("running C block");
	mainTestTree.AddTest("ExampleTestC", ExampleTestC);
	mainTestTree.AddTest("ExampleTestC", ExampleTestC);
	mainTestTree.AddTest("ExampleTestC", ExampleTestC);
	mainTestTree.AddTest("ExampleTestC", ExampleTestC);
	mainTestTree.AddNotice("running D block");
	mainTestTree.AddTest("ExampleTestD", ExampleTestD);
	mainTestTree.AddTest("ExampleTestD", ExampleTestD);
	mainTestTree.AddTest("ExampleTestD", ExampleTestD);
	mainTestTree.AddTest("ExampleTestD", ExampleTestD);

	/*mainTestTree.PushDomain("namespace", "testing");
	mainTestTree.PushDomain("class", "tester");

	mainTestTree.PushDomain("static");
	mainTestTree.PushPopTest("ExampleTest", ExampleTest);
	mainTestTree.PushPopNotice("running C block");
	mainTestTree.PushPopTest("ExampleTestC", ExampleTestC);
	mainTestTree.PushPopTest("ExampleTestC", ExampleTestC);
	mainTestTree.PushPopTest("ExampleTestC", ExampleTestC);
	mainTestTree.PushPopTest("ExampleTestC", ExampleTestC);
	mainTestTree.PopDomain();

	mainTestTree.PushDomain("instance");
	mainTestTree.PushPopTest("ExampleTestB", ExampleTestB);
	mainTestTree.PushPopNotice("running D block");
	mainTestTree.PushPopTest("ExampleTestD", ExampleTestD);
	mainTestTree.PushPopTest("ExampleTestD", ExampleTestD);
	mainTestTree.PushPopTest("ExampleTestD", ExampleTestD);
	mainTestTree.PushPopTest("ExampleTestD", ExampleTestD);
	mainTestTree.PopDomain();

	mainTestTree.PopDomain();
	mainTestTree.PopDomain();*/

	mainTestTree.RunTests(nullptr);

	uts::ConOutputTestResults(mainTestTree.GetContainer(), true, true);

    std::cout << "Hello World!\n";
}
 
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
