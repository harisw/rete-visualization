#include "ReteNet.h"

#define INDEX_ON
//#define INDEX_OFF
//#define NO_THREADING
//#define SHOW_BUILT_PROGRESS
//#define SHOW_LAST_NODE_LIST

#define USE_SECOND
//#define USE_MILLISEC

//#define SHOW_P_QUEUE
///#define STATIC_CQ

WorkingMemory ReteNet::m_WMSet;

vector<int>ReteNet::alphaListIDDictionary;
vector<int>ReteNet::betaListIDDictionary;
vector<int>ReteNet::termListIDDictionary;
vector<Node*> ReteNet::NodeList;

queue< string > ReteNet::triggered_ev;
map<string, Node*> ReteNet::triggered_node;

//this is spatial node indexing
vector<int> ReteNet::staticCQNodeId;

//this is spatio temporal
vector<Node*> ReteNet::nodewithspatialCondition;
vector<Node*> ReteNet::distanceNode;
vector<pair<pair<string, string>, int>> ReteNet::vec_anchor_id;
unordered_map<string, vector<string>> ReteNet::anchor_stab_map;
vector<string> ReteNet::observed_obj_dict;

//priority_queue < Node*, vector<Node*>, decltype(&ReteNet::Compare_pq)> p_queue;
priority_queue < Node*, vector<Node*>, ReteNet::CustomCompare> ReteNet::p_queue;
priority_queue < Node*, vector<Node*>, ReteNet::CustomCompare> ReteNet::temp_p_queue;

//incoming newcomer, a mutex
std::recursive_mutex ReteNet::mutexOfReteProcess;

queue<string> ReteNet::rulesQueue;

vector<vector<pair<string, string>>> ReteNet::parseConditionOriginal(vector<string> condList)
{
	string tempRule = "";
	for (int j = 0; j < condList.size(); j++)
		tempRule += condList[j] + " ";
	
	rulesQueue.push(tempRule);
	//Parsing parsing~
	vector<vector<pair<string, string>>> collectedMade;
	vector<string> newInputForm;

	for (auto v : condList) {
		newInputForm.push_back(v);
	}

	for (string s : newInputForm) {
		vector<pair<string, string>> SingleExpressionMade = {};

		s = Utilities::ToLower(Utilities::trim(s));
		if (s.size() == 0 || s[0] == '#') continue;

		size_t index = s.find_first_of(" ", 0);
		if (index < 0) {
			std::cout << "no clause";
			throw runtime_error("no clause");
		}

		string clause = s.substr(0, index);
		string value = s.substr(index + 1, s.size() - index - 1);
		Utilities::deleteAllMark(value, " ");

		if ("if" == clause) {
			tokenizeMultiExpCEP(value, SingleExpressionMade);
		}
		else if ("window" == clause) {//Window length=, sliding=

			vector<string> temp_string = Utilities::splitDelim(value, ",");

			for (int i = 0; i < temp_string.size(); i++) {
				vector<string> another_string = Utilities::splitDelim(temp_string[i], "=");
				SingleExpressionMade.push_back({ another_string[0],another_string[1] });
			}
		}
		else if ("then" == clause) {//then = 

			vector<string> temp_string = Utilities::splitDelim(value, ",");

			for (int i = 0; i < temp_string.size(); i++) {
				if (i == 0)
					SingleExpressionMade.push_back({ "term", temp_string[i] });
				else
					SingleExpressionMade.push_back({ "aggr", temp_string[i] });
			}
		}

		collectedMade.push_back(SingleExpressionMade);
	}
	int a;

	return collectedMade;
}

void ReteNet::getAllNodes(vector<string>& alphanodes, vector<string>& betanodes)
{
	for (auto i : alphaListIDDictionary) {
		alphanodes.push_back(NodeList[i]->justCondition);
	}

	for (auto i : betaListIDDictionary) {
		betanodes.push_back(NodeList[i]->justCondition);
	}
}

void ReteNet::tokenizeMultiExpCEP(string input, vector<pair<string, string>>& res)
{
	vector<string> expressionList = Utilities::splitDelim(input, "&");
	if (expressionList.size() == 1) {
		int coma = expressionList[0].find(","); //so far, this is the decision maker whether it is beta or alpha
		int dot = expressionList[0].find("."); //oh, if it is an alpha node, define where do they get it

		if (dot > -1) {
			res.push_back({ "alpha", expressionList[0] });
		}
		else if (coma > -1) {
			res.push_back({ "beta", expressionList[0] });
		}
		else {
			res.push_back({ "alpha", expressionList[0] });
		}
	}
	else if (expressionList.size() > 0) {
		for (string expression : expressionList) {

			int coma = expression.find(","); //so far, this is the decision maker whether it is beta or alpha
			int dot = expression.find("."); //oh, if it is an alpha node, define where do they get it

			string comparisonExp = Utilities::getComparisonExp(expression);

			if (dot > -1) { //continous alpha --> ally.speed
				res.push_back({ "alpha", expression });
			}
			else if (coma > -1) {//consist more than 1 source --> distance(x,y)
				res.push_back({ "beta", expression });
			}
			else { //just from soooo, it is beta node
				if (comparisonExp != "") { //-->speed < 100 
					res.push_back({ "alpha", expression });
				}
				else// just string --> allyObj or distance rule
					res.push_back({ "beta", expression });
			}

			res.push_back({ "condition", "and" });
		}

		res.erase(res.end() - 1);
	}
}

Node* ReteNet::addAlphaReturnNode(string condition)
{
	Node* tempNode;
	//string space = " ";
	if (condition.back() == ' ') {
		condition.pop_back();
	}

	//duplicate check
	for (int i = 0; i < alphaListIDDictionary.size(); i++) {
		if (NodeList[alphaListIDDictionary[i]]->justCondition == condition)
			return NodeList[alphaListIDDictionary[i]];
	}

	//Try to use OOP
	int masterNodeID = NodeList.size();
	tempNode = new AlphaNode(masterNodeID, condition);
	NodeList.push_back(tempNode);
	alphaListIDDictionary.push_back(masterNodeID);

	return tempNode;
}

Node* ReteNet::addBetaReturnNode(string condition)
{
	if (condition.back() == ' ') {
		condition.pop_back();
	}

	Node* tempNode;
	//duplicate check
	for (int i = 0; i < betaListIDDictionary.size(); i++) {
		if (static_cast<BetaNode*>(NodeList[betaListIDDictionary[i]])->getProduct() == condition)
			return NodeList[betaListIDDictionary[i]];
	}

	//Try to use OOP
	tempNode = new BetaNode(0, condition);

	int masterNodeID = NodeList.size();
	tempNode->setNewMasterID(masterNodeID);
	NodeList.push_back(tempNode);

	betaListIDDictionary.push_back(masterNodeID);

	return tempNode;
}

int ReteNet::getCurrNodeListLen()
{
	return NodeList.size();
}

void ReteNet::connectNodes(Node& n1, Node& n2, Node& n3)
{
	int danger = 0;
	bool left = true, right = true;
	if (static_cast<BetaNode*>(&n3)->getLeftConnNode() != NULL && &n1 != NULL) {
		danger++;
		cout << "ALERT, LEFT OF :" << static_cast<BetaNode*>(&n3)->getProduct() << " IS ALREADY OCCUPIED SOURCE" << endl;
	}
	if (static_cast<BetaNode*>(&n3)->getRightConnNode() != NULL && &n2 != NULL) {
		danger++;
		cout << "ALERT, RIGHT OF :" << static_cast<BetaNode*>(&n3)->getProduct() << " IS ALREADY OCCUPIED SOURCE" << endl;
	}

	if (danger >= 2) {
		cout << "ALERT, BOTH OF :" << static_cast<BetaNode*>(&n3)->getProduct() << " NO CONNECTION CREATED" << endl;
		return;
	}

	if (&n1 != NULL) {
		n1.addBetaPair(static_cast<BetaNode*>(&n3));
	}
	if (&n2 != NULL) {
		n2.addBetaPair(static_cast<BetaNode*>(&n3));
	}

	static_cast<BetaNode*>(&n3)->setLeftConnection(&n1);
	static_cast<BetaNode*>(&n3)->setRightConnection(&n2);
}

void ReteNet::connectNodes(Node& n1, Node& n2)
{
	if (static_cast<AlphaNode*>(&n2)->getPrevPair() != NULL) {
		cout << "ALERT :" << static_cast<AlphaNode*>(&n2)->getCondition() << " IS ALREADY OCCUPIED SOURCE" << endl;
		return;
	}

	n1.addBetaPair(&n2);

	static_cast<AlphaNode*>(&n2)->setPrevConnection(&n1);
}

void ReteNet::growTheNodes(vector<vector<pair<string, string>>> exp_vec)
{
	vector<Node*> created_node;
	vector<pair<Node*, string>> compiled_expression;
	vector<string> joinExpression, BetaExpression;

	Node* tempNode;
	int range = -1, step = -1;
	for (int i = 0; i < exp_vec.size(); i++) {

		//If clause
		if (i == 0) {

			//Step 1 --> list the created nodes
			for (int j = 0; j < exp_vec[i].size(); j++) {
				if (exp_vec[i][j].first == "alpha") {
					tempNode = addAlphaReturnNode(exp_vec[i][j].second);
					//created_node.push_back(tempNode);
					compiled_expression.push_back({ tempNode , tempNode->justCondition });
				}
				else if (exp_vec[i][j].first == "beta") {
					//BetaExpression.push_back(exp_vec[i][j].second);
					compiled_expression.push_back({ NULL , exp_vec[i][j].second });
				}
				else if (exp_vec[i][j].first == "condition") {
					joinExpression.push_back(exp_vec[i][j].second);
				}
			}
		}
		else { // can be window or then clause

			//Keep the window val
			if (exp_vec[i][0].first == "range") {
				int index, index1;
				//index = exp_vec[i][0].second.find("range");
				//index1 = exp_vec[i][0].second.find("trigger");

				//range = atoi(exp_vec[i][0].second.substr(index + 6, exp_vec[i][0].second.size() - index -6).c_str());

				range = atoi(exp_vec[i][0].second.c_str());
				step = atoi(exp_vec[i][1].second.c_str());

			}
			else if (exp_vec[i][0].first == "term") {
				//Step2 --> join all of them until the last beta --> then clause

				//Process the original input
				//Due to vector limitation, so we do it backwards --> there are no pop_front, and i didn't want to create it
				;				string tempBetaProduct, tempBetaCondition;
				Node* n1, * n2, * n3;
				while (compiled_expression.size() > 2) {
					//so in temporary beta condition, it has to be flipped again
					tempBetaCondition = "", tempBetaProduct = "";

					char buff[10];

					if (created_node.size() > 0) { // if there are previous node created
						n2 = created_node[0];
						tempBetaCondition = dynamic_cast<BetaNode*>(created_node[0])->getProduct() + _itoa(created_node[0]->getID(), buff, 10);
						tempBetaProduct += dynamic_cast<BetaNode*>(created_node[0])->getProduct().substr(0, 2) + _itoa(created_node[0]->getID(), buff, 10);
						created_node.pop_back();

						tempBetaCondition = " " + joinExpression[joinExpression.size() - 1] + " " + tempBetaCondition;
						joinExpression.pop_back();

						n1 = compiled_expression[compiled_expression.size() - 1].first;
						tempBetaCondition = compiled_expression[compiled_expression.size() - 1].second + tempBetaCondition;
						tempBetaProduct += compiled_expression[compiled_expression.size() - 1].second.substr(0, 2);
						compiled_expression.pop_back();

						n3 = addBetaReturnNode(tempBetaCondition + " then " + tempBetaProduct);

						connectNodes(*n1, *n2, *n3);

						if (range > 0 || step > 0) {
							if (n1 != NULL)
								(n1)->setWindow(range, step);
							if (n2 != NULL)
								(n2)->setWindow(range, step);
							if (n2 != NULL)
								(n3)->setWindow(range, step);
						}

						created_node.push_back(n3);
					}
					else { // if there are no node created before
						n2 = compiled_expression[compiled_expression.size() - 1].first;
						tempBetaCondition = compiled_expression[compiled_expression.size() - 1].second;
						tempBetaProduct += compiled_expression[compiled_expression.size() - 1].second.substr(0, 2);

						tempBetaCondition = " " + joinExpression[joinExpression.size() - 1] + " " + tempBetaCondition;

						compiled_expression.pop_back();
						joinExpression.pop_back();

						n1 = compiled_expression[compiled_expression.size() - 1].first;
						tempBetaCondition = compiled_expression[compiled_expression.size() - 1].second + tempBetaCondition;
						tempBetaProduct += compiled_expression[compiled_expression.size() - 1].second.substr(0, 2);
						compiled_expression.pop_back();

						//add additional id marker so it wont go away
						string combineId = to_string(n1->getID());
						combineId += to_string(n2->getID());
						n3 = addBetaReturnNode(tempBetaCondition + " then " + tempBetaProduct + combineId);

						connectNodes(*n1, *n2, *n3);

						if (range > 0 || step > 0) {
							if (n1 != NULL)
								(n1)->setWindow(range, step);
							if (n2 != NULL)
								(n2)->setWindow(range, step);
							if (n2 != NULL)
								(n3)->setWindow(range, step);
						}

						created_node.push_back(n3);
					}
				}

				//FINALIZE THE LAST BETA
				tempBetaCondition = "", tempBetaProduct = "";
				if (created_node.size() > 0 && compiled_expression.size() > 0) { //there is previous beta created
					n2 = created_node[0];
					tempBetaCondition += dynamic_cast<BetaNode*>(created_node[0])->getProduct();
					created_node.pop_back();

					tempBetaCondition = " " + joinExpression[joinExpression.size() - 1] + " " + tempBetaCondition;
					joinExpression.pop_back();

					n1 = compiled_expression[compiled_expression.size() - 1].first;
					tempBetaCondition = compiled_expression[compiled_expression.size() - 1].second + tempBetaCondition;
					compiled_expression.pop_back();

					n3 = addBetaReturnNode(tempBetaCondition + " then " + exp_vec[i][0].second);

					if (range > 0 || step > 0) {
						if (n1 != NULL)
							(n1)->setWindow(range, step);
						if (n2 != NULL)
							(n2)->setWindow(range, step);
						if (n2 != NULL)
							(n3)->setWindow(range, step);
					}

					//add additional aggregate if exist
					if (exp_vec[i].size() > 1) {
						static_cast<BetaNode*>(n3)->setAggregateCondition(exp_vec[i][1].second);
					}

					connectNodes(*n1, *n2, *n3);
				}
				else { // no beta node created or this expression is single expression
					/*if (compiled_expression.size() == 0 && created_node.size() == 1) {
						break;
					}*/

					if (compiled_expression.size() > 1) {
						n2 = compiled_expression[compiled_expression.size() - 1].first;
						tempBetaCondition = compiled_expression[compiled_expression.size() - 1].second;
						tempBetaProduct += compiled_expression[compiled_expression.size() - 1].second.substr(0, 2);
						compiled_expression.pop_back();

						tempBetaCondition = " " + joinExpression[joinExpression.size() - 1] + " " + tempBetaCondition;
						joinExpression.pop_back();
					}
					else {
						n2 = compiled_expression[0].first;
						tempBetaCondition = compiled_expression[0].second;

						tempBetaCondition = " and " + tempBetaCondition;
					}

					n1 = compiled_expression[0].first;
					tempBetaCondition = compiled_expression[0].second + tempBetaCondition;
					compiled_expression.pop_back();

					n3 = addBetaReturnNode(tempBetaCondition + " then " + exp_vec[i][0].second);
					if (range > 0 || step > 0) {
						if (n1 != NULL)
							(n1)->setWindow(range, step);
						if (n2 != NULL)
							(n2)->setWindow(range, step);
						if (n2 != NULL)
							(n3)->setWindow(range, step);
					}

					//add additional aggregate if exist
					if (exp_vec[i].size() > 1) {
						static_cast<BetaNode*>(n3)->setAggregateCondition(exp_vec[i][1].second);
					}

					connectNodes(*n1, *n2, *n3);
				}
			}
		}
	}

#ifdef SHOW_BUILT_PROGRESS
	printAllNodes();
#endif // SHOW_BUILT_PROGRESS

	int a = 11;
	//End of function
}

void ReteNet::deleteNode(string exp, int identifier)
{
	//find the node
	Node* tmp;
	if (identifier == -1) {
		tmp = findNode(exp, identifier);
	}
	else {
		tmp = findNode(exp, 0);
		if (tmp == NULL) {
			tmp = findNode(exp, 1);
		}
	}

	if (tmp == NULL)
		return;

	int node_id = tmp->getID();
	if (dynamic_cast<AlphaNode*>(tmp)) {
		//delete from main
		auto position = find(NodeList.begin(), NodeList.end(), tmp);
		*position = NULL;

		//delete from dictionary
		auto position_id = find(alphaListIDDictionary.begin(), alphaListIDDictionary.end(), node_id);
		alphaListIDDictionary.erase(position_id);
	}
	else {
		//delete from main
		auto position = find(NodeList.begin(), NodeList.end(), tmp);
		//NodeList.erase(position);
		*position = NULL;

		//delete from dictionary
		auto position_id = find(betaListIDDictionary.begin(), betaListIDDictionary.end(), node_id);
		betaListIDDictionary.erase(position_id);
	}
}

void ReteNet::ExecuteRete(int TimeSlice, int TimeNow)
{
	std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
#ifdef INDEX_ON
	//Step 0, make sure that the event is exist
	//if (m_WMSet.getWMInputQueue().size() < 1)
	//	return;

#pragma region Indexed
	//unordered_map<int, int> pushed_node_map;
	int itt = 0;

	//cout << "safe working memory size: " << m_WMSet.size() << endl;
	while (itt < TimeSlice && m_WMSet.size() > 0) {
		//while (m_WMSet.size() > 0) {
#pragma region Old school stab and proceed
		//vector<int> stabbed;
		//stabbed = MultiNodeIndexing::stabEvent(m_WMSet.getFront());

		////examine proceeding node
		//for (auto s : stabbed) {
		//	for (auto itt_pair : static_cast<AlphaNode*>(NodeList[s])->getAllPairs()) {
		//		static_cast<BetaNode*>(itt_pair)->forcePushInQueue(&m_WMSet.getFront(), NodeList[s]);

		//		//duplicate check
		//		std::unordered_map<int, int>::const_iterator got = pushed_node_map.find(itt_pair->getID());
		//		if (got == pushed_node_map.end()) {
		//			//std::cout << "not found";
		//			pushedNode.push_back(itt_pair);
		//			pushed_node_map[itt_pair->getID()] = itt_pair->getID();
		//		}
		//	}
		//}
#pragma endregion

#pragma region combination mathcing

		//empty?, just quit
		if (m_WMSet.size() < 1)
			return;

		//delete broken event
		if (m_WMSet.getFront()->map_size() < 0) {
			m_WMSet.pop();
			continue;
		}

		//there is faster solution, which contain combination ._.
		int finalNodeId = MultiNodeIndexing::returnFinalNode(m_WMSet.getFront());

		//force push to event result if it need to be kept. final node must be beta node tho ._.
		if (finalNodeId != -1) {
			static_cast<BetaNode*>(NodeList[finalNodeId])->forcePushInEv(m_WMSet.getFront());
		}
#pragma endregion
		m_WMSet.pop();
		//		cout << "safe working memory size after pop: " << m_WMSet.size() << endl;
		itt++;
	}
#pragma endregion

#endif // INDEX_ON



#ifdef INDEX_OFF

	bool isDone = false;
	vector<pair<Node*, int>> visitedMark;
	vector<Node*> pushedNode;

#pragma region NoIndexing but works
	//Step 1 Activate all direct alpha
	for (int j = 0; j < alphaListIDDictionary.size(); j++) {

		//test on alpha
		if (static_cast<AlphaNode*>(NodeList[alphaListIDDictionary[j]])->ItIsDirect())
			static_cast<AlphaNode*>(NodeList[alphaListIDDictionary[j]])->testAlphaAndPush(&m_WMSet.getWMInputQueue(), TimeSlice); // original
			//static_cast<AlphaNode*>(NodeList[alphaListIDDictionary[j]])->testAlphaAndPush(&m_WMSet.getWMInputQueue().front(), TimeSlice); // testing purpose
		else
			continue;

		//activate the beta
		//searching all pair and push it
		for (int k = 0; k < static_cast<AlphaNode*>(NodeList[alphaListIDDictionary[j]])->getAllPairs().size(); k++) {

			//check for duplicate
			bool isADuplicate = false;
			for (int l = 0; l < pushedNode.size(); l++) {
				if (pushedNode[l] == static_cast<AlphaNode*>(NodeList[alphaListIDDictionary[j]])->getSinglePair(k)) {
					isADuplicate = true;
					break;
				}
			}
			if (!isADuplicate) {
				pushedNode.push_back(static_cast<AlphaNode*>(NodeList[alphaListIDDictionary[j]])->getSinglePair(k));
				visitedMark.push_back({ static_cast<AlphaNode*>(NodeList[alphaListIDDictionary[j]])->getSinglePair(k), 0 });
			}
		}
	}
	//once it is cleared, dont forget to clear the WM
#pragma endregion

	//BFS based on Queue
	while (!pushedNode.empty()) {

		//This is the borderline ----------------------------------------------------------------------------
		//let the spatial rule processed outside
		if (dynamic_cast<BetaNode*>(pushedNode[0])) {
			//if (dynamic_cast<BetaNode*>(pushedNode[0])->getSpecialOpName() != "" || pushedNode[0]->getExecutionEstimated() > 0) {
			if (pushedNode[0]->getExecutionEstimated() > 0) {
				pushedNode.erase(pushedNode.begin());
				continue;
			}
		}
		//---------------------------------------------------------------------------------------------------

		//Maybe this is the indexing?
		//int testStatus = pushedNode[0]->testNode(TimeSlice);
		int testStatus = pushedNode[0]->testNode(TimeSlice, TimeNow);

		if (testStatus == 0) {
			pushedNode.erase(pushedNode.begin());
			continue;
		}

		//#ifdef INDEX_OFF
				//pushedNode[0]->testNode(TimeSlice);
		pushedNode[0]->testNode(TimeSlice, TimeNow);
		//#endif // INDEX_OFF

				//Activate successor node	
		for (int i = 0; i < pushedNode[0]->getAllPairs().size(); i++) {

			//Check to avoid duplication
			bool isDuplicate = false;
			for (int j = 0; j < pushedNode.size(); j++) {
				if (pushedNode[0]->getSinglePair(i) == pushedNode[j]) {
					isDuplicate = true;
					break;
				}
			}
			if (!isDuplicate)
				pushedNode.push_back(pushedNode[0]->getSinglePair(i));
		}

		//Mark the current node in visited mark
		for (int i = 0; i < visitedMark.size(); i++) {
			if (visitedMark[i].first == pushedNode[0]) {
				visitedMark[i].second++;

				//Evaluate visited mark
				if (visitedMark[i].second >= 3)
					isDone = true;
				break;
			}
		}

		pushedNode.erase(pushedNode.begin());

		if (isDone)
			break;
	}

#endif // INDEX_OFF
	//lg.~lock_guard();
	//ReteNet::checkTemporalQueue(TimeSlice, TimeNow);
	int a = 0;
}

vector<int> findCorrespondingAnchor(string objType, vector<string> dictionary) {
	vector<int> result = {};
	for (int i = 0; i < dictionary.size(); i++) {
		if (objType == dictionary[i])
			result.push_back(i);
	}
	return result;
}

/*
void ReteNet::SpatioTemporalExecution(int TimeSlice, int TimeNow)
{
	//RTree<int, float, 4, float> tree_scalar; // this one responsible for scalar node indexing --> later

#ifdef INDEX_ON
#pragma region 3DimRegion
	RTree<int, float, 3, float> tree; // lat, long, anchor_enum
	//enum dictionaries --> well it is based on the hash map then
	//number of vec_anchor_id and nodewithspatialindex are same
	//the difference is just one only handle string, one handle the node also

#pragma region newAnchorEventAddressing
	string prev_entity = "";

	bool treeIsSet = false;

#ifdef STATIC_CQ
	if (tree.Count() > 0)
		treeIsSet = true;
	else
		treeIsSet = false;
#else
	treeIsSet = false;
#endif // STATIC_CQ
	//asd
	queue<EventPtr> oneTimeEvent_master = {};
	queue<EventPtr> oneTimeEvent_child = {};
	for (int j = 0; j < distanceNode.size(); j++) {

		//one time fetching, if the next entity name is same, so don't fetch cuz its empty :"
		if (prev_entity != static_cast<BetaNode*>(distanceNode[j])->getLeftConnName()) {

			prev_entity = static_cast<BetaNode*>(distanceNode[j])->getLeftConnName();
			oneTimeEvent_master = findNode(prev_entity,1)->getEvRes();

			oneTimeEvent_child = oneTimeEvent_master;
		}
		else {
			oneTimeEvent_child = oneTimeEvent_master;
		}

		if (!treeIsSet) { // do we need update? yes we always do
			if (anchor_stab_map.find(static_cast<BetaNode*>(distanceNode[j])->getLeftConnName()) != anchor_stab_map.end()) // dia adalah anchor stab map yang ke j berarti termasuk achor
			{
				for (; oneTimeEvent_child.size() > 0; oneTimeEvent_child.pop()) {

					float xpos[3], ypos[3];
					//just initiated
					//if (hash_needUpdate[j] == true && hash_latestUpdate[j].first == 0) {
					xpos[0] = oneTimeEvent_child.front()->getFloat("lat") - (static_cast<BetaNode*>(distanceNode[j])->getSpatialLimFloat() / 2);
					xpos[1] = oneTimeEvent_child.front()->getFloat("lat") + (static_cast<BetaNode*>(distanceNode[j])->getSpatialLimFloat() / 2);

					ypos[0] = oneTimeEvent_child.front()->getFloat("lon") - (static_cast<BetaNode*>(distanceNode[j])->getSpatialLimFloat() / 2);
					ypos[1] = oneTimeEvent_child.front()->getFloat("lon") + (static_cast<BetaNode*>(distanceNode[j])->getSpatialLimFloat() / 2);

					//3rd dimension is the enum
					vector<int> corresponding_dimension = findCorrespondingAnchor(static_cast<BetaNode*>(distanceNode[j])->getRightConnName(), observed_obj_dict);
					for (int k = 0; k < corresponding_dimension.size(); k++) {
						xpos[2] = corresponding_dimension[k];
						ypos[2] = corresponding_dimension[k];

						tree.Insert(xpos, ypos, distanceNode[j]->getID());
					}

					//insert to the node
					dynamic_cast<BetaNode*>(distanceNode[j])->forcePushInQueue(&oneTimeEvent_child.front(), true);
				}
			}
		}
	}
#pragma endregion
	//the tree is set now

#pragma region newTreeTesting
	//now test with the enemys
	vector<vector<EventPtr>> organizedTestEvents = {};
	vector<int> nhits_vec;

	//initiate the organizedTestEvents
	for (int j = 0; j < observed_obj_dict.size(); j++) {
		organizedTestEvents.push_back({});
	}

	prev_entity = "";
	for (int j = 0; j < vec_anchor_id.size(); j++) {

		//find is it belong to which index?
		std::vector<string>::iterator it;
		it = find(observed_obj_dict.begin(), observed_obj_dict.end(), vec_anchor_id[j].first.second);
		int dist = distance(observed_obj_dict.begin(), it);

		//assign to each observed index
		if (prev_entity != static_cast<BetaNode*>(distanceNode[j])->getRightConnName()) {
			prev_entity = static_cast<BetaNode*>(distanceNode[j])->getRightConnName();
			oneTimeEvent_master = findNode(prev_entity, 1)->getEvRes();

			oneTimeEvent_child = oneTimeEvent_master;
		}
		else {
			oneTimeEvent_child = oneTimeEvent_master;
		}

		if (organizedTestEvents[dist].size() <= 0) {
			for (; oneTimeEvent_child.size() > 0; oneTimeEvent_child.pop()) {

				organizedTestEvents[dist].push_back(oneTimeEvent_child.front());
			}
		}

	}

	//------------------------------------------------------------------------------------------------
	//after it is dinctinct, so we can process the stabbing process
	//organizedevents is organized as follow --> [dictionary id][vector of event pointer]
	for (int i = 0; i < organizedTestEvents.size(); i++) {
		for (int j = 0; j < organizedTestEvents[i].size(); j++) {

			float xpos[3], ypos[3];
			xpos[0] = xpos[1] = organizedTestEvents[i][j]->getFloat("lat");
			ypos[0] = ypos[1] = organizedTestEvents[i][j]->getFloat("lon");

			xpos[2] = ypos[2] = i; // i is dictionary id

			nhits_vec = tree.Search_vec(xpos, ypos, NULL, NULL);

			//distinct the correlated beta nodes that hit

			//push this into rete net
			//this must be on the right
			for (auto n : nhits_vec) {
				Node* tempNode = NodeList[n];
				if (dynamic_cast<BetaNode*>(tempNode)) {
					dynamic_cast<BetaNode*>(tempNode)->forcePushInQueue(&organizedTestEvents[i][j], false);
				}
			}
		}
	}
#pragma endregion

	//due to constant movement, so just format the tree --> well, this is totally wrong, but still wondering how to fix it
#ifdef STATIC_CQ

#else
	tree.RemoveAll();
#endif // STATIC_CQ


#ifdef SHOW_P_QUEUE
	//bool isExecuting = false;
	//cout << "current time : " << TimeNow << endl <<endl;
	if (p_queue.top()->getExecutionEstimated() == TimeNow) {
		cout << "at time : " << TimeNow << endl;
		printPQNodes();
	}
#endif // SHOW_P_QUEUE


#pragma region Execute based on priority queue

	//while (p_queue.top()->getExecutionEstimated() == TimeNow || p_queue.top()->getExecutionEstimated() <= TimeNow) {
	//	Node* temp = p_queue.top();
	//	p_queue.pop();
	//	temp->testNode(TimeSlice, TimeNow);
	//	p_queue.push(temp);
	//	//isExecuting = true;
	//}

#pragma endregion


#pragma endregion
#endif // INDEX_ON



#ifdef INDEX_OFF
	//address all spatial indexing node
	vector<Node*> pushedNode;
	for (int i = 0; i < distanceNode.size(); i++) {
		pushedNode.push_back(distanceNode[i]);
	}

	while (!pushedNode.empty())
	{
		//pushedNode[0]->testNode(TimeSlice);
		pushedNode[0]->testNode(TimeSlice, TimeNow);

		//Activate successor node
		for (int i = 0; i < pushedNode[0]->getAllPairs().size(); i++) {

			//Check to avoid duplication
			bool isDuplicate = false;
			for (int j = 0; j < pushedNode.size(); j++) {
				if (pushedNode[0]->getSinglePair(i) == pushedNode[j]) {
					isDuplicate = true;
					break;
				}
			}
			if (!isDuplicate)
				pushedNode.push_back(pushedNode[0]->getSinglePair(i));
		}

		pushedNode.erase(pushedNode.begin());
	}
#endif // INDEX_OFF


//#pragma region Execute based on priority queue
//
//	//bool isExecuting = false;
//	//if (p_queue.top()->getExecutionEstimated() == TimeNow) {
//	//	cout << "at time : " << TimeNow << endl;
//	//	printPQNodes();
//	//}
//
//	while (p_queue.top()->getExecutionEstimated() == TimeNow || p_queue.top()->getExecutionEstimated() <= TimeNow) {
//		Node* temp = p_queue.top();
//		p_queue.pop();
//		temp->testNode(TimeSlice);
//		p_queue.push(temp);
//		//isExecuting = true;
//	}
//
//#pragma endregion

}
*/

void ReteNet::NewSpatioTemporalExecution(int TimeSlice, int TimeNow)
{
	//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
#ifdef INDEX_ON

	/*
#pragma region SpatialRegion
	string prev_entity;
	unordered_map<string, queue<EventPtr>> mapTheEvent = {};
	vector<string> EntityName = {};

	for (int j = 0; j < staticCQNodeId.size(); j++) {

		//one time fetching, if the next entity name is same, so don't fetch cuz its empty :"
		//if (prev_entity != static_cast<BetaNode*>(NodeList[staticCQNodeId[j]])->getRightConnName()) {
		if(mapTheEvent[prev_entity].size() == 0){
			prev_entity = static_cast<BetaNode*>(NodeList[staticCQNodeId[j]])->getRightConnName();
			mapTheEvent[prev_entity] = findNode(prev_entity, 1)->getEvRes();
			EntityName.push_back(prev_entity);
		}
	}

	for (auto mapElement : EntityName) {
		for (;mapTheEvent[mapElement].size() > 0; mapTheEvent[mapElement].pop()) {

			if (mapTheEvent[mapElement].front()->map_size() < 0) {
				mapTheEvent[mapElement].pop();
				//delete& mapTheEvent[mapElement].front();
				continue;
			}

			//vector<int> stabbed_node = SpatialNodeIndexing::stabStaticTree(mapTheEvent[mapElement].front(), mapElement);
			vector<int> stabbed_node_boost = SpatialNodeIndexing::stabStaticBoostTree(mapTheEvent[mapElement].front(), mapElement);

			for (int i = 0; i < stabbed_node_boost.size(); i++) {
					//manipulate the anchor
					//static plot does not have anchor
					mapTheEvent[mapElement].front()->addAttr("anchors", "-1,");

					//push to next each proceeding node
					dynamic_cast<BetaNode*>(NodeList[stabbed_node_boost[i]])->forcePushToPairNodeQueue(mapTheEvent[mapElement].front());
			}

			//for (int i = 0; i < stabbed_node.size(); i++) {
			//	//manipulate the anchor
			//	//static plot does not have anchor
			//	mapTheEvent[mapElement].front()->addAttr("anchors", "-1,");

			//	//push to next each proceeding node
			//	dynamic_cast<BetaNode*>(NodeList[stabbed_node[i]])->forcePushToPairNodeQueue(mapTheEvent[mapElement].front());
			//}
		}
	}
#pragma endregion
*/

#pragma region NewSpatialRegion
	for (auto str : observed_obj_dict) {
		queue<EventPtr> result = findNode(str, 1)->getEvRes();

		for (; result.size() > 0; result.pop()) {
			vector<int> stabbed_node_boost = SpatialNodeIndexing::stabStaticBoostTree(result.front(), str);

			for (auto stb : stabbed_node_boost) {
				//manipulate the anchor
				//static plot does not have anchor
				result.front()->addAttr("anchors", "-1,");

				//push to next each proceeding node
				dynamic_cast<BetaNode*>(NodeList[stb])->forcePushToPairNodeQueue(result.front());
				dynamic_cast<BetaNode*>(NodeList[stb])->ClearAllInputQueue();
			}
		}

	}
#pragma endregion


#endif // INDEX_ON

#ifdef INDEX_OFF
	//address all spatial indexing node
	vector<Node*> pushedNode;
	for (int i = 0; i < staticCQNodeId.size(); i++) {
		pushedNode.push_back(NodeList[staticCQNodeId[i]]);
	}

	while (!pushedNode.empty())
	{
		//pushedNode[0]->testNode(TimeSlice);
		pushedNode[0]->testNode(TimeSlice, TimeNow);

		//Activate successor node	
		for (int i = 0; i < pushedNode[0]->getAllPairs().size(); i++) {

			//Check to avoid duplication
			bool isDuplicate = false;
			for (int j = 0; j < pushedNode.size(); j++) {
				if (pushedNode[0]->getSinglePair(i) == pushedNode[j]) {
					isDuplicate = true;
					break;
				}
			}
			if (!isDuplicate)
				pushedNode.push_back(pushedNode[0]->getSinglePair(i));
		}

		pushedNode.erase(pushedNode.begin());
	}

#endif // INDEX_OFF

#ifdef NO_THREADING
	//ReteNet::checkTemporalQueue(TimeSlice, TimeNow);
#endif // THREADING
	//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
	//ReteNet::checkTemporalQueue(TimeSlice, TimeNow);
}

void ReteNet::TemporalExecution(int TimeSlice, int TimeNow)
{
	ReteNet::checkTemporalQueue(TimeSlice, TimeNow);
#ifdef INDEX_ON

#pragma region Execute based on priority queue

	//while (p_queue.top()->getExecutionEstimated() == TimeNow || p_queue.top()->getExecutionEstimated() <= TimeNow) {
	//	Node* temp = p_queue.top();
	//	p_queue.pop();
	//	temp->testNode(TimeSlice);
	//	p_queue.push(temp);
	//	//isExecuting = true;
	//}

#pragma endregion

#endif // INDEX_ON
}

void ReteNet::ExecuteSingleTemporalNode(int TimeSlice, int TimeNow, Node* n)
{
	n->testNode(TimeSlice, TimeNow);
}

vector<int> ReteNet::CheckTemporalExecutionID(int TimeSlice, int TimeNow)
{
	vector<int> res = {};
	while (p_queue.top()->getExecutionEstimated() == TimeNow || p_queue.top()->getExecutionEstimated() <= TimeNow) {
		res.push_back(p_queue.top()->getID());
		p_queue.pop();
		//p_queue.push(temp);
	}
	return res;
}

void ReteNet::reInsertTemporalQueue(int finishedId)
{
	p_queue.push(NodeList[finishedId]);
}

void ReteNet::checkTemporalQueue(int TimeSlice, int TimeNow)
{
	long long timeStart;
	if (p_queue.size() > 0) {
		//if (p_queue.top()->getExecutionEstimated() == TimeNow || p_queue.top()->getExecutionEstimated() <= TimeNow) {
#ifdef USE_SECOND
		if (p_queue.top()->getExecutionEstimated() == Utilities::getTimeSec() || p_queue.top()->getExecutionEstimated() <= Utilities::getTimeSec()) {
#endif // USE_SECOND
#ifdef USE_MILLISEC
			if (p_queue.top()->getExecutionEstimated() == Utilities::getTime() || p_queue.top()->getExecutionEstimated() <= Utilities::getTime()) {
#endif // USE_MILLISEC

				/*cout << "processing : " << p_queue.top()->justCondition << " size = ";
				p_queue.top()->printQueueLen();
				cout << endl;*/

				std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
				//timeStart = Utilities::getTimeSec();
				Node* temp = p_queue.top();
				p_queue.pop();
				//temp->testNode(TimeSlice, Utilities::getTimeSec());
				temp->testNode(TimeSlice, 0);
				p_queue.push(temp);

				//if (p_queue.top()->getExecutionEstimated() == TimeNow) {
				//	cout << "at time : " << Utilities::getTimeSec() << endl;
				//	printPQNodes();
				//}
				//cout << "temporal processing spent: " << Utilities::getTimeSec() - timeStart << " node id: " <<temp->getID() << endl;
			}
		}
	}

	int ReteNet::GetNumberOfNodes()
	{
		return NodeList.size();
	}

	//std::thread ReteNet::runTemporal()
	//{
	//	return std::thread(&ReteNet::ThreadTemporalExecution, this);
	//}

	void ReteNet::buildNetNode()
	{
		//check available Nodes
		for (int i = 0; i < NodeList.size(); i++) {
			if (NodeList[i]->prevNodeAlreadyConnected())
				continue;

			if (dynamic_cast<AlphaNode*>(NodeList[i])) {
				if (static_cast<AlphaNode*>(NodeList[i])->getPrevPairStr() == "")
					continue;
				else {
					Node* prev;
					//check alpha
					prev = findNode(static_cast<AlphaNode*>(NodeList[i])->getPrevPairStr(), 0);
					if (prev) {
						connectNodes(*NodeList[i], *prev);
						continue;
					}

					//check beta
					prev = findNode(static_cast<AlphaNode*>(NodeList[i])->getPrevPairStr(), 1);
					if (prev) {
						connectNodes(*prev, *NodeList[i]);
						continue;
					}

					//it there are no option, then make a new one
					//prev = addAlphaReturnNode(static_cast<AlphaNode*>(NodeList[i])->getPrevPairStr());
					//connectNodes()
				}
			}
			else if (dynamic_cast<BetaNode*>(NodeList[i])) {
				Node* prevL, * prevR;

				/*
				//alpha - alpha
				prevL = findNode(static_cast<BetaNode*>(NodeList[i])->getLeftConnName(), 0);
				prevR = findNode(static_cast<BetaNode*>(NodeList[i])->getRightConnName(), 0);
				if (prevL && prevR) {
					connectNodes(*prevL, *prevR, *NodeList[i]);
				}

				//alpha - beta
				prevL = findNode(static_cast<BetaNode*>(NodeList[i])->getLeftConnName(), 0);
				prevR = findNode(static_cast<BetaNode*>(NodeList[i])->getRightConnName(), 1);
				if (prevL && prevR) {
					connectNodes(*prevL, *prevR, *NodeList[i]);
				}
				else if (prevR) {//special case plot_dist, only left that has connection
					connectNodes(*prevL, *prevR, *NodeList[i]);
				}

				//beta - alpha
				prevL = findNode(static_cast<BetaNode*>(NodeList[i])->getLeftConnName(), 1);
				prevR = findNode(static_cast<BetaNode*>(NodeList[i])->getRightConnName(), 0);
				if (prevL && prevR) {
					connectNodes(*prevL, *prevR, *NodeList[i]);
				}

				//beta - beta
				prevL = findNode(static_cast<BetaNode*>(NodeList[i])->getLeftConnName(), 1);
				prevR = findNode(static_cast<BetaNode*>(NodeList[i])->getRightConnName(), 1);
				if (prevL && prevR) {
					connectNodes(*prevL, *prevR, *NodeList[i]);
				}
				*/

				prevL = findNode(static_cast<BetaNode*>(NodeList[i])->getLeftConnName(), 0);
				prevR = findNode(static_cast<BetaNode*>(NodeList[i])->getRightConnName(), 0);
				if (prevL == NULL) {
					prevL = findNode(static_cast<BetaNode*>(NodeList[i])->getLeftConnName(), 1);
				}
				if (prevR == NULL) {
					prevR = findNode(static_cast<BetaNode*>(NodeList[i])->getRightConnName(), 1);
				}

				connectNodes(*prevL, *prevR, *NodeList[i]);

				continue;
			}
			else
				continue;
		}

#pragma region OldSpatialNodeIndexing
		//mbr node addressing
		for (int i = 0; i < betaListIDDictionary.size(); i++) {
			if (static_cast<BetaNode*>(NodeList[betaListIDDictionary[i]])->getSpecialOpName() == "distance") {
				distanceNode.push_back(NodeList[betaListIDDictionary[i]]);
				vec_anchor_id.push_back({ {static_cast<BetaNode*>(NodeList[betaListIDDictionary[i]])->getLeftConnName(), static_cast<BetaNode*>(NodeList[betaListIDDictionary[i]])->getRightConnName()}, NodeList[betaListIDDictionary[i]]->getID() });
			}
		}
		//discritize vec_anchor_id & mbr node
		std::sort(vec_anchor_id.begin(), vec_anchor_id.end());
		vec_anchor_id.erase(unique(vec_anchor_id.begin(), vec_anchor_id.end()), vec_anchor_id.end());
		std::sort(distanceNode.begin(), distanceNode.end());
		distanceNode.erase(unique(distanceNode.begin(), distanceNode.end()), distanceNode.end());

		//new mbr node addressing
		for (int i = 0; i < betaListIDDictionary.size(); i++) {
			if (static_cast<BetaNode*>(NodeList[betaListIDDictionary[i]])->getSpecialOpName() == ("plot_dist") ||
				static_cast<BetaNode*>(NodeList[betaListIDDictionary[i]])->getSpecialOpName() == ("sonar_dist") ||
				static_cast<BetaNode*>(NodeList[betaListIDDictionary[i]])->getSpecialOpName() == ("radar_dist")) {
				//if (static_cast<BetaNode*>(NodeList[betaListIDDictionary[i]])->getSpecialOpName().find("dist") > 0) {
				staticCQNodeId.push_back(NodeList[betaListIDDictionary[i]]->getID());
			}
		}

		//address the anchor and stabber
		//format = anchor -- desired stabber
		for (int i = 0; i < distanceNode.size(); i++) {
			//not exist
			if (anchor_stab_map.find(static_cast<BetaNode*>(distanceNode[i])->getLeftConnName()) == anchor_stab_map.end()) {
				vector<string> temp;
				temp.push_back(static_cast<BetaNode*>(distanceNode[i])->getRightConnName());
				anchor_stab_map[static_cast<BetaNode*>(distanceNode[i])->getLeftConnName()] = temp;
			}
			else {
				//duplicate check
				bool duplicate = false;
				for (auto a : anchor_stab_map[static_cast<BetaNode*>(distanceNode[i])->getLeftConnName()]) {
					if (static_cast<BetaNode*>(distanceNode[i])->getRightConnName() == a) {
						duplicate = true;
					}
				}
				if (!duplicate) {
					anchor_stab_map[static_cast<BetaNode*>(distanceNode[i])->getLeftConnName()].push_back(static_cast<BetaNode*>(distanceNode[i])->getRightConnName());
				}
			}
		}

		//build enum dictionary
		for (auto au : anchor_stab_map) {
			for (auto sec : au.second) {
				observed_obj_dict.push_back(sec);
			}
		}

		for (auto i : staticCQNodeId) {
			observed_obj_dict.push_back(static_cast<BetaNode*>(NodeList[i])->getRightConnName());
		}

		//disctitize the observed_obj enumeration
		std::sort(observed_obj_dict.begin(), observed_obj_dict.end());
		observed_obj_dict.erase(unique(observed_obj_dict.begin(), observed_obj_dict.end()), observed_obj_dict.end());
#pragma endregion

#pragma region PriorityQueue
		//Reset the queue
		while (p_queue.size() > 0) {
			p_queue.pop();
		}
		//Addressing Node with temporal condition
		for (int i = 0; i < NodeList.size(); i++) {
#ifdef INDEX_OFF
			if (NodeList[i]->getExecutionEstimated() != -1) {
				//nodewithspatialCondition.push_back(NodeList[i]);
				//if (dynamic_cast<BetaNode*>(NodeList[i])) {
				//	if(dynamic_cast<BetaNode*>(NodeList[i])->getSpecialOpName().find("dist") < 0 )
				//		p_queue.push(NodeList[i]);
				//}
				//else {
				p_queue.push(NodeList[i]);
				//}
			}
#endif // INDEX_OFF
#ifdef INDEX_ON
			if (NodeList[i]->getExecutionEstimated() != -1) {
				if (dynamic_cast<BetaNode*>(NodeList[i])) {
					if (static_cast<BetaNode*>(NodeList[i])->getSpecialOpName() != "plot_dist" &&
						static_cast<BetaNode*>(NodeList[i])->getSpecialOpName() != "sonar_dist" &&
						static_cast<BetaNode*>(NodeList[i])->getSpecialOpName() != "radar_dist") {
						p_queue.push(NodeList[i]);
					}
				}
				else
					p_queue.push(NodeList[i]);
			}
#endif // INDEX_ON
		}

		//show the priority queue list
		//printPQNodes();
#ifdef SHOW_LAST_NODE_LIST
		pritAllEndNodes();
#endif // SHOW_BUILT_PROGRESS

#pragma endregion

		////node w/ spatial indexing
		//sort(nodewithspatialCondition.begin(), nodewithspatialCondition.end());
		//nodewithspatialCondition.erase(unique(nodewithspatialCondition.begin(), nodewithspatialCondition.end()), nodewithspatialCondition.end());
		////push into priority queue
		//for (auto n : nodewithspatialCondition) {
		//	//p_queue.push(*n);
		//}


#ifdef INDEX_ON
	//build multi node indexing
		MultiNodeIndexing::reset();

		for (int i = 0; i < alphaListIDDictionary.size(); i++) {
			if (static_cast<AlphaNode*>(NodeList[alphaListIDDictionary[i]])->getPrevPair() == NULL)
				MultiNodeIndexing::addAlphaNode(NodeList[alphaListIDDictionary[i]]);
		}

		MultiNodeIndexing::BuildTree();
		MultiNodeIndexing::createCombination();

		//build Spatial Node Indexing
		SpatialNodeIndexing::reset();

		for (auto id : staticCQNodeId) {
			SpatialNodeIndexing::AddStaticCQ(NodeList[id]);
			//--> this is later
			/*else if (static_cast<BetaNode*>(NodeList[betaListIDDictionary[i]])->getSpecialOpName().find("dist") > 0) {
				SpatialNodeIndexing::AddDynamicCQ(NodeList[betaListIDDictionary[i]]);
			}*/
		}


		//for (int i = 0; i < betaListIDDictionary.size(); i++) {
		//	if (static_cast<BetaNode*>(NodeList[betaListIDDictionary[i]])->getSpecialOpName() == ("plot_dist")) {
		//		SpatialNodeIndexing::AddStaticCQ(NodeList[betaListIDDictionary[i]]);
		//		staticCQNodeId.push_back(NodeList[betaListIDDictionary[i]]->getID());
		//	}
		//	//--> this is later
		//	/*else if (static_cast<BetaNode*>(NodeList[betaListIDDictionary[i]])->getSpecialOpName().find("dist") > 0) {
		//		SpatialNodeIndexing::AddDynamicCQ(NodeList[betaListIDDictionary[i]]);
		//	}*/
		//}

		SpatialNodeIndexing::buildStaticTree();
		//SpatialNodeIndexing::buildDynamicTree(); //--Later
#endif // INDEX_ON

//#pragma region BuildTemporalThread
//	tr_size = new std::thread[p_queue.size()];
//#pragma endregion


		int a = 10;
	}

	void ReteNet::printAllNodes()
	{
		cout << endl << "-------------------------------------------------" << endl;
		cout << "All Created Nodes: " << endl << endl;
		cout << "Alpha Nodes:" << endl;
		int itt = 1;
		for (auto i : alphaListIDDictionary) {
			cout << itt << ". " << NodeList[i]->justCondition << endl;
			itt++;
		}

		cout << endl;
		cout << "Beta Nodes:" << endl;
		for (auto i : betaListIDDictionary) {
			cout << itt << ". " << NodeList[i]->justCondition << endl;
			itt++;
		}
		cout << endl;
	}

	void ReteNet::printPQNodes()
	{
		temp_p_queue = p_queue;

		cout << "Priority queue for spatiotemporal execution" << endl;
		for (; temp_p_queue.size() > 0; temp_p_queue.pop()) {
			cout << "trigger time: " << temp_p_queue.top()->getExecutionEstimated() << " -- Node condition :" << temp_p_queue.top()->justCondition << endl;
		}
	}

	void ReteNet::pritAllEndNodes()
	{
		cout << endl << "-------------------------------------------------" << endl;
		cout << "All End Nodes: " << endl << endl;
		cout << "Alpha Nodes:" << endl;
		int itt = 1;
		for (auto i : alphaListIDDictionary) {
			if (NodeList[i]->getAllPairs().size() < 1) {
				cout << itt << ". " << NodeList[i]->justCondition << endl;
				itt++;
			}
		}

		cout << endl;
		cout << "Beta Nodes:" << endl;
		for (auto i : betaListIDDictionary) {
			if (NodeList[i]->getAllPairs().size() < 1) {
				cout << itt << ". " << NodeList[i]->justCondition << endl;
				itt++;
			}
		}
		cout << endl;
	}

	int ReteNet::lastNode()
	{
		return NodeList.back()->getID();
	}

	Node* ReteNet::findNode(string expression, int nodeType)
	{
		// 0 --> alpha
		// 1 --> beta
		if (nodeType == 0) {
			for (int i = 0; i < alphaListIDDictionary.size(); i++) {
				if (NodeList[alphaListIDDictionary[i]]->justCondition == expression) {
					return NodeList[alphaListIDDictionary[i]];
				}
			}
		}
		else if (nodeType == 1) {
			for (int i = 0; i < betaListIDDictionary.size(); i++) {
				if (static_cast<BetaNode*>(NodeList[betaListIDDictionary[i]])->getProduct() == expression) {
					return NodeList[betaListIDDictionary[i]];
				}
			}
		}
		return nullptr;
	}

	Node* ReteNet::getNodeFromId(int id)
	{
		return NodeList[id];
	}

	void ReteNet::cloneToWm(queue<EventPtr> input)
	{
		m_WMSet.cloneInputQueues(input);
	}

	void ReteNet::pushToWM(EventPtr ev)
	{
		m_WMSet.pushEvent(ev);
	}

	int ReteNet::getWMSize()
	{
		return m_WMSet.getWMInputQueue().size();
		//return m_WMSet.size();
	}

	int ReteNet::getCurrentTime()
	{
		if (m_WMSet.size() > 0)
			return m_WMSet.getFront()->getInt("time");
		else
			return 0;
	}

	void ReteNet::popFrontWM()
	{
		m_WMSet.pop();
	}

	TemporalThread::TemporalThread()
	{
	}

	void TemporalThread::run()
	{
		executedNode->testNode(100, Utilities::time);
	}

	std::thread TemporalThread::runThread(Node * nod)
	{
		executedNode = nod;
		return std::thread(&TemporalThread::run, this);
	}
