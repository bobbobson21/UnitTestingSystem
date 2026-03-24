// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "utsNew/UnitTestingSystem.h"
//#include "UnitTestingSystem.h"

uts::UTSUnitTestResults ExampleTest(void* inArgs)
{
	uts::UTSUnitTestResults returnData = uts::UTSUnitTestResults();
	returnData.m_testResultSeverityCode = uts::UTSUnitTestSeverityCode::TSCFail;

	return returnData;
}

uts::UTSUnitTestResults ExampleTestB(void* inArgs)
{
	uts::UTSUnitTestResults returnData = uts::UTSUnitTestResults();
	returnData.m_testResultSeverityCode = uts::UTSUnitTestSeverityCode::TSCPass;

	returnData.WriteStringSafe(&returnData.m_testResultDescriptionBuffer, "you gay");
	returnData.WriteStringSafe(&returnData.m_objectFileLocationRelative, "Test\\\\Test.cpp");
	returnData.WriteStringSafe(&returnData.m_unitTestfileLocationRelative, "Test\\\\Test.cpp");

	return returnData;
}

uts::UTSUnitTestResults ExampleTestC(void* inArgs)
{
	uts::UTSUnitTestResults returnData = uts::UTSUnitTestResults();
	returnData.m_testResultSeverityCode = uts::UTSUnitTestSeverityCode::TSCPass;

	return returnData;
}

uts::UTSUnitTestResults ExampleTestD(void* inArgs)
{
	uts::UTSUnitTestResults returnData = uts::UTSUnitTestResults();
	returnData.m_testResultSeverityCode = uts::UTSUnitTestSeverityCode::TSCPass;

	return returnData;
}

int main()
{
	uts::UTSTreeConstructor mainTestTree = uts::UTSTreeConstructor();

	mainTestTree.PushDomain("namespace", "testing");
	mainTestTree.PushDomain("class", "tester");

	mainTestTree.PushDomain("static");
	mainTestTree.PushPopTest("ExampleTest", ExampleTest);
	mainTestTree.PushPopNotice("running C block");
	mainTestTree.PushPopTest("ExampleTestC", ExampleTestC);
	mainTestTree.PushPopTest("ExampleTestC_1", ExampleTestC);
	mainTestTree.PushPopTest("ExampleTestC_2", ExampleTestC);
	mainTestTree.PushPopTest("ExampleTestC_3", ExampleTestC);
	mainTestTree.PopDomain();

	mainTestTree.PushDomain("instance");
	mainTestTree.PushPopTest("ExampleTestB", ExampleTestB);
	mainTestTree.PushPopNotice("running D block");
	mainTestTree.PushPopTest("ExampleTestD", ExampleTestD);
	mainTestTree.PushPopTest("ExampleTestD_1", ExampleTestD);
	mainTestTree.PushPopTest("ExampleTestD_2", ExampleTestD);
	mainTestTree.PushPopTest("ExampleTestD_3", ExampleTestD);
	mainTestTree.PopDomain();

	mainTestTree.PopDomain();
	mainTestTree.PopDomain();

	mainTestTree.RunTests(nullptr);
	uts::ConOutputTestResults(mainTestTree.GetContainer(), uts::ConOutputSettings{true, true});
	uts::ExtOutputTestResults(mainTestTree.GetContainer(), uts::ExtOutputSettings{true});

	/*uts::UTSListConstructor mainTestList = uts::UTSListConstructor();

	mainTestList.AddTest("ExampleTest", ExampleTest);
	mainTestList.AddTest("ExampleTestB", ExampleTestB);
	mainTestList.AddNotice("running C block");
	mainTestList.AddTest("ExampleTestC", ExampleTestC);
	mainTestList.AddTest("ExampleTestC", ExampleTestC);
	mainTestList.AddTest("ExampleTestC", ExampleTestC);
	mainTestList.AddTest("ExampleTestC", ExampleTestC);
	mainTestList.AddNotice("running D block");
	mainTestList.AddTest("ExampleTestD", ExampleTestD);
	mainTestList.AddTest("ExampleTestD", ExampleTestD);
	mainTestList.AddTest("ExampleTestD", ExampleTestD);
	mainTestList.AddTest("ExampleTestD", ExampleTestD);
	
	mainTestList.RunTests(nullptr);
	uts::ConOutputTestResults(mainTestList.GetContainer(), true, false);*/

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
