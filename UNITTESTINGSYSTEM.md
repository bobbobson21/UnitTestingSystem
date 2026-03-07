
# namespace: uts; UTSNode; class: UTSTreeConstructor<br>

| library | system | testing | copyable | instantiate |
|---------|--------|---------|----------|-------------|

## Contents (uses github MD parser)<br>
   o [Types](#Types)<br>
   o [Functions](#Functions)<br>
   o [How it works overview](#How-it-works-overview)<br>
   o [Usage examples](#Usage-examples)<br>
   &#xa0;&#xa0;&#xa0;^[Main example](#Main-example)<br>
   &#xa0;&#xa0;&#xa0;^[An example of how to use domains](#An-example-of-how-to-use-domains)<br>
   &#xa0;&#xa0;&#xa0;^[An example of how to use tests](#An-example-of-how-to-use-tests)<br>
   &#xa0;&#xa0;&#xa0;^[How to render results](#How-to-render-results)<br>
   o [Importance](#Importance)<br>
   o [Reflection 1.0.0](#Reflection-100)<br>
   o [Further reading](#Further-reading)<br>

<br>

## Types<br>

| types                                   | meaning                                                                                                      |
|-----------------------------------------|--------------------------------------------------------------------------------------------------------------|
| typedef bool(*UnitTest)()               | A format for storing the pointer to unit test functions.                                                     |
| typedef std::vector\<UTSNode\> UTSTree; | A format for stoing a collection of UTSNode that can be access as both a vector and a tree.                  |
| struct UTSNode                          | Contains infomation on test child nodes and parent nodes                                                     |
| class UTSTreeConstructor                | Allows you to build and run you unit test and acts a sort of middle man between you and the raw UTSTree.     |


## Functions<br>

| public UTSTreeConstructor class definitions   | what it dose                                
|-----------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------|
| void PushDomain(std::string holder)                   | Pushs a new domain/child barering node to the tree and then sets it as the active node which all other domain and test pushes will be put under. |
| void PushDomain(std::string type, std::string holder) | Pushs a new domain/child barering node to the tree but with a type and a name/holder.                                                            |
| void PopDomain()                                      | Pops the last domain from the setting the pervious active node to the active node once more.                                                     |
| void PushTest(std::string holder, UnitTest test)      | Pushes a test to a tree and pops it as well because tests cant have children                                                                     |
| void Replant(std::string holder = "root")             | Kills the tree and creates another one with a new root which can be given a name.                                                                |
| UTSTree RunTests()                                    | Runs all the unit test with o(n) time thanks to how UTSTree works. Then it outputs a copy of the currently constuced tree with the test data.    |
| UTSTree GetTree()                                     | Gets a copy of the currently constructed tree.                                                                                                   |


| uts namespace definitions                                                               | what it dose                                                                                               |  
|-----------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------|
| void ConOutputTestResults(const UTSTree* results)                                       | Outputs a UTS tree to console with infomation on what errors occore as long as the tests have been runned. |
| void ConOutputDomainsAndSubDomains(const UTSTree* results, int domainIndex, int depth); | Not really ment to be used but it outputs a small section of the tree to the console.                      |

<br>

## How it works overview<br>
This system uses a lot of custom made logic in order work. However the domains work due to a stack buff and every time a domain is added a new index is added to the buff and this index also relates to a index in the UTSTree. the node at whos related index as previouselly at the top of the tree gets a child link index added to it and the new node also gets it parent index set to that of the last node index at the top of the stack

Tests dont effect the stack as they can not have childrent but they do still link to there parent node whos index will be at the top of the stack. Test fuctions also require no pramaters and a boolean retun thare are no restrictions as to what can go in a test function beyond that.

ConOutputTestResults renderes the results and it should wokr on all platforms apart from playstation and UTSTreeConstructor is how you actually put togeter the test structuer.

<br>

## Usage examples<br>
### Main example<br>
An example of a unit test system tree can be found bellow. This example contains both tests and domains as well as domains in domains and it should function correctly.

```
#include <iostream>
#include "lib/UnitTestingSystem.h"

bool ExampleTest()
{
	return false;
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
 
```

Bellow this is also an example of its main output.
```
======================================================================================================
Unit Testing System Output
======================================================================================================

[!] root
{
   [!] namespace: testing
   {
      [!] class: tester
      {
         [!] static
         {
            [!] test: ExampleTest
            [.] test: ExampleTestC
            [.] test: ExampleTestC
            [.] test: ExampleTestC
            [.] test: ExampleTestC
         }

         [.] instance
         {
            [.] test: ExampleTestB
            [.] test: ExampleTestC
            [.] test: ExampleTestC
            [.] test: ExampleTestC
            [.] test: ExampleTestC
         }
      }
   }
}

======================================================================================================
```

### An example of how to use domains<br>
Domains are like your unity game objects they can store components/test and they can store other gameobjects/domains but they dont do anything else beyond that and they do not run tests.

Insted tests can be put inside of them and if a tast fails it gose red and the domain will go red and so will every other parent domain to the roo making it very easy to find out if someting whent wrong by going to the top of the test output and from there you can scroll down to you find the class and then the sub section and then the test.

Domains can be added with names and they dont need anything else but it is also woth noting for every push domain there MUST be a pop domain and vice versa.

```
	uts::UTSTreeConstructor mainTestTree = uts::UTSTreeConstructor();

	mainTestTree.PushDomain("namespace", "testing");

	mainTestTree.PushDomain("class", "graphicsA");
	mainTestTree.PushTest("ExampleTestB", ExampleTestB);
	mainTestTree.PopDomain();

	mainTestTree.PushDomain("class", "graphicsB"); //domains can be made with nothing but that is a bit pointless

	mainTestTree.PopDomain();

	mainTestTree.PushDomain("class", "graphicsC");
	mainTestTree.PushTest("ExampleTest", ExampleTest);
	mainTestTree.PopDomain();

	mainTestTree.PushDomain("class", "graphicsD");

	mainTestTree.PopDomain();

	mainTestTree.PopDomain();
```

### An example of how to use tests<br>
Tests are best of always being in a domain and idearly there should be one for the file,class,static menbers,non static members, and inherited and public if there is a parent class.

To make a test you must fist make a function with no inputs and a bool output and if that function retuns false the test fails and the function will be exacuted when RunTests is exacuted.

```
bool ExampleTest()
{
	return false;
}

	uts::UTSTreeConstructor mainTestTree = uts::UTSTreeConstructor();

	mainTestTree.PushDomain("class", "wow");
	mainTestTree.PushTest("ExampleTest", ExampleTest);
	mainTestTree.PopDomain();
```

<br>

### How to render results<br>
To render/output the results to console you must first create your uts tree using the UTSTreeConstructor after that exacute the function UTSTreeConstructor.RunTests() and send the results to uts::ConOutputTestResults which will only work on pcs.

```
bool ExampleTest() //creating tests
{
	return false;
}

	uts::UTSTreeConstructor mainTestTree = uts::UTSTreeConstructor(); //creating tree

	mainTestTree.PushDomain("class", "wow");
	mainTestTree.PushTest("ExampleTest", ExampleTest);
	mainTestTree.PopDomain();

	uts::UTSTree results = mainTestTree.RunTests(); //outputing results
	uts::ConOutputTestResults(&results);

```

<br>

## Importance<br>
Althougth this is not actually needed for the final version of the engie its important because it helps ensure better code quality and it also provides proof that the code is being mantained to a high degree.

<br>


## Reflection 1.0.0<br>
### What was developed<br>


<br>

### How I feel about this<br>


<br>

### Evaluration of work<br>


<br>

### Conclusion<br>


<br>

### Next steps<br>


<br>

## Further reading<br>
   - [How to Write Unit Tests: A Problem-Solving Approach](https://www.testrail.com/blog/how-to-write-unit-tests/)
   - [Unit testing best practices for .NET](https://learn.microsoft.com/en-us/dotnet/core/testing/unit-testing-best-practices)
   - [11 unit testing best practices](https://www.ibm.com/think/insights/unit-testing-best-practices)
   - [Function Pointer in C++](https://www.geeksforgeeks.org/cpp/function-pointer-in-cpp/)
   - [20.1 — Function Pointers](https://www.learncpp.com/cpp-tutorial/function-pointers/)
	
<br>


>Class and document done by: Kyle Edwards, e017770n@student.staffs.ac.uk&#xa0;&#xa0;&#xa0;\|&#xa0;&#xa0;&#xa0;First created: 07/3/2026&#xa0;&#xa0;&#xa0;\|&#xa0;&#xa0;&#xa0;Last updated: 07/0302026