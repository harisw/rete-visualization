#include "MultiNodeIndexing.h"

RTree<int, float, 10, float> MultiNodeIndexing::multiDimensionalTree;
unordered_map<string, int> MultiNodeIndexing::attributes_map;
unordered_map<int, vector<int>> MultiNodeIndexing::combinationMap;
vector<pair<int, Node*>> MultiNodeIndexing::collectedAlphaNodes;

int MultiNodeIndexing::checkAttrMap(string attrName)
{
	string name = Utilities::ToLower(attrName);
	std::unordered_map<std::string, int>::const_iterator got = attributes_map.find(name);

	if (got == attributes_map.end())
		return -1;
	else
		return got->second;

	return -1;
}

int MultiNodeIndexing::addAlphaNode(Node* node)
{
	//get the address from map
	int checker = checkAttrMap(static_cast<AlphaNode*>(node)->getAlphaDataType());

	if (checker == -1) { //add to map
		checker = attributes_map.size();

		attributes_map.insert({ static_cast<AlphaNode*>(node)->getAlphaDataType() , checker });
		//freshInsert(checker, node);
	}
	//else {
	//	updateInsert(checker, node);
	//}

	//register to collected node
	collectedAlphaNodes.push_back({ checker, node });
	//distinct in case it is dups
	std::sort(collectedAlphaNodes.begin(), collectedAlphaNodes.end(), sortByMapIndex);
	collectedAlphaNodes.erase(unique(collectedAlphaNodes.begin(), collectedAlphaNodes.end()), collectedAlphaNodes.end());

	return 1;
}

int MultiNodeIndexing::returnFinalNode(EventPtr ep)
{
	vector<int> fetchedNode = stabEvent(ep);
	
	std::sort(fetchedNode.begin(), fetchedNode.end());

	/*for (auto n : combinationMap) {
		if (fetchedNode.size() == n.second.size()) {
			if (fetchedNode == n.second)
				return n.first;
		}
	}*/

	//think about this, what if the nodes are overlapping. like speed > 3 and speed > 6. if the speed is 7, so it will stab both
	//that's why we need candidates?
	for (auto n : combinationMap) {
		if (fetchedNode.size() >= n.second.size()) {
			int match_sum = 0;
			for (auto f_n : fetchedNode) {
				if (match_sum == n.second.size()) {
					//candidates

					break;
				}
				if (f_n == n.second[match_sum]) {
					match_sum++;
				}
			}
			if (match_sum == n.second.size()) {
				return n.first;
			}
		}
	}

	//lookup the combination map
	return -1;
}

void MultiNodeIndexing::createCombination()
{
	vector<Node*> nextNodeId = {}, finalNodeIsTheNodeWithoutWindow = {};

	//init the for forward traversal
	for (auto& itt : collectedAlphaNodes) {
		nextNodeId.push_back(itt.second);

		for (auto nextNode : itt.second->getAllPairs()) {
			nextNodeId.push_back(nextNode);
		}

	}

	//distinct and remove dups
	std::sort(nextNodeId.begin(), nextNodeId.end());
	nextNodeId.erase(unique(nextNodeId.begin(), nextNodeId.end()), nextNodeId.end());

	//forward traversal
	while (nextNodeId.size() > 0) {
		//check is there any of them exist?
		bool alphaExist = false;
		for (auto m_node : nextNodeId) {
			if (dynamic_cast<AlphaNode*>(m_node)) {
				alphaExist = true;
				break;
			}
		}

		if (!alphaExist || finalNodeIsTheNodeWithoutWindow.size() > MAX_LIM)
			break;

		//check each beta node prev connection
		for (auto m_node : nextNodeId) {
			if (dynamic_cast<BetaNode*>(m_node)) {
				Node* left, *right;
				left = static_cast<BetaNode*>(m_node)->getLeftConnNode();
				right = static_cast<BetaNode*>(m_node)->getRightConnNode();

				//find and eliminate left and right
				for (int itt = 0; itt < nextNodeId.size(); itt++) {
					if (left->getID() == nextNodeId[itt]->getID()) {
						nextNodeId.erase(nextNodeId.begin() + itt);
					}
				}
				for (int itt = 0; itt < nextNodeId.size(); itt++) { //have to be double, to avoid not finding
					if (right->getID() == nextNodeId[itt]->getID()) {
						nextNodeId.erase(nextNodeId.begin() + itt);
					}
				}
				//std::vector<Node*>::iterator position = std::find(nextNodeId.begin(), nextNodeId.end(), left);
				//if (position != nextNodeId.end()) // == myVector.end() means the element was not found
				//	nextNodeId.erase(position);

				//position = std::find(nextNodeId.begin(), nextNodeId.end(), right);
				//if (position != nextNodeId.end()) // == myVector.end() means the element was not found
				//	nextNodeId.erase(position);

				//push the proceeding
				for (auto next_m_node : m_node->getAllPairs()) {
					if (next_m_node->getExecutionEstimated() > 0) {
						finalNodeIsTheNodeWithoutWindow.push_back(m_node);
					}
					else {
						nextNodeId.push_back(next_m_node);
					}
				}

				if (m_node->getAllPairs().size() == 0)
					finalNodeIsTheNodeWithoutWindow.push_back(m_node);
			}
		}
		//thats all?
	}

	int b = 11;
	//distinct and remove dups
	std::sort(finalNodeIsTheNodeWithoutWindow.begin(), finalNodeIsTheNodeWithoutWindow.end());
	finalNodeIsTheNodeWithoutWindow.erase(unique(finalNodeIsTheNodeWithoutWindow.begin(), finalNodeIsTheNodeWithoutWindow.end()), finalNodeIsTheNodeWithoutWindow.end());

	//we don't need nextNodeId. so we will recycle it
	nextNodeId = {};
	vector<int>traverse_made;

	//now backtrack using bfs again :v
	for (auto node_now : finalNodeIsTheNodeWithoutWindow) {
		
		//this is for traversal queue
		nextNodeId;
		
		Node* temp, *traverseNode;
		temp = static_cast<BetaNode*>(node_now)->getLeftConnNode();
		if (dynamic_cast<AlphaNode*>(temp))
			traverse_made.push_back(temp->getID());
		else
			nextNodeId.push_back(temp);

		temp = static_cast<BetaNode*>(node_now)->getRightConnNode();
		if (dynamic_cast<AlphaNode*>(temp))
			traverse_made.push_back(temp->getID());
		else
			nextNodeId.push_back(temp);

		while (nextNodeId.size() > 0) {
			traverseNode = nextNodeId[0];
			temp = static_cast<BetaNode*>(traverseNode)->getLeftConnNode();
			if (dynamic_cast<AlphaNode*>(temp))
				traverse_made.push_back(temp->getID());
			else
				nextNodeId.push_back(temp);

			temp = static_cast<BetaNode*>(traverseNode)->getRightConnNode();
			if (dynamic_cast<AlphaNode*>(temp))
				traverse_made.push_back(temp->getID());
			else
				nextNodeId.push_back(temp);

			nextNodeId.erase(nextNodeId.begin());
		}

		//remove and distinct again
		std::sort(traverse_made.begin(), traverse_made.end());
		traverse_made.erase(unique(traverse_made.begin(), traverse_made.end()), traverse_made.end());

		combinationMap[node_now->getID()] = traverse_made;

		//don't forget to reset the traverse made
		traverse_made = {};
	}

	int a = 12;
}

void MultiNodeIndexing::BuildTree()
{
	vector<Node*> tempNodeVector = {};
	vector<pair<int, int>> tempRange = {};
	int now_att = 0;
	for (auto st : collectedAlphaNodes) {
		if (now_att == st.first) {
			tempNodeVector.push_back(st.second);
		}
		else {
			//define the ranges
			vector<pair<int, pair<int, int>>> adjusted_range = adjustRange(tempNodeVector);
			
			int mapEnum = checkAttrMap(static_cast<AlphaNode*>(tempNodeVector[0])->getAlphaDataType());
			//insert it to the tree
			for (auto n : adjusted_range) {
				RectHelper h(mapEnum, n.second.first, n.second.second);

				multiDimensionalTree.Insert(h.min, h.max, n.first);
			}

			//reset all
			tempNodeVector = {};
			tempNodeVector.push_back(st.second);
			now_att++;
		}
	}

	if (tempNodeVector.size() > 0) {
		vector<pair<int, pair<int, int>>> adjusted_range = adjustRange(tempNodeVector);

		int mapEnum = checkAttrMap(static_cast<AlphaNode*>(tempNodeVector[0])->getAlphaDataType());
		//insert it to the tree
		for (auto n : adjusted_range) {
			RectHelper h(mapEnum, n.second.first, n.second.second);

			multiDimensionalTree.Insert(h.min, h.max, n.first);
		}
	}
}

vector<pair<int, pair<int, int>>> MultiNodeIndexing::adjustRange(vector<Node*> tempNodeVector)
{
	//format = ">", (node_id, range_up, range_down)
	unordered_map<string, vector<pair<int, pair<int, int>>>> conditionMap = { {">", {}},{ "<", {}}, {"=", {}} };
	for (auto n : tempNodeVector) {
		//extract the limit
		string cond = static_cast<AlphaNode*>(n)->getCondition();
		float limit = atof(static_cast<AlphaNode*>(n)->getVarLimit().c_str());
		pair<int, int> min_max_pair;
		if (cond == ">") {
			min_max_pair.first = limit + 1;
			min_max_pair.second = MAX_LIM;

			conditionMap[">"].push_back({ n->getID(), min_max_pair });
		}
		else if (cond == ">=") {
			min_max_pair.first = limit;
			min_max_pair.second = MAX_LIM;

			conditionMap[">"].push_back({ n->getID(), min_max_pair });
		}
		else if (cond == "<") {
			min_max_pair.first = MIN_LIM;
			min_max_pair.second = limit - 1;

			conditionMap["<"].push_back({ n->getID(), min_max_pair });
		}
		else if (cond == "<=") {
			min_max_pair.first = MAX_LIM;
			min_max_pair.second = limit;

			conditionMap["<"].push_back({ n->getID(), min_max_pair });
		}
		else if (cond == "==" || cond == "=") {
			// check whether contain number or not
			if (limit == 0) { // no number detected
				//it should be true or false
				if (static_cast<AlphaNode*>(n)->getVarLimit() == "true") {
					//wait, cannot think :"
					min_max_pair.first = 0; //-->true (0, 999)
					min_max_pair.second = MAX_LIM;
				}
				else {
					min_max_pair.first = MIN_LIM; // --> false (-999, 0)
					min_max_pair.second = 0;
				}
			}
			else { // it is number
				min_max_pair.first = limit - 1; // plus minus 1 for tolerance
				min_max_pair.second = limit + 1;
			}

			conditionMap["="].push_back({ n->getID(), min_max_pair });
		}
		//tempRange.push_back(min_max_pair);
	}

	vector<pair<int, pair<int, int>>> finalRes = {};

	// morethan
	vector<pair<int, pair<int, int>>> tempPair = {};
	for (auto a : conditionMap[">"]) {
		tempPair.push_back({a.first, { a.second.first, a.second.second } });
	}
	if (tempPair.size() > 1) {
		sort(tempPair.begin(), tempPair.end(), sortByLowerBound);
		for (int i = 0; i < tempPair.size() - 1; i++) {
			finalRes.push_back({ tempPair[i].first, {tempPair[i].second.first, tempPair[i + 1].second.first} });
		}
	}
	//push the last
	if(tempPair.size() > 0)
		finalRes.push_back({ tempPair[tempPair.size()-1].first, {tempPair[tempPair.size() - 1].second.first, tempPair[tempPair.size() - 1].second.second} });

	//lessthan
	tempPair = {};
	for (auto a : conditionMap["<"]) {
		tempPair.push_back({ a.first, { a.second.first, a.second.second } });
	}
	if (tempPair.size() > 1) {
		sort(tempPair.begin(), tempPair.end(), sortByUpperBound);
		for (int i = 0; i < tempPair.size() - 1; i++) {
			finalRes.push_back({ tempPair[i].first, {tempPair[i].second.second, tempPair[i + 1].second.second} });
		}
	}
	//push the last
	if(tempPair.size() > 0)
		finalRes.push_back({ tempPair[tempPair.size() - 1].first, {tempPair[tempPair.size() - 1].second.first, tempPair[tempPair.size() - 1].second.second} });

	//equals
	tempPair = {};
	for (auto a : conditionMap["="]) {
		finalRes.push_back({ a.first, {a.second.first, a.second.second} });
	}

	int c = 11;

	return finalRes;
}

void MultiNodeIndexing::freshInsert(int attrIndex ,Node* node)
{
	//extract the limit
	string cond = static_cast<AlphaNode*>(node)->getCondition();
	float limit = atof(static_cast<AlphaNode*>(node)->getVarLimit().c_str());

	pair <int, int> min_max_pair;

	if (cond == ">") {
		min_max_pair.first = limit + 1;
		min_max_pair.second = MAX_LIM;
	}
	else if (cond == ">=") {
		min_max_pair.first = limit;
		min_max_pair.second = MAX_LIM;
	}
	else if (cond == "<") {
		min_max_pair.first = MIN_LIM;
		min_max_pair.second = limit - 1;
	}
	else if (cond == "<=") {
		min_max_pair.first = MAX_LIM;
		min_max_pair.second = limit;
	}
	else if (cond == "==" || cond == "=") {
		// check whether contain number or not
		if (limit == 0) { // no number detected
			//it should be true or false
			if (static_cast<AlphaNode*>(node)->getVarLimit() == "true") {
				//wait, cannot think :"
				min_max_pair.first = 0; //-->true (0, 999)
				min_max_pair.second = MAX_LIM;
			}
			else {
				min_max_pair.first = MIN_LIM; // --> false (-999, 0)
				min_max_pair.second = 0;
			}
		}
		else { // it is number
			min_max_pair.first = limit - 1; // plus minus 1 for tolerance
			min_max_pair.second = limit + 1;
		}
	}

	//insert into R-Tree
	RectHelper r(attrIndex, min_max_pair.first, min_max_pair.second);

	multiDimensionalTree.Insert(r.min, r.max, node->getID());
}

void MultiNodeIndexing::updateInsert(int attrIndex, Node* node)
{
	//extract the limit
	string cond = static_cast<AlphaNode*>(node)->getCondition();
	float limit = atof(static_cast<AlphaNode*>(node)->getVarLimit().c_str());

	pair <int, int> min_max_pair;

	if (cond == ">") {
		min_max_pair.first = limit + 1;
		min_max_pair.second = MAX_LIM;
	}
	else if (cond == ">=") {
		min_max_pair.first = limit;
		min_max_pair.second = MAX_LIM;
	}
	else if (cond == "<") {
		min_max_pair.first = MIN_LIM;
		min_max_pair.second = limit - 1;
	}
	else if (cond == "<=") {
		min_max_pair.first = MAX_LIM;
		min_max_pair.second = limit;
	}
	else if (cond == "==" || cond == "=") {
		// check whether contain number or not
		if (limit == 0) { // no number detected
			//it should be true or false
			if (static_cast<AlphaNode*>(node)->getVarLimit() == "true") {
				//wait, cannot think :"
				min_max_pair.first = 0; //-->true (0, 999)
				min_max_pair.second = MAX_LIM;
			}
			else {
				min_max_pair.first = MIN_LIM; // --> false (-999, 0)
				min_max_pair.second = 0;
			}
		}
		else { // it is number
			min_max_pair.first = limit - 1; // plus minus 1 for tolerance
			min_max_pair.second = limit + 1;
		}
	}

	//overlap checking
	string datatype = static_cast<AlphaNode*>(node)->getAlphaDataType();
	vector<Node*> fetchedNode = {};
	for (auto n : collectedAlphaNodes) {
		//if (static_cast<AlphaNode*>(n)->getAlphaDataType() == datatype) {
		//	
		//	static_cast<AlphaNode*>(n)->

		//}
	}

}

bool MultiNodeIndexing::sortByMapIndex(pair<int, Node*> a, pair<int, Node*> b)
{
	return (a.first < b.first);
}

bool MultiNodeIndexing::sortByUpperBound(pair<int, pair<int, int>> a, pair<int, pair<int, int>> b)
{
	return (a.second.second < b.second.second);
}

bool MultiNodeIndexing::sortByLowerBound(pair<int, pair<int, int>> a, pair<int, pair<int, int>> b)
{
	return (a.second.first < b.second.first);
}

vector<int> MultiNodeIndexing::stabEvent(EventPtr ep)
{
	RectHelper r;

	//ah, just iterate it
	for (auto a : ep->getMap()) {
		int checker = checkAttrMap(a.first);

		if(checker != -1)
			r.searchAndSet(checker, ep->getString(a.first));
		//r.(checker, ep->getString(a.first));
	}

	return multiDimensionalTree.Search_vec(r.max, r.min, NULL, NULL);
}

void MultiNodeIndexing::reset()
{
	clearTheTree();
	attributes_map = {};
	combinationMap = {};
	collectedAlphaNodes = {};
}

int MultiNodeIndexing::clearTheTree()
{
	multiDimensionalTree.RemoveAll();

	return 1;
}
