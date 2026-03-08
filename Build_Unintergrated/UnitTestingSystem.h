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