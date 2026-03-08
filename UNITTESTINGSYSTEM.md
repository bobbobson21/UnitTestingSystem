
# namespace: uts; struct: UTSNode; class: UTSTreeConstructor<br>

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

Tests don't effect the stack as they can not have children but they do still link to there parent node who index will be at the top of the stack. Test functions also require no parameters and a boolean return there are no restrictions as to what can go in a test function beyond that.

ConOutputTestResults renderers/outputs the results and it should work on all platforms apart from play station and UTSTreeConstructor is how you actually put together the test structure.

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

Instead tests can be put inside of them and if a test fails it goes red and the domain will go red and so will every other parent domain to the root making it very easy to find out if something went wrong by going to the top of the test output and from there you can scroll down to you find the class and then the sub section and then the test.

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
Tests are best of always being in a domain and ideally there should be one for the file,class,static members,non static members, and inherited and public if there is a parent class.

To make a test you must fist make a function with no inputs and a bool output and if that function returns false the test fails and the function will be executed when RunTests is executed.

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
Although this is not actually needed for the final version of the engine its important because it helps ensure better code quality and it also provides proof that the code is being maintained to a high degree.

<br>


## Reflection 1.0.0<br>

### What was developed<br>
The unit test system static library was in whole what was developed this library contains the UTSTreeConstructor which allows you to assemble tests and domains in a way that allows you to easily spot where the failures are in the output. The library also contains some helper classes such as UTSTree and UTSNode which acts a tree elments also aiding in this simple to understand view.

Tests can be added though the UTSTree constructor and right now they are only a bool based system with false meaning that a rest failed.

<br>

### How I feel about this<br>
I really like how this library came together and how well it functions at primary task and I really like how well the UTSTreeConstructor I am not much of a bing fan to the file layout its self as it is a bit different from what I am used to given its a library and all but it is something I can cope with.

<br>

### Evaluation of work<br>
This library is effective at providing an understandable and readable layout when print the tests and results to console allowing for easy in tracking down issues. It is also effective at running the tests with the time complexity being o(n) as long as the individual tests them self are not factored in to the time complexity.

The library is a static one making it a bit harder to work with and it also means it is still a factor for compile time a non static version can be made but it will require all strings and vectors to be replaced with non likenesses. Likewise there also isn't much detail into the type of error (such as if its a try catch or an error code) and there also isn't information on what tasks its currently processing which is bad because timer related tasks may take a while which will stall execution till there done.

In conclusion the work done so far is sufficient at handling the errors them self's but it lacks the ability to provide a level of comprehensive insight into how exactly a test failed oh what tests are currently being processed.

<br>

### Conclusion<br>
A lot was learned from this experience prearticular with the std::stacks as well as with asni color code. A lot was also gained from the experience of developing this system but in prearticular the main takeaway is the knowledge I developed in creating my own custom library.

I could of made it so that test functions don't return a simple bool from the start and that would of lead to a better testing and handling of edge cases and I could of made some sort of testing disclaimer for the test that will take a while. but the library as a whole dose function remarkable well without failure or crashing.

<br>

### Next steps<br>
Next I want to disclaimer support so that tests which can freeze up the program can inform you about it before hand and want to add more error types and maybe turn it into a dll.

<br>

## Further reading<br>
   - [How to Write Unit Tests: A Problem-Solving Approach](https://www.testrail.com/blog/how-to-write-unit-tests/)
   - [Unit testing best practices for .NET](https://learn.microsoft.com/en-us/dotnet/core/testing/unit-testing-best-practices)
   - [11 unit testing best practices](https://www.ibm.com/think/insights/unit-testing-best-practices)
   - [Function Pointer in C++](https://www.geeksforgeeks.org/cpp/function-pointer-in-cpp/)
   - [20.1 — Function Pointers](https://www.learncpp.com/cpp-tutorial/function-pointers/)
	
<br>


>library and document done by: Kyle Edwards, e017770n@student.staffs.ac.uk&#xa0;&#xa0;&#xa0;\|&#xa0;&#xa0;&#xa0;First created: 06/3/2026&#xa0;&#xa0;&#xa0;\|&#xa0;&#xa0;&#xa0;Last updated: 08/03/2026