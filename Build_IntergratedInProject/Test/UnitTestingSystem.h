
#include <iostream>

#define UTSNEW__MAX_STRING_BUFFER_SIZE 1024
#define UTSNEW_API

namespace uts
{
	/// <summary>
	/// the lower down on the list it is the more important it is
	/// </summary>
	enum UTSNEW_API UTSTestSeverityCode : char
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
	typedef UTSNEW_API UTSTestSeverityCode(*UTSUnitTest)();

	class UTSNEW_API UTSNode
	{
	public:
		char* m_identifyer = nullptr;
		char* m_runNotice = nullptr; ///if set this will output text when its coaspoing test runs and it can also be rendered in the test tree output m_identifyer or test result will not be rendered
		
		unsigned int* m_children = nullptr;
		unsigned int m_childrenLength = 0;

		unsigned int m_parent = 0;

		UTSUnitTest m_test = nullptr;
		UTSTestSeverityCode m_severityCodeOfTest = UTSTestSeverityCode::TSCPass; ///the code the test returned when it was exacuted

		void free(void);
	};

	class UTSNEW_API UTSDataContainer
	{
	public:
		UTSNode* m_nodes = nullptr;
		unsigned int m_nodesLength = 0;

		void free(void); ///use this when you are done with the tree
		void AddNode(UTSNode node, unsigned int parentIndex);
	};

	// This class is exported from the dll
	class UTSNEW_API UTSTreeConstructor {
	public:
		UTSTreeConstructor(void);
		UTSTreeConstructor(const char* rootName);
		~UTSTreeConstructor(void);

		/// <summary>
		/// Adds a new child cabable node/identifyer to the last identifyer created.
		/// </summary>
		/// <param name="identifyer">Node name.</param>
		void PushDomain(const char* identifyer);

		/// <summary>
		/// Adds a new child cabable node/identifyer to the last identifyer created.
		/// </summary>
		/// <param name="type">Object type for example: class, namespace ect.</param>
		/// <param name="identifyer">Node name.</param>
		void PushDomain(const char* type, const char* identifyer);

		/// <summary>
		/// Replace the last identifyer crated with the one created before it and in doing so all new pushed domains will be added to that insted.
		/// </summary>
		void PopDomain();


		/// <summary>
		/// Adds a unit test to the last hoder created. Tests can not have children.
		/// </summary>
		/// <param name="identifyer">Node name.</param>
		/// <param name="test">The unit test function.</param>
		void PushPopTest(const char* identifyer, UTSUnitTest test);

		/// <summary>
		/// Adds a unit test to the last hoder created. Tests can not have children.
		/// </summary>
		void PushPopNotice(const char* notice);


		/// <summary>
		/// Kills the tree and everything in it and then creates a new tree with a root identifyer.
		/// </summary>
		void Replant(const char* identifyer = "root");


		/// <summary>
		/// Runs thougth the tree exacuting every test it finds on the way and storing the result.
		/// </summary>
		void RunTests(void);

		/// <summary>
		/// Get the tree.
		/// </summary>
		/// <returns>The tree.</returns>
		UTSDataContainer* GetContainer(void);

	private:
		void DomainIdStackPush(unsigned int item);
		void DomainIdStackPop();
		void DomainIdStackFree();
		unsigned int DomainIdStackRead();


		unsigned int* m_activeDomainStackPointer = nullptr;
		unsigned int m_activeDomainStackLength = 0; //how big the stack is in genral including poped elements
		unsigned int m_activeDomainStackIndex = 0; //the top of the stack

		UTSDataContainer* m_treeMain = nullptr;
	};



	/// <summary>
	/// Outputs the results to the console.
	/// </summary>
	/// <param name="results">The data from UTSTreeConstructor.RunTests.</param>
	UTSNEW_API void ConOutputTestResults(const UTSDataContainer* results, bool outputRunNotices = false);

	/// <summary>
	/// outputs the charater and color related to a test code into the console
	/// </summary>
	/// <param name="testCode">the test code</param>
	UTSNEW_API void ConOutputTestSeverityCode(UTSTestSeverityCode testCode);

	/// <summary>
	/// you shouldnt use this it is ment to be used by ConOutputTestResults only.
	/// </summary>
	/// <param name="results">The data from UTSTreeConstructor.RunTests.</param>
	/// <param name="domainIndex">The node we are starting from.</param>
	/// <param name="depth">Our depth so far.</param>
	UTSNEW_API void ConOutputDomainsAndSubDomains(const UTSDataContainer* results, unsigned int domainIndex, unsigned int depth, bool outputRunNotices = false);

}