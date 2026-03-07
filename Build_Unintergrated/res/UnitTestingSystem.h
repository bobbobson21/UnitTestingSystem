
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