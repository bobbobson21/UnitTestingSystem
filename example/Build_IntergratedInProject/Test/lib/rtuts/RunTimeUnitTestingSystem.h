
#include <iostream>
#include <fstream>

#define RTRTUTS__MAX_STRING_BUFFER_SIZE 1024
#define RTRTUTS__MAX_STRING_EXTENDED_BUFFER_SIZE 2024

#ifndef _STATIC
#ifdef RTUTS_EXPORTS
#define RTUTS_API __declspec(dllexport)
#else
#define RTUTS_API __declspec(dllimport)
#endif
#endif

#ifdef _STATIC
#define RTUTS_API
#endif

/// <summary>
/// the run time unit testing lib for games
/// </summary>
namespace rtuts
{
	/// <summary>
	/// The lower down on the list it is the more important it is.
	/// </summary>
	enum RTUTS_API RTUTSUnitTestSeverityCode : char
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
	/// Stores all the test results from the unit test.
	/// </summary>
	class RTUTS_API RTUTSUnitTestResults
	{
	public:
		RTUTSUnitTestSeverityCode m_testResultSeverityCode = RTUTSUnitTestSeverityCode::TSCPass; ///the code the test returned when it was exacuted.
		char* m_testResultDescriptionBuffer = nullptr; ///DO NOT ACCESS DIRECTLY, ACCESS THOUGTH WriteStringSafe
		
		char* m_unitTestFileLocationFromRoot = nullptr; ///DO NOT ACCESS DIRECTLY, ACCESS THOUGTH WriteStringSafe
		char* m_unitTestfileLocationRelative = nullptr; ///DO NOT ACCESS DIRECTLY, ACCESS THOUGTH WriteStringSafe
		
		char* m_objectFileLocationFromRoot = nullptr; ///DO NOT ACCESS DIRECTLY,  ACCESS THOUGTH WriteStringSafe
		char* m_objectFileLocationRelative = nullptr; ///DO NOT ACCESS DIRECTLY, ACCESS THOUGTH WriteStringSafe

		/// <summary>
		/// use this to add descriptions and file location also file paths neeed 4 slashes as folder separators. This also can not surpass RTRTUTS__MAX_STRING_EXTENDED_BUFFER_SIZE in length
		/// </summary>
		/// <param name="writeTo">writeTo: A pointer to a char variable that you whish to write to which can be passed using the refance oparator.</param>
		/// <param name="stringToWrite">stringToWrite: The data you wish to safely write to that buffer.</param>
		void WriteStringSafe(char** writeTo, const char* stringToWrite);
		void free(void);
	};

	/// <summary>
	/// A function that returns a severity code TSCPass for good anything else for bad.
	/// It also contains one pramater the first one is an object that can be passed to all the tests thougth the run test function.
	/// </summary>
	typedef RTUTS_API RTUTSUnitTestResults(*RTUTSUnitTest)(void* args);

	/// <summary>
	/// The base uts object for storing test, names and results, and parent chhild links.
	/// </summary>
	class RTUTS_API RTUTSNode
	{
	public:
		char* m_identifyer = nullptr;
		char* m_runningTestsBellowNotice = nullptr;  ///If set this will output text but it will not be able to do anything else including actually running its test.

		unsigned int m_parent = 0; ///A parent is always required but if it parents its self then it will function as if there is no parent.
		unsigned int* m_children = nullptr;
		unsigned int m_childrenLength = 0;

		RTUTSUnitTest m_test = nullptr;
		RTUTSUnitTestResults m_testResults = RTUTSUnitTestResults();

		void free(void);
	};

	/// <summary>
	/// Can contain trees, lists, and other thing inbetween. 
	/// </summary>
	class RTUTS_API RTUTSDataContainer
	{
	public:
		RTUTSNode* m_nodes = nullptr; //the nodes as an array
		unsigned int m_nodesLength = 0; //the length of the node array
		unsigned int m_rootObjectsLength = 0; //the lenth of all the root node
		unsigned int m_rootObjectsAvoidanceLength = 0; //the length of the root nodes to avoid printing

		void free(void); ///use this when you are done with the tree
		void AddNode(RTUTSNode node, unsigned int parentIndex);
	};


	/// <summary>
	/// Creates a tree base repasentation of the tests and of the issues and it will also back propagate issues up the tree if they are more sever than the parents issue.
	/// </summary>
	class RTUTS_API RTUTSTreeConstructor {
	public:
		RTUTSTreeConstructor& operator=(const RTUTSTreeConstructor& t) = delete;

		RTUTSTreeConstructor(void);
		RTUTSTreeConstructor(const char* rootName);
		~RTUTSTreeConstructor(void);

		/// <summary>
		/// Adds a new child cabable node/identifyer to the last identifyer created.
		/// </summary>
		/// <param name="identifyer">identifyer: The node name which can not be greater than RTRTUTS__MAX_STRING_BUFFER_SIZE.</param>
		void PushDomain(const char* identifyer);

		/// <summary>
		/// Adds a new child cabable node/identifyer to the last identifyer created.
		/// </summary>
		/// <param name="type">type: the object type as a string for example: class, namespace ect. This string can not be greater than RTRTUTS__MAX_STRING_BUFFER_SIZE</param>
		/// <param name="identifyer">identifyer: The node name which can not be greater than RTRTUTS__MAX_STRING_BUFFER_SIZE.</param>
		void PushDomain(const char* type, const char* identifyer);

		/// <summary>
		/// Replace the last identifyer crated with the one created before it and in doing so all new pushed domains will be added to that insted.
		/// </summary>
		void PopDomain();


		/// <summary>
		/// Adds a unit test to the last hoder created. Tests can not have children.
		/// </summary>
		/// <param name="identifyer">identifyer: The node name which can not be greater than RTRTUTS__MAX_STRING_BUFFER_SIZE.</param>
		/// <param name="test">test: A function pointer that returns RTUTSUnitTestResults and containes an argument of type void*.</param>
		void PushPopTest(const char* identifyer, RTUTSUnitTest test);

		/// <summary>
		/// Adds a unit test to the last hoder created. Tests can not have children.
		/// </summary>
		/// <param name="notice">notice: The notice message that is printed to console when exacuted by RunTests. Can not be greater than RTRTUTS__MAX_STRING_BUFFER_SIZE.</param>
		void PushPopNotice(const char* notice);


		/// <summary>
		/// Kills the tree and everything in it and then creates a new tree with a root node that uses the provided identifyer.
		/// </summary>
		/// <param name="identifyer">identifyer: The name of the new root node which can not be greater than RTRTUTS__MAX_STRING_BUFFER_SIZE.</param>
		void Replant(const char* identifyer = "tree root");


		/// <summary>
		/// Runs thougth the tree exacuting every test it finds on the way and storing the result.
		/// </summary>
		/// <param name="args">args: A pointer to a structer or class containing the arguments/members you wish to pass to all the tests.</param>
		void RunTests(void* args);

		/// <summary>
		/// Get the tree.
		/// </summary>
		/// <returns>The tree.</returns>
		RTUTSDataContainer* GetContainer(void);

	private:
		void DomainIdStackPush(unsigned int item);
		void DomainIdStackPop();
		void DomainIdStackFree();
		unsigned int DomainIdStackRead();


		unsigned int* m_activeDomainStackPointer = nullptr;
		unsigned int m_activeDomainStackLength = 0; //How big the stack is in genral including poped elements.
		unsigned int m_activeDomainStackIndex = 0; //The top of the stack.

		RTUTSDataContainer* m_treeMain = nullptr;
	};

	/// <summary>
	/// creates a list base repasentation of tests, notices and the issues
	/// </summary>
	class RTUTS_API RTUTSListConstructor
	{
	public:
		RTUTSListConstructor& operator=(const RTUTSListConstructor& t) = delete;

		RTUTSListConstructor(void);
		~RTUTSListConstructor(void);

		/// <summary>
		/// adds a test to the list
		/// </summary>
		/// <param name="identifyer">identifyer: The name of the test being done and this name can not be greater than RTRTUTS__MAX_STRING_BUFFER_SIZE.</param>
		/// <param name="test">test: A function that returns RTUTSUnitTestResults and takes in a value of type void*.</param>
		void AddTest(const char* identifyer, RTUTSUnitTest test);

		/// <summary>
		/// adds a notice
		/// </summary>
		/// <param name="notice">notice: Notice name string also the string can not be grater than RTRTUTS__MAX_STRING_BUFFER_SIZE.</param>
		void AddNotice(const char* notice);


		/// <summary>
		/// Wipes the list.
		/// </summary>
		void ClearList(void);


		/// <summary>
		/// Runs thougth the tree exacuting every test it finds on the way and storing the result.
		/// </summary>
		/// <param name="args">args: A pointer to a structer or class containing the arguments/members you wish to pass to all the tests</param>
		void RunTests(void* args);

		/// <summary>
		/// Gets the data out of this object in a format that is compatible with ConOutputTestResults and ExtOutputTestResults.
		/// </summary>
		/// <returns>The list.</returns>
		RTUTSDataContainer* GetContainer(void);
	private:
		RTUTSDataContainer* m_listMain = nullptr;
	};

	/// <summary>
	/// creates a list base repasentation of tests, notices and the issues but with the list being done in one singular domain
	/// </summary>
	class RTUTS_API RTUTSBindedListConstructor
	{
	public:
		RTUTSBindedListConstructor& operator=(const RTUTSBindedListConstructor& t) = delete;

		RTUTSBindedListConstructor(void);
		RTUTSBindedListConstructor(const char* identifyer);
		~RTUTSBindedListConstructor(void);

		/// <summary>
		/// adds a test to the list
		/// </summary>
		/// <param name="identifyer">identifyer: The name of the test being done and this name can not be greater than RTRTUTS__MAX_STRING_BUFFER_SIZE.</param>
		/// <param name="test">test: A function that returns RTUTSUnitTestResults and takes in a value of type void*.</param>
		void AddTest(const char* identifyer, RTUTSUnitTest test);

		/// <summary>
		/// adds a notice
		/// </summary>
		/// <param name="notice">notice: Notice name string also the string can not be grater than RTRTUTS__MAX_STRING_BUFFER_SIZE.</param>
		void AddNotice(const char* notice);


		/// <summary>
		/// Wipes the list.
		/// </summary>
		void ClearList(void);

		/// <summary>
		/// sets the title of list
		/// </summary>
		/// <param name="identifyer">the new title of the list and this con not exxced RTRTUTS__MAX_STRING_BUFFER_SIZE</param>
		void SetListIdenifyer(const char* identifyer = "list root");

		/// <summary>
		/// Runs thougth the tree exacuting every test it finds on the way and storing the result.
		/// </summary>
		/// <param name="args">args: A pointer to a structer or class containing the arguments/members you wish to pass to all the tests</param>
		void RunTests(void* args);

		/// <summary>
		/// Gets the data out of this object in a format that is compatible with ConOutputTestResults and ExtOutputTestResults.
		/// </summary>
		/// <returns>The list.</returns>
		RTUTSDataContainer* GetContainer(void);
	private:
		RTUTSDataContainer* m_listMain = nullptr;
	};

	/// <summary>
	/// Con out settings.
	/// </summary>
	class RTUTS_API ConOutputSettings
	{
	public:
		bool m_outputRunNotices = false; ///Outputs the notices as tree/list elements.
		bool m_outputFailureDesriptions = false; ///Outputs descriptions.

		bool m_outputDataFromRootAvoidance = false; ///if true the root of the sturucter that is to be avoided will be printed insted and nothing else will be apart from that areas children
	};

	/// <summary>
	/// Output to external program settings.
	/// </summary>
	class RTUTS_API ExtOutputSettings
	{
	public:
		bool m_waitForCollection = true; ///If there are alread outputed results wait for them to be collected first.
		bool m_projectIsInline = false; ///If true the issue dump will not be exporte to the parent folder of the create file location isted it will only exsit in the create file location.
	
		bool m_outputDataFromRootAvoidance = true;
	};


	/// <summary>
	/// Outputs the results to the console.
	/// </summary>
	/// <param name="results">results: The data from RTUTSTreeConstructor.GetContainer.</param>
	RTUTS_API void ConOutputTestResults(const RTUTSDataContainer* results, ConOutputSettings settings);

	/// <summary>
	/// Outputs the charater and color related to a test code into the console.
	/// </summary>
	/// <param name="testCode">testCode: the test code.</param>
	RTUTS_API void ConOutputTestSeverityCode(RTUTSUnitTestSeverityCode testCode);

	/// <summary>
	/// you shouldnt use this it is ment to be used by ConOutputTestResults only.
	/// </summary>
	/// <param name="results">results: The data from RTUTSTreeConstructor.GetContainer.</param>
	/// <param name="domainIndex">domainIndex: The node we are starting from.</param>
	/// <param name="depth">depth: Our depth so far.</param>
	/// <param name="settings">settings: what should be oputed to the console.</param>
	RTUTS_API void ConOutputDomainsAndSubDomains(const RTUTSDataContainer* results, unsigned int domainIndex, unsigned int depth, ConOutputSettings settings);

	/// <summary>
	/// outputs the results to an extenrnal debuging program
	/// </summary>
	/// <param name="results">results: The data from RTUTSTreeConstructor.GetContainer.</param>
	/// <param name="settings">settings: how should the export be configured settings.</param>
	/// <returns>was sucessful at doing this</returns>
	RTUTS_API bool ExtOutputTestResults(const RTUTSDataContainer* results, ExtOutputSettings settings);
}

