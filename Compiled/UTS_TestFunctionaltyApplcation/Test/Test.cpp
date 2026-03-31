// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "rtuts/RunTimeUnitTestingSystem.h"
//#include "UnitTestingSystem.h"

rtuts::RTUTSUnitTestResults ExampleTest(void* inArgs)
{
	rtuts::RTUTSUnitTestResults returnData = rtuts::RTUTSUnitTestResults();
	returnData.m_testResultSeverityCode = rtuts::RTUTSUnitTestSeverityCode::TSCPass;

	return returnData;
}

rtuts::RTUTSUnitTestResults ExampleTestB(void* inArgs)
{
	rtuts::RTUTSUnitTestResults returnData = rtuts::RTUTSUnitTestResults();
	returnData.m_testResultSeverityCode = rtuts::RTUTSUnitTestSeverityCode::TSCFail;

	rtuts::WriteStringInLibrary(&returnData.m_testResultDescriptionBuffer, "you gay");
	rtuts::WriteStringInLibrary(&returnData.m_objectFileLocationRelative, "Test\\\\Test.cpp");
	rtuts::WriteStringInLibrary(&returnData.m_unitTestfileLocationRelative, "Test\\\\Test.cpp");

	return returnData;
}

rtuts::RTUTSUnitTestResults ExampleTestC(void* inArgs)
{
	rtuts::RTUTSUnitTestResults returnData = rtuts::RTUTSUnitTestResults();
	returnData.m_testResultSeverityCode = rtuts::RTUTSUnitTestSeverityCode::TSCPass;

	return returnData;
}

rtuts::RTUTSUnitTestResults ExampleTestD(void* inArgs)
{
	rtuts::RTUTSUnitTestResults returnData = rtuts::RTUTSUnitTestResults();
	returnData.m_testResultSeverityCode = rtuts::RTUTSUnitTestSeverityCode::TSCPass;

	return returnData;
}

int main()
{
	rtuts::RTUTSTreeConstructor mainTestTree = rtuts::RTUTSTreeConstructor();

	mainTestTree.PushDomain("namespace", "testing");
	mainTestTree.PushDomain("class", "tester");

	mainTestTree.PushDomain("static");
	mainTestTree.PushPopTest("ExampleTest", ExampleTest);
	mainTestTree.PushPopNotice("running C block");
	mainTestTree.PushPopTest("ExampleTestC", ExampleTestC);
	mainTestTree.PushPopTest("ExampleTestC (1)", ExampleTestC);
	mainTestTree.PushPopTest("ExampleTestC (2)", ExampleTestC);
	mainTestTree.PushPopTest("ExampleTestC (3)", ExampleTestC);
	mainTestTree.PopDomain();

	mainTestTree.PushDomain("instance");
	mainTestTree.PushPopTest("ExampleTestB", ExampleTestB);
	mainTestTree.PushPopNotice("running D block");
	mainTestTree.PushPopTest("ExampleTestD", ExampleTestD);
	mainTestTree.PushPopTest("ExampleTestD (1)", ExampleTestD);
	mainTestTree.PushPopTest("ExampleTestD (2)", ExampleTestD);
	mainTestTree.PushPopTest("ExampleTestD (3)", ExampleTestD);
	mainTestTree.PopDomain();

	mainTestTree.PopDomain();
	mainTestTree.PopDomain();

	mainTestTree.RunTests(nullptr);
	rtuts::ConOutputTestResults(mainTestTree.GetContainer(), rtuts::ConOutputSettings{true, true});
	rtuts::ExtOutputTestResults(mainTestTree.GetContainer(), rtuts::ExtOutputSettings{true});

	rtuts::RTUTSBindedListConstructor mainTestList = rtuts::RTUTSBindedListConstructor();

	mainTestList.AddTest("ExampleTest", ExampleTest);
	mainTestList.AddTest("ExampleTestB", ExampleTestB);
	mainTestList.AddNotice("running C block");
	mainTestList.AddTest("ExampleTestC", ExampleTestC);
	mainTestList.AddTest("ExampleTestC (1)", ExampleTestC);
	mainTestList.AddTest("ExampleTestC (2)", ExampleTestC);
	mainTestList.AddTest("ExampleTestC (3)", ExampleTestC);
	mainTestList.AddNotice("running D block");
	mainTestList.AddTest("ExampleTestD", ExampleTestD);
	mainTestList.AddTest("ExampleTestD (1)", ExampleTestD);
	mainTestList.AddTest("ExampleTestD (2)", ExampleTestD);
	mainTestList.AddTest("ExampleTestD (3)", ExampleTestD);
	
	mainTestList.RunTests(nullptr);
	//rtuts::ConOutputTestResults(mainTestList.GetContainer(), rtuts::ConOutputSettings{ true, true });
	rtuts::ExtOutputTestResults(mainTestList.GetContainer(), rtuts::ExtOutputSettings{ true });

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
