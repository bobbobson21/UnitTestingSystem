// RTRTUTS.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"

#include "RunTimeUnitTestingSystem.h"


void rtuts::RTUTSUnitTestResults::free(void)
{
	delete[] m_testResultDescriptionBuffer;

	delete[] m_unitTestFileLocationFromRoot;
	delete[] m_unitTestfileLocationRelative;
	
	delete[] m_objectFileLocationFromRoot;
	delete[] m_objectFileLocationRelative;
}

void rtuts::RTUTSNode::free(void)
{
	m_testResults.free();

	delete[] m_children;
	delete[] m_runningTestsBellowNotice;
	delete[] m_identifyer;
	m_childrenLength = 0;
}


void rtuts::RTUTSDataContainer::free(void)
{
	for (unsigned int i = 0; i < m_nodesLength; i++)
	{
		m_nodes[i].free();
	}

	delete[] m_nodes;

	m_nodesLength = 0;
	m_rootObjectsLength = 0;
	m_rootObjectsAvoidanceLength = 0;
}

void rtuts::RTUTSDataContainer::AddNode(RTUTSNode node, unsigned int parentIndex)
{
	//adds node to tree
	RTUTSNode* nodeBuffer = new RTUTSNode[m_nodesLength + 1];

	if (m_nodes != nullptr)
	{
		memcpy(nodeBuffer, m_nodes, sizeof(RTUTSNode) * m_nodesLength);
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



rtuts::RTUTSTreeConstructor::RTUTSTreeConstructor(void)
{
	m_treeMain = new RTUTSDataContainer();
	Replant("tree root");
}

rtuts::RTUTSTreeConstructor::RTUTSTreeConstructor(const char* rootName)
{
	Replant(rootName);
}

rtuts::RTUTSTreeConstructor::~RTUTSTreeConstructor(void)
{
	m_treeMain->free();
	DomainIdStackFree();

	delete m_treeMain;
}

void rtuts::RTUTSTreeConstructor::PushDomain(const char* identifyer)
{
	RTUTSNode newDomain = RTUTSNode();

	newDomain.m_identifyer = new char[strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 1];
	memcpy(newDomain.m_identifyer, identifyer, sizeof(char) * (strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 1));

	m_treeMain->AddNode(newDomain, DomainIdStackRead());
	DomainIdStackPush(m_treeMain->m_nodesLength - 1);
}

void rtuts::RTUTSTreeConstructor::PushDomain(const char* type, const char* identifyer)
{
	char* idBuffer = new char[(strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE)) + 3];

	memcpy(idBuffer, type, strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE));
	idBuffer[strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + 0] = ':';
	idBuffer[strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + 1] = ' ';
	memcpy(idBuffer + (strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + 2), identifyer, strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE));
	idBuffer[strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 2] = '\0';

	PushDomain(idBuffer);
	delete[] idBuffer;
}


void rtuts::RTUTSTreeConstructor::PopDomain()
{
	DomainIdStackPop();
}


void rtuts::RTUTSTreeConstructor::PushPopTest(const char* identifyer, RTUTSUnitTest test)
{
	RTUTSNode newDomain = RTUTSNode();
	newDomain.m_test = test;

	char type[] = "test";
	char* idBuffer = new char[(strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE)) + 3];

	memcpy(idBuffer, type, strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE));
	idBuffer[strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + 0] = ':';
	idBuffer[strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + 1] = ' ';
	memcpy(idBuffer + (strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + 2), identifyer, strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE));
	idBuffer[strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 2] = '\0';

	newDomain.m_identifyer = idBuffer;
	m_treeMain->AddNode(newDomain, DomainIdStackRead());
}

void rtuts::RTUTSTreeConstructor::PushPopNotice(const char* notice)
{
	RTUTSNode newDomain = RTUTSNode();

	newDomain.m_runningTestsBellowNotice = new char[strnlen(notice, RTUTS__MAX_STRING_BUFFER_SIZE) + 1];
	memcpy(newDomain.m_runningTestsBellowNotice, notice, sizeof(char) * (strnlen(notice, RTUTS__MAX_STRING_BUFFER_SIZE) + 1));

	m_treeMain->AddNode(newDomain, DomainIdStackRead());
}


void rtuts::RTUTSTreeConstructor::Replant(const char* identifyer)
{
	//kills old tree
	m_treeMain->free();
	DomainIdStackFree();

	m_treeMain->m_rootObjectsLength = 1;

	//make new root node
	unsigned int rootIndex = 0; //it cant be anything else

	RTUTSNode root = RTUTSNode();

	root.m_identifyer = new char[strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 1];
	memcpy(root.m_identifyer, identifyer, sizeof(char) * (strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 1));

	//adds node to tree
	m_treeMain->AddNode(root, rootIndex);
	DomainIdStackPush(rootIndex);

}


void rtuts::RTUTSTreeConstructor::RunTests(void* args)
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
			RTUTSUnitTestSeverityCode resultCode = m_treeMain->m_nodes[i].m_testResults.m_testResultSeverityCode;

			if (resultCode != RTUTSUnitTestSeverityCode::TSCPass) //test failed
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

rtuts::RTUTSDataContainer* rtuts::RTUTSTreeConstructor::GetContainer(void)
{
	return m_treeMain;
}


void rtuts::RTUTSTreeConstructor::DomainIdStackPush(unsigned int item)
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

void rtuts::RTUTSTreeConstructor::DomainIdStackPop()
{
	m_activeDomainStackIndex = m_activeDomainStackIndex - 1;
}

void rtuts::RTUTSTreeConstructor::DomainIdStackFree()
{
	delete[] m_activeDomainStackPointer;
	m_activeDomainStackLength = 0;
	m_activeDomainStackLength = 0;
}

unsigned int rtuts::RTUTSTreeConstructor::DomainIdStackRead()
{
	return m_activeDomainStackPointer[m_activeDomainStackIndex - 1];
}



rtuts::RTUTSListConstructor::RTUTSListConstructor(void)
{
	m_listMain = new RTUTSDataContainer();
}

rtuts::RTUTSListConstructor::~RTUTSListConstructor(void)
{
	m_listMain->free();

	delete m_listMain;
}


void rtuts::RTUTSListConstructor::AddTest(const char* identifyer, RTUTSUnitTest test)
{
	unsigned int newDomainIndex = m_listMain->m_nodesLength;

	RTUTSNode newDomain = RTUTSNode();
	newDomain.m_test = test;

	char type[] = "test";
	char* idBuffer = new char[(strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE)) + 3];

	memcpy(idBuffer, type, strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE));
	idBuffer[strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + 0] = ':';
	idBuffer[strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + 1] = ' ';
	memcpy(idBuffer + (strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + 2), identifyer, strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE));
	idBuffer[strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 2] = '\0';

	newDomain.m_identifyer = idBuffer;
	m_listMain->AddNode(newDomain, newDomainIndex);
	m_listMain->m_rootObjectsLength = m_listMain->m_rootObjectsLength + 1; //so this list prints
}

void rtuts::RTUTSListConstructor::AddNotice(const char* notice)
{
	unsigned int newDomainIndex = m_listMain->m_nodesLength;
	RTUTSNode newDomain = RTUTSNode();

	newDomain.m_runningTestsBellowNotice = new char[strnlen(notice, RTUTS__MAX_STRING_BUFFER_SIZE) + 1];
	memcpy(newDomain.m_runningTestsBellowNotice, notice, sizeof(char) * (strnlen(notice, RTUTS__MAX_STRING_BUFFER_SIZE) + 1));

	m_listMain->AddNode(newDomain, newDomainIndex);
	m_listMain->m_rootObjectsLength = m_listMain->m_rootObjectsLength + 1; //so this list prints
}


void rtuts::RTUTSListConstructor::ClearList(void)
{
	m_listMain->free();
}


void rtuts::RTUTSListConstructor::RunTests(void* args)
{
	for (unsigned int i = 0; i < m_listMain->m_nodesLength; i++)
	{
		if (m_listMain->m_nodes[i].m_runningTestsBellowNotice != nullptr) //output notices
		{
			std::cout << "\033[0;96;49m" << "Unit Test System is running: " << "\033[0m" << m_listMain->m_nodes[i].m_runningTestsBellowNotice << "\n";
		}

		if (m_listMain->m_nodes[i].m_test != nullptr)
		{
			m_listMain->m_nodes[i].m_testResults = m_listMain->m_nodes[i].m_test(args);
		}
	}
}

rtuts::RTUTSDataContainer* rtuts::RTUTSListConstructor::GetContainer(void)
{
	return m_listMain;
}



rtuts::RTUTSBindedListConstructor::RTUTSBindedListConstructor(void)
{
	m_listMain = new RTUTSDataContainer();
	SetListIdenifyer("list root");
}

rtuts::RTUTSBindedListConstructor::RTUTSBindedListConstructor(const char* identifyer)
{
	m_listMain = new RTUTSDataContainer();
	SetListIdenifyer(identifyer);
}

rtuts::RTUTSBindedListConstructor::~RTUTSBindedListConstructor(void)
{
	m_listMain->free();

	delete m_listMain;
}


void rtuts::RTUTSBindedListConstructor::AddTest(const char* identifyer, RTUTSUnitTest test)
{
	RTUTSNode newDomain = RTUTSNode();
	newDomain.m_test = test;

	char type[] = "test";
	char* idBuffer = new char[(strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE)) + 3];

	memcpy(idBuffer, type, strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE));
	idBuffer[strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + 0] = ':';
	idBuffer[strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + 1] = ' ';
	memcpy(idBuffer + (strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + 2), identifyer, strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE));
	idBuffer[strnlen(type, RTUTS__MAX_STRING_BUFFER_SIZE) + strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 2] = '\0';

	newDomain.m_identifyer = idBuffer;
	m_listMain->AddNode(newDomain, 0);
	m_listMain->m_rootObjectsLength = m_listMain->m_rootObjectsLength + 1; //so this list prints
}

void rtuts::RTUTSBindedListConstructor::AddNotice(const char* notice)
{
	RTUTSNode newDomain = RTUTSNode();

	newDomain.m_runningTestsBellowNotice = new char[strnlen(notice, RTUTS__MAX_STRING_BUFFER_SIZE) + 1];
	memcpy(newDomain.m_runningTestsBellowNotice, notice, sizeof(char) * (strnlen(notice, RTUTS__MAX_STRING_BUFFER_SIZE) + 1));

	m_listMain->AddNode(newDomain, 0);
	m_listMain->m_rootObjectsLength = m_listMain->m_rootObjectsLength + 1; //so this list prints
}


void rtuts::RTUTSBindedListConstructor::ClearList(void)
{
	char* titleBuffer = new char[strnlen(m_listMain->m_nodes[0].m_identifyer, RTUTS__MAX_STRING_BUFFER_SIZE)];
	memcpy(titleBuffer, m_listMain->m_nodes[0].m_identifyer, sizeof(char) * (strnlen(m_listMain->m_nodes[0].m_identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 1));

	m_listMain->free();
	SetListIdenifyer(titleBuffer);

	delete titleBuffer;
}

void rtuts::RTUTSBindedListConstructor::SetListIdenifyer(const char* identifyer)
{
	if (m_listMain->m_nodesLength == 0)
	{
		m_listMain->m_rootObjectsAvoidanceLength = 1;
		m_listMain->m_rootObjectsLength = 1;

		RTUTSNode newDomain = RTUTSNode();

		newDomain.m_identifyer = new char[strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 1];
		memcpy(newDomain.m_identifyer, identifyer, sizeof(char) * (strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 1));

		m_listMain->AddNode(newDomain, 0);
	}
	else
	{
		delete[] m_listMain->m_nodes;

		m_listMain->m_nodes[0].m_identifyer = new char[strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 1];
		memcpy(m_listMain->m_nodes[0].m_identifyer, identifyer, sizeof(char) * (strnlen(identifyer, RTUTS__MAX_STRING_BUFFER_SIZE) + 1));
	}
}

void rtuts::RTUTSBindedListConstructor::RunTests(void* args)
{
	for (unsigned int i = 0; i < m_listMain->m_nodesLength; i++)
	{
		if (m_listMain->m_nodes[i].m_runningTestsBellowNotice != nullptr) //output notices
		{
			std::cout << "\033[0;96;49m" << "Unit Test System is running: " << "\033[0m" << m_listMain->m_nodes[i].m_runningTestsBellowNotice << "\n";
		}

		if (m_listMain->m_nodes[i].m_test != nullptr)
		{
			m_listMain->m_nodes[i].m_testResults = m_listMain->m_nodes[i].m_test(args);

			if (((unsigned int)m_listMain->m_nodes[i].m_testResults.m_testResultSeverityCode) > ((unsigned int)m_listMain->m_nodes[m_listMain->m_nodes[i].m_parent].m_testResults.m_testResultSeverityCode))
			{
				m_listMain->m_nodes[m_listMain->m_nodes[i].m_parent].m_testResults.m_testResultSeverityCode = m_listMain->m_nodes[i].m_testResults.m_testResultSeverityCode;
			}
		}
	}
}

rtuts::RTUTSDataContainer* rtuts::RTUTSBindedListConstructor::GetContainer(void)
{
	return m_listMain;
}


void rtuts::ConOutputTestResults(const RTUTSDataContainer* results, ConOutputSettings settings)
{
	std::cout << "\033[0;96;49m" << "\n";
	std::cout << "======================================================================================================" << "\n";
	std::cout << "Run Time Unit Testing System Output" << "\n";
	std::cout << "======================================================================================================" << "\n\n";
	std::cout << "\033[0m";

	int startingDepth = 0;
	unsigned int startingPoint = results->m_rootObjectsAvoidanceLength;
	unsigned int endingPoint = results->m_rootObjectsLength;

	if (settings.m_outputDataFromRootAvoidance == true && results->m_rootObjectsAvoidanceLength > 0)
	{
		startingPoint = 0;
		endingPoint = results->m_rootObjectsAvoidanceLength;
	}

	for (unsigned int indexOfRootNode = startingPoint; indexOfRootNode < endingPoint; indexOfRootNode++)
	{
		ConOutputDomainsAndSubDomains(results, indexOfRootNode, startingDepth, settings); //more than one root is used in lists but other than that it is just one element most of the time
	}

	std::cout << "\033[0;96;49m" << "\n";
	std::cout << "======================================================================================================" << "\n";
	std::cout << "\033[0m";
}

void rtuts::ConOutputTestSeverityCode(RTUTSUnitTestSeverityCode testCode)
{
	switch (testCode)
	{
	case RTUTSUnitTestSeverityCode::TSCPass:
		std::cout << "\033[0;32;102m" << "+";
		break;
	case RTUTSUnitTestSeverityCode::TSCWarning:
		std::cout << "\033[0;33;103m" << "~";
		break;
	case RTUTSUnitTestSeverityCode::TSCDetectionFailed:
		std::cout << "\033[0;30;100m" << "?";
		break;
	case RTUTSUnitTestSeverityCode::TSCTryCatchFail:
		std::cout << "\033[0;30;45m" << "/";
		break;
	case RTUTSUnitTestSeverityCode::TSCFail:
		std::cout << "\033[0;31;101m" << "!";
		break;
	case RTUTSUnitTestSeverityCode::TSCCrashFail:
		std::cout << "\033[0;36;106m" << "*";
		break;
	case TSCMaxNull: default:
		std::cout << "\033[0;95;100m" << "?";
		break;
	}

	std::cout << "\033[0m";
}

void rtuts::ConOutputDomainsAndSubDomains(const RTUTSDataContainer* results, unsigned int domainIndex, unsigned int depth, ConOutputSettings settings)
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

bool rtuts::ExtOutputTestResults(const RTUTSDataContainer* results, ExtOutputSettings settings)
{
	const unsigned int maxCreationDelay = 250;
	const unsigned int maxCreationAttempts = 8;
	const unsigned int numberBufferLength = 8;

	char normalExportLoccation[] = "..\\RTUTSext.dat";
	char inlineExportLoccation[] = "RTUTSext.dat";

	char* fileLocation = normalExportLoccation;

	if (settings.m_projectIsInline == true)
	{
		fileLocation = inlineExportLoccation;
	}

	while (true) //should access file
	{
		if (settings.m_waitForCollection == false) { break; }

		std::ifstream readStream = std::ifstream(fileLocation);

		if (readStream.good() == false) { break; }

		readStream.seekg(0, std::ios::end);
		if (readStream.tellg() <= 2) { break; }
	}

	std::fstream file = std::fstream();
	file.open(fileLocation, std::ios::out | std::ios::in | std::ios::trunc);

	if (file.is_open() == false) { return false; } //something wrong

	file << "{";

	unsigned int startingPoint = results->m_rootObjectsAvoidanceLength;
	unsigned int endingPoint = results->m_rootObjectsLength;

	if (settings.m_outputDataFromRootAvoidance == true && results->m_rootObjectsAvoidanceLength > 0) //finds out what should and shouldnt be printed
	{
		startingPoint = 0;
		endingPoint = results->m_rootObjectsAvoidanceLength;
	}

	char rootObjectsLengthNumBuffer[numberBufferLength] = {};
	std::snprintf(rootObjectsLengthNumBuffer, numberBufferLength, "%d", endingPoint);
	file << "\"rootObjectsLength\": " << rootObjectsLengthNumBuffer << ",";

	file << "\"nodes\": [";

	for (unsigned int currentNodeIndex = startingPoint; currentNodeIndex < results->m_nodesLength; currentNodeIndex++)
	{
		RTUTSNode currentNode = results->m_nodes[currentNodeIndex];
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
		if (currentNodeIndex != results->m_nodesLength - 1) { file << ","; }
	}

	file << "]";
	file << "}";

	file.close();

	return true;
}

void rtuts::WriteStringInLibrary(char** writeTo, const char* stringToWrite)
{
	(*writeTo) = new char[strnlen(stringToWrite, RTUTS__MAX_STRING_EXTENDED_BUFFER_SIZE) + 1];
	memcpy((*writeTo), stringToWrite, sizeof(char) * (strnlen(stringToWrite, RTUTS__MAX_STRING_EXTENDED_BUFFER_SIZE) + 1));
}