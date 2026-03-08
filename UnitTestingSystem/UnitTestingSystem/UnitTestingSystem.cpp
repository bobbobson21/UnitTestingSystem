#pragma once

#include "pch.h"
#include "framework.h"

//HEADER STARTS HERE
#include <stack>
#include <string>
#include <vector>
#include <iostream>

//unit testing system 
namespace uts
{
	/// <summary>
	/// the lower down on the list it is the more important it is
	/// </summary>
	enum UTSTestSeverityCode : char
	{
		TSCPass,
		TSCWarning,
		TSCDetectionFailed,
		TSCTryCatchFail,
		TSCFail,
		TSCCrashFail,

		TSCMaxNull,
	};

	/// <summary>
	/// A function that returns a severity code TSCPass for good anything else for bad.
	/// </summary>
	typedef UTSTestSeverityCode(*UTSUnitTest)();

	/// <summary>
	/// the nodes of the tree
	/// </summary>
	struct UTSNode
	{
		std::string m_identifyer = "";
		std::string m_runNotice = ""; ///if set this will output text when its coaspoing test runs and it can also be rendered in the test tree output m_identifyer or test result will not be rendered
		std::vector<unsigned int> m_children = std::vector<unsigned int>();
		unsigned int m_parent = 0;

		UTSUnitTest m_test = nullptr;
		UTSTestSeverityCode m_severityCodeOfTest = UTSTestSeverityCode::TSCPass; ///the code the test returned when it was exacuted
	};

	/// <summary>
	/// The uts tree object which can be acess as both a vector and a tree.
	/// </summary>
	typedef std::vector<UTSNode> UTSTree;

	/// <summary>
	/// A wapper for UTSTree that allows you to build the unit test tree a lot easier and also test it.
	/// </summary>
	class UTSTreeConstructor
	{
	public:
		UTSTreeConstructor();
		UTSTreeConstructor(std::string rootName);
		~UTSTreeConstructor();


		/// <summary>
		/// Adds a new child cabable node/identifyer to the last identifyer created.
		/// </summary>
		/// <param name="identifyer">Node name.</param>
		void PushDomain(std::string identifyer);

		/// <summary>
		/// Adds a new child cabable node/identifyer to the last identifyer created.
		/// </summary>
		/// <param name="type">Object type for example: class, namespace ect.</param>
		/// <param name="identifyer">Node name.</param>
		void PushDomain(std::string type, std::string identifyer);

		/// <summary>
		/// Replace the last identifyer crated with the one created before it and in doing so all new pushed domains will be added to that insted.
		/// </summary>
		void PopDomain();


		/// <summary>
		/// Adds a unit test to the last hoder created. Tests can not have children.
		/// </summary>
		/// <param name="identifyer">Node name.</param>
		/// <param name="test">The unit test function.</param>
		void PushPopTest(std::string identifyer, UTSUnitTest test);

		/// <summary>
		/// Adds a unit test to the last hoder created. Tests can not have children.
		/// </summary>
		void PushPopNotice(std::string notice);


		/// <summary>
		/// Kills the tree and everything in it and then creates a new tree with a root identifyer.
		/// </summary>
		void Replant(std::string identifyer = "root");


		/// <summary>
		/// Runs thougth the tree exacuting every test it finds on the way.
		/// </summary>
		/// <returns>The tree object with the results on what tests succeded and what didnt.</returns>
		UTSTree RunTests();

		/// <summary>
		/// Get the tree but without any tests being runned.
		/// </summary>
		/// <returns>The tree.</returns>
		UTSTree GetTree();

	private:
		std::stack<unsigned int> m_currentActiveDomain = std::stack<unsigned int>();
		UTSTree m_allDomains = UTSTree();
	};

	/// <summary>
	/// Outputs the results to the console.
	/// </summary>
	/// <param name="results">The data from UTSTreeConstructor.RunTests.</param>
	void ConOutputTestResults(const UTSTree* results, bool outputRunNotices = false);

	/// <summary>
	/// outputs the charater and color related to a test code into the console
	/// </summary>
	/// <param name="testCode">the test code</param>
	void ConOutputTestSeverityCode(UTSTestSeverityCode testCode);

	/// <summary>
	/// you shouldnt use this it is ment to be used by ConOutputTestResults only.
	/// </summary>
	/// <param name="results">The data from UTSTreeConstructor.RunTests.</param>
	/// <param name="domainIndex">The node we are starting from.</param>
	/// <param name="depth">Our depth so far.</param>
	void ConOutputDomainsAndSubDomains(const UTSTree* results, unsigned int domainIndex, unsigned int depth, bool outputRunNotices = false);
}
//HEADER ENDS HERE

uts::UTSTreeConstructor::UTSTreeConstructor()
{
	Replant();
}

uts::UTSTreeConstructor::UTSTreeConstructor(std::string rootName)
{
	Replant(rootName);
}

uts::UTSTreeConstructor::~UTSTreeConstructor()
{
}


void uts::UTSTreeConstructor::PushDomain(std::string identifyer)
{
	int newDomainIndex = (int)m_allDomains.size();

	UTSNode newDomain = UTSNode();
	newDomain.m_identifyer = identifyer;
	newDomain.m_parent = m_currentActiveDomain.top();

	m_allDomains[m_currentActiveDomain.top()].m_children.push_back(newDomainIndex); //add new domain to parent node

	m_allDomains.push_back(newDomain); //add domain to tree
	m_currentActiveDomain.push(newDomainIndex); //make this the new parent node that children are added to
}

void uts::UTSTreeConstructor::PushDomain(std::string type, std::string identifyer)
{
	PushDomain(type + ": " + identifyer);
}

void uts::UTSTreeConstructor::PopDomain()
{
	m_currentActiveDomain.pop(); //go back to last node
}


void uts::UTSTreeConstructor::PushPopTest(std::string identifyer, UTSUnitTest test)
{
	int newDomainIndex = (int)m_allDomains.size();

	UTSNode newDomain = UTSNode();
	newDomain.m_identifyer = "test: " + identifyer;
	newDomain.m_parent = m_currentActiveDomain.top();
	newDomain.m_test = test;

	m_allDomains[m_currentActiveDomain.top()].m_children.push_back(newDomainIndex); //add to parent

	m_allDomains.push_back(newDomain); //add to tree
}

void uts::UTSTreeConstructor::PushPopNotice(std::string notice)
{
	int newDomainIndex = (int)m_allDomains.size();

	UTSNode newDomain = UTSNode();
	newDomain.m_parent = m_currentActiveDomain.top();
	newDomain.m_runNotice = notice;

	m_allDomains[m_currentActiveDomain.top()].m_children.push_back(newDomainIndex); //add to parent

	m_allDomains.push_back(newDomain); //add to tree
}


void uts::UTSTreeConstructor::Replant(std::string identifyer)
{
	m_allDomains.clear();
	m_currentActiveDomain = std::stack<unsigned int>();

	UTSNode newDomain = UTSNode();
	newDomain.m_identifyer = identifyer;

	m_currentActiveDomain.push(0);
	m_allDomains.push_back(newDomain);
}


uts::UTSTree uts::UTSTreeConstructor::RunTests()
{
	UTSTree returnTree = m_allDomains; //copyies tree so that we can run tests on it without effecting the main tree

	for (size_t i = 0; i < returnTree.size(); i++)
	{
		if (returnTree[i].m_runNotice != "")
		{
			std::cout << "\033[0;96;49m" << "Unit Test System is running: " << "\033[0m" << returnTree[i].m_runNotice << "\n";
		}

		if (returnTree[i].m_test != nullptr)
		{
			UTSTestSeverityCode resultCode = returnTree[i].m_test();

			if (resultCode != UTSTestSeverityCode::TSCPass) //test failed
			{
				int currentTreeTravelIndex = returnTree[i].m_parent;
				returnTree[i].m_severityCodeOfTest = resultCode; //mark failure

				while (true)
				{
					if (((unsigned int)resultCode) > ((unsigned int)returnTree[currentTreeTravelIndex].m_severityCodeOfTest))
					{
						returnTree[currentTreeTravelIndex].m_severityCodeOfTest = resultCode;
					}

					if (currentTreeTravelIndex == returnTree[currentTreeTravelIndex].m_parent) { break; } //we cant go back anymore
					currentTreeTravelIndex = returnTree[currentTreeTravelIndex].m_parent;
				}
			}
		}
	}

	return returnTree;
}

uts::UTSTree uts::UTSTreeConstructor::GetTree()
{
	return m_allDomains;
}


void uts::ConOutputTestResults(const UTSTree* results, bool outputRunNotices)
{

	std::cout << "\033[0;96;49m" << "\n";
	std::cout << "======================================================================================================" << "\n";
	std::cout << "Unit Testing System Output" << "\n";
	std::cout << "======================================================================================================" << "\n\n";
	std::cout << "\033[0m";

	int startingIndexOfTree = 0; //root is always 0
	int startingDepth = 0;
	ConOutputDomainsAndSubDomains(results, startingIndexOfTree, startingDepth, outputRunNotices); //start with root and depth 0

	std::cout << "\033[0;96;49m" << "\n";
	std::cout << "======================================================================================================" << "\n";
	std::cout << "\033[0m";
}

void uts::ConOutputTestSeverityCode(UTSTestSeverityCode testCode)
{
	switch (testCode)
	{
	case UTSTestSeverityCode::TSCPass:
		std::cout << "\033[0;32;102m" << "+";
		break;
	case UTSTestSeverityCode::TSCWarning:
		std::cout << "\033[0;33;103m" << "~";
		break;
	case UTSTestSeverityCode::TSCDetectionFailed: case TSCMaxNull: default:
		std::cout << "\033[0;30;100m" << "?";
		break;
	case UTSTestSeverityCode::TSCTryCatchFail:
		std::cout << "\033[0;30;45m" << "/";
		break;
	case UTSTestSeverityCode::TSCFail:
		std::cout << "\033[0;31;101m" << "!";
		break;
	case UTSTestSeverityCode::TSCCrashFail:
		std::cout << "\033[0;36;106m" << "*";

		break;
	}

	std::cout << "\033[0m";
}

void uts::ConOutputDomainsAndSubDomains(const UTSTree* results, unsigned int domainIndex, unsigned int depth, bool outputRunNotices)
{
	std::string colorBaseWhite = "\033[0m";

	std::string indentationSpaceAmount = "\033[0;90;49m:  " + colorBaseWhite;
	std::string indentationSpace = "";

	for (int i = 0; i < depth; i++)
	{
		indentationSpace += indentationSpaceAmount;
	}

	if ((*results)[domainIndex].m_runNotice != "")
	{
		if (outputRunNotices == true)
		{
			std::cout << indentationSpace << "\033[0;96;49m" << "[n] notice: " << (*results)[domainIndex].m_runNotice << colorBaseWhite << "\n";
		}

		return;
	}

	std::cout << indentationSpace << "[";

	ConOutputTestSeverityCode((*results)[domainIndex].m_severityCodeOfTest);

	std::cout << "] ";
	std::cout << (*results)[domainIndex].m_identifyer << "\n";


	if ((*results)[domainIndex].m_children.size() > 0) //tests cant have chaildren so we shouldnt do this
	{
		std::cout << indentationSpace << "{" << "\n";
	}

	for (size_t i = 0; i < (*results)[domainIndex].m_children.size(); i++)
	{
		ConOutputDomainsAndSubDomains(results, (*results)[domainIndex].m_children[i], depth + 1, outputRunNotices);

		if ((*results)[(*results)[domainIndex].m_children[i]].m_children.size() > 0 && i != (*results)[domainIndex].m_children.size() - 1) //spaces out sections in same group
		{
			std::cout << indentationSpace << indentationSpaceAmount << "\n";
		}
	}

	if ((*results)[domainIndex].m_children.size() > 0) //tests cant have chaildren so we shouldnt do this
	{
		std::cout << indentationSpace << "}" << "\n";
	}
}

