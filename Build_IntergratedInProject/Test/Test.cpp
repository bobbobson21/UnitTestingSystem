// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "lib/UnitTestingSystem.h"

bool ExampleTest()
{
	return true;
}

bool ExampleTestB()
{
	return true;
}

bool ExampleTestC()
{
	return true;
}

int main()
{
	uts::UTSTreeConstructor mainTestTree = uts::UTSTreeConstructor();

	mainTestTree.PushDomain("namespace", "testing");
	mainTestTree.PushDomain("class", "tester");

	mainTestTree.PushDomain("static");
	mainTestTree.PushTest("ExampleTest", ExampleTest);
	mainTestTree.PushTest("ExampleTestC", ExampleTestC);
	mainTestTree.PushTest("ExampleTestC", ExampleTestC);
	mainTestTree.PushTest("ExampleTestC", ExampleTestC);
	mainTestTree.PushTest("ExampleTestC", ExampleTestC);
	mainTestTree.PopDomain();

	mainTestTree.PushDomain("instance");
	mainTestTree.PushTest("ExampleTestB", ExampleTestB);
	mainTestTree.PushTest("ExampleTestC", ExampleTestC);
	mainTestTree.PushTest("ExampleTestC", ExampleTestC);
	mainTestTree.PushTest("ExampleTestC", ExampleTestC);
	mainTestTree.PushTest("ExampleTestC", ExampleTestC);
	mainTestTree.PopDomain();

	mainTestTree.PopDomain();
	mainTestTree.PopDomain();

	uts::UTSTree results = mainTestTree.RunTests();

	uts::ConOutputTestResults(&results);

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
