// utsNew.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"

#include "UnitTestingSystem.h"

void uts::UTSUnitTestResults::free(void)
{
	delete[] m_testResultDescriptionBuffer;

	delete[] m_unitTestFileLocationFromRoot;
	delete[] m_unitTestfileLocationRelative;
	
	delete[] m_objectFileLocationFromRoot;
	delete[] m_objectFileLocationRelative;
}

void uts::UTSNode::free(void)
{
	delete[] m_children;
	delete[] m_runningTestsBellowNotice;
	delete[] m_identifyer;
	m_childrenLength = 0;

	m_testResults.free();
}


void uts::UTSDataContainer::free(void)
{
	for (unsigned int i = 0; i < m_nodesLength; i++)
	{
		m_nodes[i].free();
	}

	delete[] m_nodes;
	m_nodesLength = 0;
	m_rootObjectsLength = 0;
}

void uts::UTSDataContainer::AddNode(UTSNode node, unsigned int parentIndex)
{
	//adds node to tree
	UTSNode* nodeBuffer = new UTSNode[m_nodesLength + 1];

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

	if (m_nodesLength - 1 != parentIndex)
	{
		unsigned int* parentChildrenBuffer = new unsigned int[m_nodes[parentIndex].m_childrenLength + 1];

		if (m_nodes[parentIndex].m_children != nullptr)
		{
			memcpy(parentChildrenBuffer, m_nodes[parentIndex].m_children, sizeof(int) * m_nodes[parentIndex].m_childrenLength);
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
	m_treeMain->free();
	DomainIdStackFree();

	delete m_treeMain;
}

void uts::UTSTreeConstructor::PushDomain(const char* identifyer)
{
	UTSNode newDomain = UTSNode();

	newDomain.m_identifyer = new char[strnlen(identifyer, UTSNEW__MAX_STRING_BUFFER_SIZE) + 1];
	memcpy(newDomain.m_identifyer, identifyer, sizeof(char) * (strnlen(identifyer, UTSNEW__MAX_STRING_BUFFER_SIZE) + 1));

	m_treeMain->AddNode(newDomain, DomainIdStackRead());
	DomainIdStackPush(m_treeMain->m_nodesLength - 1);
}

void uts::UTSTreeConstructor::PushDomain(const char* type, const char* identifyer)
{
	char* idBuffer = new char[(strlen(type) + strlen(identifyer)) + 3];

	memcpy(idBuffer, type, strlen(type));
	idBuffer[strlen(type) + 0] = ':';
	idBuffer[strlen(type) + 1] = ' ';
	memcpy(idBuffer + (strlen(type) + 2), identifyer, strlen(identifyer));
	idBuffer[strlen(type) + strlen(identifyer) + 2] = '\0';

	PushDomain(idBuffer);
	delete[] idBuffer;
}


void uts::UTSTreeConstructor::PopDomain()
{
	DomainIdStackPop();
}


void uts::UTSTreeConstructor::PushPopTest(const char* identifyer, UTSUnitTest test)
{
	UTSNode newDomain = UTSNode();
	newDomain.m_test = test;

	char type[] = "test";
	char* idBuffer = new char[(strlen(type) + strlen(identifyer)) + 3];

	memcpy(idBuffer, type, strlen(type));
	idBuffer[strlen(type) + 0] = ':';
	idBuffer[strlen(type) + 1] = ' ';
	memcpy(idBuffer + (strlen(type) + 2), identifyer, strlen(identifyer));
	idBuffer[strlen(type) + strlen(identifyer) + 2] = '\0';

	newDomain.m_identifyer = idBuffer;
	m_treeMain->AddNode(newDomain, DomainIdStackRead());
}

void uts::UTSTreeConstructor::PushPopNotice(const char* notice)
{
	UTSNode newDomain = UTSNode();

	newDomain.m_runningTestsBellowNotice = new char[strnlen(notice, UTSNEW__MAX_STRING_BUFFER_SIZE) + 1];
	memcpy(newDomain.m_runningTestsBellowNotice, notice, sizeof(char) * (strnlen(notice, UTSNEW__MAX_STRING_BUFFER_SIZE) + 1));

	m_treeMain->AddNode(newDomain, DomainIdStackRead());
}


void uts::UTSTreeConstructor::Replant(const char* identifyer)
{
	//kills old tree
	m_treeMain->free();
	DomainIdStackFree();

	m_treeMain->m_rootObjectsLength = 1;

	//make new root node
	unsigned int rootIndex = 0; //it cant be anything else

	UTSNode root = UTSNode();

	root.m_identifyer = new char[strnlen(identifyer, UTSNEW__MAX_STRING_BUFFER_SIZE) + 1];
	memcpy(root.m_identifyer, identifyer, sizeof(char) * (strnlen(identifyer, UTSNEW__MAX_STRING_BUFFER_SIZE) + 1));

	//adds node to tree
	m_treeMain->AddNode(root, rootIndex);
	DomainIdStackPush(rootIndex);

}


void uts::UTSTreeConstructor::RunTests(void* args)
{
	for (size_t i = 0; i < m_treeMain->m_nodesLength; i++)
	{
		if (m_treeMain->m_nodes[i].m_runningTestsBellowNotice != nullptr)
		{
			std::cout << "\033[0;96;49m" << "Unit Test System is running: " << "\033[0m" << m_treeMain->m_nodes[i].m_runningTestsBellowNotice << "\n";
		}

		if (m_treeMain->m_nodes[i].m_test != nullptr)
		{
			m_treeMain->m_nodes[i].m_testResults = m_treeMain->m_nodes[i].m_test(args);
			UTSUnitTestSeverityCode resultCode = m_treeMain->m_nodes[i].m_testResults.m_testResultSeverityCode;

			if (resultCode != UTSUnitTestSeverityCode::TSCPass) //test failed
			{
				m_treeMain->m_nodes[i].m_testResults.m_testResultSeverityCode = resultCode;
				int currentTreeTravelIndex = m_treeMain->m_nodes[i].m_parent;

				while (true) //back propagate to parents
				{
					if (((unsigned int)resultCode) > ((unsigned int)m_treeMain->m_nodes[currentTreeTravelIndex].m_testResults.m_testResultSeverityCode))
					{
						m_treeMain->m_nodes[currentTreeTravelIndex].m_testResults.m_testResultSeverityCode = resultCode;
					}

					if (currentTreeTravelIndex == m_treeMain->m_nodes[currentTreeTravelIndex].m_parent) { break; } //we cant go back anymore
					currentTreeTravelIndex = m_treeMain->m_nodes[currentTreeTravelIndex].m_parent;
				}
			}
		}
	}
}

uts::UTSDataContainer* uts::UTSTreeConstructor::GetContainer(void)
{
	return m_treeMain;
}


void uts::UTSTreeConstructor::DomainIdStackPush(unsigned int item)
{
	if (m_activeDomainStackIndex >= m_activeDomainStackLength)
	{
		m_activeDomainStackLength = m_activeDomainStackLength + 1;
		unsigned int* stackBuffer = new unsigned int[m_activeDomainStackLength];

		if (m_activeDomainStackPointer != nullptr)
		{
			memcpy(stackBuffer, m_activeDomainStackPointer, sizeof(unsigned int) * m_activeDomainStackLength);
			delete[] m_activeDomainStackPointer;
		}

		m_activeDomainStackPointer = stackBuffer;
	}

	m_activeDomainStackPointer[m_activeDomainStackIndex] = item;
	m_activeDomainStackIndex = m_activeDomainStackIndex + 1;
}

void uts::UTSTreeConstructor::DomainIdStackPop()
{
	m_activeDomainStackIndex = m_activeDomainStackIndex - 1;
}

void uts::UTSTreeConstructor::DomainIdStackFree()
{
	delete[] m_activeDomainStackPointer;
	m_activeDomainStackLength = 0;
	m_activeDomainStackLength = 0;
}

unsigned int uts::UTSTreeConstructor::DomainIdStackRead()
{
	return m_activeDomainStackPointer[m_activeDomainStackIndex - 1];
}


uts::UTSListConstructor::UTSListConstructor(void)
{
	m_listMain = new UTSDataContainer();
}

uts::UTSListConstructor::~UTSListConstructor(void)
{
	free(m_listMain);

	delete m_listMain;
}

void uts::UTSListConstructor::AddTest(const char* identifyer, UTSUnitTest test)
{
	unsigned int newDomainIndex = m_listMain->m_nodesLength;

	UTSNode newDomain = UTSNode();
	newDomain.m_test = test;

	char type[] = "test";
	char* idBuffer = new char[(strlen(type) + strlen(identifyer)) + 3];

	memcpy(idBuffer, type, strlen(type));
	idBuffer[strlen(type) + 0] = ':';
	idBuffer[strlen(type) + 1] = ' ';
	memcpy(idBuffer + (strlen(type) + 2), identifyer, strlen(identifyer));
	idBuffer[strlen(type) + strlen(identifyer) + 2] = '\0';

	newDomain.m_identifyer = idBuffer;
	m_listMain->AddNode(newDomain, newDomainIndex);
	m_listMain->m_rootObjectsLength = m_listMain->m_rootObjectsLength + 1; //so this list prints
}

void uts::UTSListConstructor::AddNotice(const char* notice)
{
	unsigned int newDomainIndex = m_listMain->m_nodesLength;
	UTSNode newDomain = UTSNode();

	newDomain.m_runningTestsBellowNotice = new char[strnlen(notice, UTSNEW__MAX_STRING_BUFFER_SIZE) + 1];
	memcpy(newDomain.m_runningTestsBellowNotice, notice, sizeof(char) * (strnlen(notice, UTSNEW__MAX_STRING_BUFFER_SIZE) + 1));

	m_listMain->AddNode(newDomain, newDomainIndex);
	m_listMain->m_rootObjectsLength = m_listMain->m_rootObjectsLength + 1; //so this list prints
}


void uts::UTSListConstructor::ClearList(void)
{
	m_listMain->free();
}


void uts::UTSListConstructor::RunTests(void* args)
{
	for (unsigned int i = 0; i < m_listMain->m_nodesLength; i++)
	{
		if (m_listMain->m_nodes[i].m_runningTestsBellowNotice != nullptr)
		{
			std::cout << "\033[0;96;49m" << "Unit Test System is running: " << "\033[0m" << m_listMain->m_nodes[i].m_runningTestsBellowNotice << "\n";
		}

		if (m_listMain->m_nodes[i].m_test != nullptr)
		{
			m_listMain->m_nodes[i].m_test(args);
		}
	}
}

uts::UTSDataContainer* uts::UTSListConstructor::GetContainer(void)
{
	return m_listMain;
}


void uts::ConOutputTestResults(const UTSDataContainer* results, ConOutputSettings settings)
{
	std::cout << "\033[0;96;49m" << "\n";
	std::cout << "======================================================================================================" << "\n";
	std::cout << "Unit Testing System Output" << "\n";
	std::cout << "======================================================================================================" << "\n\n";
	std::cout << "\033[0m";

	int startingDepth = 0;

	for (unsigned int indexOfRootNode = 0; indexOfRootNode < results->m_rootObjectsLength; indexOfRootNode++)
	{
		ConOutputDomainsAndSubDomains(results, indexOfRootNode, startingDepth, settings); //start with root and depth 0
	}

	std::cout << "\033[0;96;49m" << "\n";
	std::cout << "======================================================================================================" << "\n";
	std::cout << "\033[0m";
}

void uts::ConOutputTestSeverityCode(UTSUnitTestSeverityCode testCode)
{
	switch (testCode)
	{
	case UTSUnitTestSeverityCode::TSCPass:
		std::cout << "\033[0;32;102m" << "+";
		break;
	case UTSUnitTestSeverityCode::TSCWarning:
		std::cout << "\033[0;33;103m" << "~";
		break;
	case UTSUnitTestSeverityCode::TSCDetectionFailed:
		std::cout << "\033[0;30;100m" << "?";
		break;
	case UTSUnitTestSeverityCode::TSCTryCatchFail:
		std::cout << "\033[0;30;45m" << "/";
		break;
	case UTSUnitTestSeverityCode::TSCFail:
		std::cout << "\033[0;31;101m" << "!";
		break;
	case UTSUnitTestSeverityCode::TSCCrashFail:
		std::cout << "\033[0;36;106m" << "*";
		break;
	case TSCMaxNull: default:
		std::cout << "\033[0;95;100m" << "?";
		break;
	}

	std::cout << "\033[0m";
}

void uts::ConOutputDomainsAndSubDomains(const UTSDataContainer* results, unsigned int domainIndex, unsigned int depth, ConOutputSettings settings)
{
	char colorBaseWhite[] = "\033[0m";

	char indentationSpaceSingle[] = "\033[0;90;49m:  \033[0m"; //18 charters long

	if (results->m_nodes[domainIndex].m_runningTestsBellowNotice != nullptr) //DOSE NOTICES
	{
		if (settings.m_outputRunNotices == true)
		{
			for (unsigned int i = 0; i < depth; i++) { std::cout << indentationSpaceSingle; }
			std::cout << "\033[0;96;49m" << "[n] notice: " << results->m_nodes[domainIndex].m_runningTestsBellowNotice << colorBaseWhite << "\n";
		}

		return;
	}

	for (unsigned int i = 0; i < depth; i++) { std::cout << indentationSpaceSingle; } //DOSE THE SQUARE SECTION AND THE IDENTIFYER
	std::cout << "[";

	ConOutputTestSeverityCode(results->m_nodes[domainIndex].m_testResults.m_testResultSeverityCode);

	std::cout << "] ";
	std::cout << results->m_nodes[domainIndex].m_identifyer;

	if (settings.m_outputFailureDesriptions == true && results->m_nodes[domainIndex].m_testResults.m_testResultDescriptionBuffer != nullptr) //DOSE DESRCIPTION
	{
		std::cout << ": <" << results->m_nodes[domainIndex].m_testResults.m_testResultDescriptionBuffer << ">";
	}

	std::cout << "\n";


	if (results->m_nodes[domainIndex].m_childrenLength > 0) //group start for domains
	{
		for (unsigned int i = 0; i < depth; i++) { std::cout << indentationSpaceSingle; }
		std::cout << "{" << "\n";
	}

	for (size_t i = 0; i < results->m_nodes[domainIndex].m_childrenLength; i++)//DOSE SUB DOMAINS
	{
		ConOutputDomainsAndSubDomains(results, results->m_nodes[domainIndex].m_children[i], depth + 1, settings);

		if (results->m_nodes[results->m_nodes[domainIndex].m_children[i]].m_childrenLength > 0 && i != results->m_nodes[domainIndex].m_childrenLength - 1) //spaces out sections in same group
		{
			for (unsigned int i = 0; i < +depth; i++) { std::cout << indentationSpaceSingle; }
			std::cout << indentationSpaceSingle << "\n";
		}
	}

	if (results->m_nodes[domainIndex].m_childrenLength > 0) //group end for domains
	{
		for (unsigned int i = 0; i < +depth; i++) { std::cout << indentationSpaceSingle; }
		std::cout << "}" << "\n";
	}
}

bool uts::ExtOutputTestResults(const UTSDataContainer* results, ExtOutputSettings settings)
{
	const unsigned int maxCreationDelay = 250;
	const unsigned int maxCreationAttempts = 8;
	const unsigned int numberBufferLength = 8;

	char normalExportLoccation[] = "..\\UTSext.dat";
	char inlineExportLoccation[] = "UTSext.dat";

	char* fileLocation = normalExportLoccation;

	if (settings.m_projectIsInline == true)
	{
		fileLocation = inlineExportLoccation;
	}

	while (true) //validate file access
	{
		if (settings.m_waitForCollection == false) { break; }

		std::ifstream readStream = std::ifstream(fileLocation);

		if (readStream.good() == false) { break; }

		readStream.seekg(0, std::ios::end);
		if (readStream.tellg() <= 2) { break; }
	}

	std::fstream file = std::fstream();
	file.open(fileLocation, std::ios::out | std::ios::in | std::ios::trunc);

	if (file.is_open() == false) { return false; }

	file << "{";

	char rootObjectsLengthNumBuffer[numberBufferLength] = {};
	std::snprintf(rootObjectsLengthNumBuffer, numberBufferLength, "%d", results->m_rootObjectsLength);
	file << "\"rootObjectsLength\": " << rootObjectsLengthNumBuffer << ",";

	file << "\"nodes\": [";

	for (size_t i = 0; i < results->m_nodesLength; i++)
	{
		UTSNode currentNode = results->m_nodes[i];
		file << "{";

		if (currentNode.m_identifyer != nullptr) { file << "\"identifyer\": \"" << currentNode.m_identifyer << "\","; }
		else { file << "\"identifyer\": \"\","; }

		if (currentNode.m_runningTestsBellowNotice != nullptr) { file << "\"runningTestsBellowNotice\": \"notice: " << currentNode.m_runningTestsBellowNotice << "\","; }
		else { file << "\"runningTestsBellowNotice\": \"\","; }

		file << "\"parent\": " << currentNode.m_parent << ",";
		file << "\"children\": [";
		for (unsigned int o = 0; o < currentNode.m_childrenLength; o++)
		{
			file << currentNode.m_children[o];
			if (o != currentNode.m_childrenLength - 1) { file << ","; }
		}
		file << "],";

		file << "\"testResults\": {";

		char testResultSeverityCodeNumBuffer[numberBufferLength] = {};
		std::snprintf(testResultSeverityCodeNumBuffer, numberBufferLength, "%d", currentNode.m_testResults.m_testResultSeverityCode);
		file << "\"testResultSeverityCode\": " << testResultSeverityCodeNumBuffer << ",";

		if (currentNode.m_testResults.m_testResultDescriptionBuffer != nullptr) { file << "\"testResultDescriptionBuffer\": \"" << currentNode.m_testResults.m_testResultDescriptionBuffer << "\","; }
		else { file << "\"testResultDescriptionBuffer\": \"\","; }

		if (currentNode.m_testResults.m_objectFileLocationFromRoot != nullptr) { file << "\"objectFileLocation\": \"" << currentNode.m_testResults.m_objectFileLocationFromRoot << "\","; }
		else if (currentNode.m_testResults.m_objectFileLocationRelative != nullptr) { file << "\"objectFileLocation\": \"" << currentNode.m_testResults.m_objectFileLocationRelative << "\","; }
		else { file << "\"objectFileLocation\": \"\","; }

		if (currentNode.m_testResults.m_objectFileLocationFromRoot != nullptr) { file << "\"isObjectFileLocationFromRoot\": true,"; }
		else { file << "\"isObjectFileLocationFromRoot\": false,"; }

		if (currentNode.m_testResults.m_unitTestFileLocationFromRoot != nullptr) { file << "\"unitTestFileLocation\": \"" << currentNode.m_testResults.m_unitTestFileLocationFromRoot << "\","; }
		else if (currentNode.m_testResults.m_unitTestfileLocationRelative != nullptr) { file << "\"unitTestFileLocation\": \"" << currentNode.m_testResults.m_unitTestfileLocationRelative << "\","; }
		else { file << "\"unitTestFileLocation\": \"\","; }

		if (currentNode.m_testResults.m_unitTestFileLocationFromRoot != nullptr) { file << "\"isUnitTestFileLocationFromRoot\": true"; }
		else { file << "\"isUnitTestFileLocationFromRoot\": false"; }

		file << "}";
		file << "}";
		if (i != results->m_nodesLength - 1) { file << ","; }
	}

	file << "]";
	file << "}";

	file.close();

	return true;
}

