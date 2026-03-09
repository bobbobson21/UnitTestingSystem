// utsNew.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "UnitTestingSystem.h"

void uts::UTSNode::free(void)
{
	delete[] m_children;
	delete[] m_runNotice;
	delete[] m_identifyer;
	m_childrenLength = 0;
}


void uts::UTSDataContainer::free(void)
{
	for (unsigned int i = 0; i < m_nodesLength; i++)
	{
		m_nodes[i].free();
	}

	delete[] m_nodes;
	m_nodesLength = 0;
}

void uts::UTSDataContainer::AddNode(UTSNode node, unsigned int parentIndex)
{
	//adds node to tree
	UTSNode* nodeBuffer = new UTSNode[m_nodesLength +1];

	if (m_nodes != nullptr)
	{
		memcpy(nodeBuffer, m_nodes, sizeof(UTSNode) * m_nodesLength);
		delete[] m_nodes;
	}

	node.m_parent = parentIndex;
	nodeBuffer[m_nodesLength] = node;

	m_nodes = nodeBuffer;
	m_nodesLength = m_nodesLength + 1;


	//adds child to parent
	if (m_nodesLength -1 != parentIndex)
	{
		unsigned int* parentChildrenBuffer = new unsigned int[m_nodes[parentIndex].m_childrenLength + 1];

		if (m_nodes[parentIndex].m_children != nullptr)
		{
			memcpy(parentChildrenBuffer, m_nodes[parentIndex].m_children, sizeof(unsigned int) * m_nodes[parentIndex].m_childrenLength);
			delete[] m_nodes[parentIndex].m_children;
		}

		parentChildrenBuffer[m_nodes[parentIndex].m_childrenLength] = m_nodesLength - 1;

		m_nodes[parentIndex].m_children = parentChildrenBuffer;
		m_nodes[parentIndex].m_childrenLength = m_nodes[parentIndex].m_childrenLength + 1;
	}
}


uts::UTSTreeConstructor::UTSTreeConstructor(void)
{
	m_treeMain = new UTSDataContainer();
	Replant("root");
}

uts::UTSTreeConstructor::UTSTreeConstructor(const char* rootName)
{
	Replant(rootName);
}

uts::UTSTreeConstructor::~UTSTreeConstructor(void)
{
	free(m_treeMain);
	freeStack();
}

void uts::UTSTreeConstructor::PushDomain(const char* identifyer)
{
	UTSNode newDomain = UTSNode();
	strcpy(newDomain.m_identifyer, identifyer);

	m_treeMain->AddNode(newDomain, ReadStack());
	PushToStack(m_treeMain->m_nodesLength - 1);
}

void uts::UTSTreeConstructor::PushDomain(const char* type, const char* identifyer)
{
	char* idBuffer = new char[(strlen(type) + strlen(identifyer) - 2) + 2];
	
	memcpy(idBuffer, type, strlen(type));
	idBuffer[strlen(type) +0] = ':';
	idBuffer[strlen(type) +1] = ' ';
	memcpy(idBuffer + (strlen(type) + 2), identifyer, strlen(identifyer));

	PushDomain(idBuffer);
	delete[] idBuffer;
}

void uts::UTSTreeConstructor::PopDomain()
{
	PopFromStack();
}


void uts::UTSTreeConstructor::PushPopTest(const char* identifyer, UTSUnitTest test)
{
	int newDomainIndex = (int)m_treeMain->m_nodesLength;

	UTSNode newDomain = UTSNode();
	newDomain.m_test = test;
	strcpy(newDomain.m_identifyer, identifyer);

	m_treeMain->AddNode(newDomain, ReadStack());
}

void uts::UTSTreeConstructor::PushPopNotice(const char* notice)
{
	int newDomainIndex = (int)m_treeMain->m_nodesLength;

	UTSNode newDomain = UTSNode();
	strcpy(newDomain.m_runNotice, notice);

	m_treeMain->AddNode(newDomain, ReadStack());
}


void uts::UTSTreeConstructor::Replant(const char* identifyer)
{
	//kills old tree
	free(m_treeMain);
	freeStack();

	//make new root node
	unsigned int rootIndex = 0; //it cant be anything else

	UTSNode root = UTSNode();
	strcpy(root.m_identifyer, identifyer);

	//adds node to tree
	m_treeMain->AddNode(root, rootIndex);
	PushToStack(rootIndex);

}


void uts::UTSTreeConstructor::RunTests(void)
{
	for (size_t i = 0; i < m_treeMain->m_nodesLength; i++)
	{
		if (m_treeMain->m_nodes[i].m_runNotice != nullptr)
		{
			std::cout << "\033[0;96;49m" << "Unit Test System is running: " << "\033[0m" << m_treeMain->m_nodes[i].m_runNotice << "\n";
		}

		if (m_treeMain->m_nodes[i].m_test != nullptr)
		{
			UTSTestSeverityCode resultCode = m_treeMain->m_nodes[i].m_test();

			if (resultCode != UTSTestSeverityCode::TSCPass) //test failed
			{
				int currentTreeTravelIndex = m_treeMain->m_nodes[i].m_parent;
				m_treeMain->m_nodes[i].m_severityCodeOfTest = resultCode; //mark failure

				while (true)
				{
					if (((unsigned int)resultCode) > ((unsigned int)m_treeMain->m_nodes[currentTreeTravelIndex].m_severityCodeOfTest))
					{
						m_treeMain->m_nodes[currentTreeTravelIndex].m_severityCodeOfTest = resultCode;
					}

					if (currentTreeTravelIndex == m_treeMain->m_nodes[currentTreeTravelIndex].m_parent) { break; } //we cant go back anymore
					currentTreeTravelIndex = m_treeMain->m_nodes[currentTreeTravelIndex].m_parent;
				}
			}
		}
	}
}

uts::UTSDataContainer* uts::UTSTreeConstructor::GetTree(void)
{
	return m_treeMain;
}


void uts::UTSTreeConstructor::PushToStack(unsigned int item)
{
	m_activeDomainStackIndex = m_activeDomainStackIndex + 1;

	if (m_activeDomainStackIndex >= m_activeDomainStackLength)
	{
		unsigned int* stackBuffer = new unsigned int[m_activeDomainStackLength + 1];

		if (m_activeDomainStackPointer != nullptr)
		{
			memcpy(stackBuffer, m_activeDomainStackPointer, sizeof(unsigned int) * m_activeDomainStackLength);
			delete[] m_activeDomainStackPointer;
		}

		m_activeDomainStackPointer = stackBuffer;
	}

	m_activeDomainStackPointer[m_activeDomainStackIndex] = item;
}

void uts::UTSTreeConstructor::PopFromStack()
{
	m_activeDomainStackIndex = m_activeDomainStackIndex -1;
}

void uts::UTSTreeConstructor::freeStack()
{
	delete[] m_activeDomainStackPointer;
	m_activeDomainStackLength = 0;
	m_activeDomainStackLength = 1;

}

unsigned int uts::UTSTreeConstructor::ReadStack()
{
	return m_activeDomainStackPointer[m_activeDomainStackIndex];
}


void uts::ConOutputTestResults(const UTSDataContainer* results, bool outputRunNotices)
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

void uts::ConOutputDomainsAndSubDomains(const UTSDataContainer* results, unsigned int domainIndex, unsigned int depth, bool outputRunNotices)
{
	char colorBaseWhite[] = "\033[0m";

	char indentationSpaceSingle[] = "\033[0;90;49m:  \033[0m"; //18 charters long

	if (*results->m_nodes[domainIndex].m_runNotice != '\0')
	{
		if (outputRunNotices == true)
		{
			for (unsigned int i = 0; i < depth; i++) { std::cout << indentationSpaceSingle; }
			std::cout << "\033[0;96;49m" << "[n] notice: " << results->m_nodes[domainIndex].m_runNotice << colorBaseWhite << "\n";
		}

		return;
	}

	for (unsigned int i = 0; i < depth; i++) { std::cout << indentationSpaceSingle; }
	std::cout << "[";

	ConOutputTestSeverityCode(results->m_nodes[domainIndex].m_severityCodeOfTest);

	std::cout << "] ";
	std::cout << results->m_nodes[domainIndex].m_identifyer << "\n";


	if (results->m_nodes[domainIndex].m_childrenLength > 0) //tests cant have chaildren so we shouldnt do this
	{
		for (unsigned int i = 0; i < depth; i++) { std::cout << indentationSpaceSingle; }
		std::cout << "{" << "\n";
	}

	for (size_t i = 0; i < results->m_nodes[domainIndex].m_childrenLength; i++)
	{
		ConOutputDomainsAndSubDomains(results, (*results)[domainIndex].m_children[i], depth + 1, outputRunNotices);

		if (results->m_nodes[results->m_nodes[domainIndex].m_children[i]].m_childrenLength > 0 && i != results->m_nodes[domainIndex].m_childrenLength - 1) //spaces out sections in same group
		{
			for (unsigned int i = 0; i <+ depth; i++) { std::cout << indentationSpaceSingle; }
			std::cout << indentationSpaceSingle << "\n";
		}
	}

	if (results->m_nodes[domainIndex].m_childrenLength > 0) //tests cant have chaildren so we shouldnt do this
	{
		for (unsigned int i = 0; i < +depth; i++) { std::cout << indentationSpaceSingle; }
		std::cout << indentationSpaceSingle << "\n";
	}
}
