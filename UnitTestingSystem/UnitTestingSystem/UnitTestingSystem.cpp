// UnitTestingSystem.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

#include <stack>
#include <string>
#include <vector>
#include <iostream>

//unit testing system
namespace uts
{
	/// <summary>
	/// A function that returns a bool. tru for good, false for bad.
	/// </summary>
	typedef bool(*UnitTest)();

	/// <summary>
	/// the nodes of the tree
	/// </summary>
	struct UTSNode
	{
		std::string m_holder = "";
		std::vector<int> m_children = std::vector<int>();
		int m_parent = -1;

		UnitTest m_test = nullptr;
		bool m_didSucced = true;
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
		/// Adds a new child cabable node/holder to the last holder created.
		/// </summary>
		/// <param name="holder">Node name.</param>
		void PushDomain(std::string holder);

		/// <summary>
		/// Adds a new child cabable node/holder to the last holder created.
		/// </summary>
		/// <param name="type">Object type for example: class, namespace ect.</param>
		/// <param name="holder">Node name.</param>
		void PushDomain(std::string type, std::string holder);

		/// <summary>
		/// Replace the last holder crated with the one created before it and in doing so all new pushed domains will be added to that insted.
		/// </summary>
		void PopDomain();


		/// <summary>
		/// Adds a unit test to the last hoder created. Tests can not have children.
		/// </summary>
		/// <param name="holder">Node name.</param>
		/// <param name="test">The unit test function.</param>
		void PushTest(std::string holder, UnitTest test);


		/// <summary>
		/// Kills the tree and everything in it and then creates a new tree with a root holder.
		/// </summary>
		void Replant(std::string holder = "root");


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
		std::stack<int> m_currentActiveDomain = std::stack<int>();
		UTSTree m_allDomains = UTSTree();
	};

	/// <summary>
	/// Outputs the results to the console.
	/// </summary>
	/// <param name="results">The data from UTSTreeConstructor.RunTests.</param>
	void ConOutputTestResults(const UTSTree* results);

	/// <summary>
	/// you shouldnt use this it is ment to be used by ConOutputTestResults only.
	/// </summary>
	/// <param name="results">The data from UTSTreeConstructor.RunTests.</param>
	/// <param name="domainIndex">The node we are starting from.</param>
	/// <param name="depth">Our depth so far.</param>
	void ConOutputDomainsAndSubDomains(const UTSTree* results, int domainIndex, int depth);
}


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


void uts::UTSTreeConstructor::PushDomain(std::string holder)
{
	int newDomainIndex = (int)m_allDomains.size();

	UTSNode newDomain = UTSNode();
	newDomain.m_holder = holder;
	newDomain.m_parent = m_currentActiveDomain.top();

	m_allDomains[m_currentActiveDomain.top()].m_children.push_back(newDomainIndex); //add new domain to parent node

	m_allDomains.push_back(newDomain); //add domain to tree
	m_currentActiveDomain.push(newDomainIndex); //make this the new parent node that children are added to
}

void uts::UTSTreeConstructor::PushDomain(std::string type, std::string holder)
{
	PushDomain(type + ": " + holder);
}

void uts::UTSTreeConstructor::PopDomain()
{
	m_currentActiveDomain.pop(); //go back to last node
}


void uts::UTSTreeConstructor::PushTest(std::string holder, UnitTest test)
{
	int newDomainIndex = (int)m_allDomains.size();

	UTSNode newDomain = UTSNode();
	newDomain.m_holder = "test: " + holder;
	newDomain.m_parent = m_currentActiveDomain.top();
	newDomain.m_test = test;

	m_allDomains[m_currentActiveDomain.top()].m_children.push_back(newDomainIndex); //add to parent

	m_allDomains.push_back(newDomain); //add to tree
}


void uts::UTSTreeConstructor::Replant(std::string holder)
{
	m_allDomains.clear();
	m_currentActiveDomain = std::stack<int>();

	UTSNode newDomain = UTSNode();
	newDomain.m_holder = holder;

	m_currentActiveDomain.push(0);
	m_allDomains.push_back(newDomain);
}


uts::UTSTree uts::UTSTreeConstructor::RunTests()
{
	UTSTree returnTree = m_allDomains; //copyies tree so that we can run tests on it without effecting the main tree

	for (size_t i = 0; i < returnTree.size(); i++)
	{
		if (returnTree[i].m_test != nullptr)
		{
			if (returnTree[i].m_test() == false) //test failed
			{
				int currentTreeTravelIndex = returnTree[i].m_parent;
				returnTree[i].m_didSucced = false; //mark failure

				while (currentTreeTravelIndex >= 0) //mark all parents as failed
				{
					returnTree[currentTreeTravelIndex].m_didSucced = false;
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


void uts::ConOutputTestResults(const UTSTree* results)
{
	int indentationIndex = 0;
	;
	std::string colorBaseWhite = "\033[0m";

	std::cout << colorBaseWhite << std::flush;
	std::cout << "======================================================================================================" << "\n";
	std::cout << "Unit Testing System Output" << "\n";
	std::cout << "======================================================================================================" << "\n\n";

	ConOutputDomainsAndSubDomains(results, 0, 0); //start with root and depth 0

	std::cout << "\n" << "======================================================================================================" << "\n";
}

void uts::ConOutputDomainsAndSubDomains(const UTSTree* results, int domainIndex, int depth)
{
	std::string colorBaseWhite = "\033[0m";
	std::string colorBaseRed = "\033[0;31;101m";
	std::string colorBaseGreen = "\033[0;32;102m";

	std::string indentationSpaceAmount = "   ";
	std::string indentationSpace = "";

	for (int i = 0; i < depth; i++)
	{
		indentationSpace += indentationSpaceAmount;
	}

	std::cout << indentationSpace << "[" << std::flush;

	std::cout << colorBaseGreen << std::flush;
	if ((*results)[domainIndex].m_didSucced == false) { std::cout << colorBaseRed << std::flush; }
	std::cout << "x" << std::flush;

	std::cout << colorBaseWhite << std::flush;
	std::cout << "] " << std::flush;
	std::cout << (*results)[domainIndex].m_holder << "\n";


	if ((*results)[domainIndex].m_children.size() > 0) //tests cant have chaildren so we shouldnt do this
	{
		std::cout << indentationSpace << "{" << "\n";
	}

	for (size_t i = 0; i < (*results)[domainIndex].m_children.size(); i++)
	{
		ConOutputDomainsAndSubDomains(results, (*results)[domainIndex].m_children[i], depth + 1);

		if ((*results)[(*results)[domainIndex].m_children[i]].m_children.size() > 0 && i != (*results)[domainIndex].m_children.size() - 1) //spaces out sections in same group
		{
			std::cout << "\n";
		}
	}

	if ((*results)[domainIndex].m_children.size() > 0) //tests cant have chaildren so we shouldnt do this
	{
		std::cout << indentationSpace << "}" << "\n";
	}
}

