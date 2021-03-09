#pragma once

#include "AlphaNode.h"
#include "BetaNode.h"
//#include "TerminalNode.h"
#include "Node.h"

#include "MultiNodeIndexing.h"
#include "SpatialNodeIndexing.h"
#include "WorkingMemory.h"
#include "RTree.h"

#include <list>
#include <algorithm>
#include <thread>

class TemporalThread
{
public:
	TemporalThread();
	void run();
	std::thread runThread(Node* nod);

private:
	Node* executedNode;
};

class ReteNet
{
public:
	static queue<string> rulesQueue;
	static vector<vector<pair<string, string>>> parseConditionOriginal(vector<string> condList);
	static void tokenizeMultiExpCEP(string input, vector<pair<string, string>>& res);

	//node building i guess
	static Node* addAlphaReturnNode(string condition);
	static Node* addBetaReturnNode(string condition);
	static int getCurrNodeListLen();
	//connect 3 nodes
	static void connectNodes(Node& n1, Node& n2, Node& n3);
	//connect 2 nodes
	static void connectNodes(Node& n1, Node& n2);
	static void growTheNodes(vector<vector<pair<string, string>>> exp_vec);
	//static void growCQNodes(vector<)

	//still about nodes
	static void deleteNode(string exp, int identifier = -1);// 0 --> alpha, 1 --> beta
	static void updateNode(string exp_old, string exp_new);

	//Main execution of RETE
	static void ExecuteRete(int TimeSlice, int TimeNow);
	static void buildNetNode();

	//debug purpose
	static void printAllNodes();
	static void printPQNodes();
	static void getAllNodes(vector<string>& alphanodes, vector<string>& betanodes);
	static void pritAllEndNodes();
	static int lastNode();

	//Node type 0 --> alpha, 1-->beta
	static Node* findNode(string expression, int type = -1);
	static Node* getNodeFromId(int id);

	//About Working Memory
	static void cloneToWm(queue<EventPtr> input);
	static void pushToWM(EventPtr ev);
	static int getWMSize();
	static int getCurrentTime();
	static void popFrontWM();

	//About Spatiotemporal special handling
	//static void SpatioTemporalExecution(int TimeSlice, int timeNow);
	static void NewSpatioTemporalExecution(int TimeSlice, int timeNow);
	static void TemporalExecution(int TimeSlice, int timeNow);
	static void ExecuteSingleTemporalNode(int TimeSlice, int TimeNow, Node* n);
	static vector<int> CheckTemporalExecutionID(int TimeSlice, int timeNow);
	static void reInsertTemporalQueue(int finishedId);
	static void checkTemporalQueue(int TimeSlice, int TimeNow);

	//get the number of nodes created:
	static int GetNumberOfNodes();

	//priority queue for different temporal execution
	static struct CustomCompare {
		bool operator()(Node* a, Node* b) {
			if (a->getExecutionEstimated() != b->getExecutionEstimated()) {
				return a->getExecutionEstimated() > b->getExecutionEstimated();
				// this is ascending order --> 1, 2, 3, 4
				// if 3 > 4 --> false, nope
				// if 5 > 4 --> true, perlu dibalik
			}
			else {
				return a->SuperNodeID > b->SuperNodeID;
				//	//string str = a->justCondition;
				//	//string str2 = b->justCondition;

				//	////return 1;
				//	//std::size_t found = str.find(str2);
				//	////if (found != std::string::npos)
				//	//	return found > 0;
				//	////else
				//	////	return 1;
			}
		}
	};
	static vector<Node*> getCopyNodes() {
		return NodeList;
	}
	static priority_queue < Node*, vector<Node*>, CustomCompare> p_queue, temp_p_queue;
	static queue< string > triggered_ev;
	static map<string, Node*> triggered_node;
	/*static void pushTriggeredEvent(string inpEvent) {
		triggered_ev.push_back(inpEvent);
		return;
	}*/
	static std::recursive_mutex mutexOfReteProcess;//mutex lock among threads

private:
	static WorkingMemory m_WMSet;
	static vector<int>alphaListIDDictionary;
	static vector<int>betaListIDDictionary;
	static vector<int>termListIDDictionary;
	static vector<Node*> NodeList;

	//use spatialnodeindexing
	static vector<int> staticCQNodeId;

	//filter the spatial node that we can do indexing
	static vector<Node*> nodewithspatialCondition, distanceNode;
	//list the anchor class
	//vector format --> left input, right input, node id
	static vector<pair<pair<string, string>, int>> vec_anchor_id;
	//map format --> anchor, vector of corresponding obj type
	static unordered_map<string, vector<string>> anchor_stab_map;
	static vector<string> observed_obj_dict;

	//temporal thread
	//std::thread runTemporal();
	//std::thread temporal_thread[999];

};

