#include "AlphaNode.h"

int AlphaNode::testNode(int TimeSlice, int TimeNow)
{
	std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
//	if(inputQueue.first.size() > 0)
	return justTest(TimeSlice);
	//else {
	//	if (win)
	//		win->setInitTime(win->getInitTime() + win->getTriggerTime());
	//}
	//return 0;
}

//int AlphaNode::getID()
//{
//	return id;
//}

string AlphaNode::getType()
{
	return "Alpha";
}


AlphaNode::AlphaNode(int id_given, string condition) : Node(id_given, condition)
{
	inputQueue.first = {};
	int itt = 0;

	Utilities::tokenizeSingleExp(condition, prevNode.first, thisDataType, thisCondition, thisVarLimit, specialOp);
	if (specialOp != "") {
		m_spatialOp = new SpatialOp(specialOp);
		win = new SlidingWindow(1, 1); // by default it will be executed each second
		/*if (specialOp != "exist") {
			m_spatialOp->setVarCondition(thisCondition);
			m_spatialOp->setVarLimit(thisVarLimit);
		}*/
	}
}

string AlphaNode::getAlphaDataType()
{
	return thisDataType;
}

string AlphaNode::getCondition()
{
	return thisCondition;
}

string AlphaNode::getVarLimit()
{
	return thisVarLimit;
}

Node* AlphaNode::getSinglePair(int i)
{
	return listOfNextPair[i];
}

vector<Node*> AlphaNode::getAllPairs()
{
	return listOfNextPair;
}

bool AlphaNode::prevNodeAlreadyConnected()
{
	if (prevNode.second != NULL)
		return true;
	return false;
}

Node* AlphaNode::getPrevPair()
{
	return prevNode.second;
}

string AlphaNode::getPrevPairStr()
{
	return prevNode.first;
}

bool AlphaNode::ItIsDirect()
{
	return directToWM;
}

void AlphaNode::printQueueLen()
{
	cout << " single size : " << inputQueue.first.size() << endl;
}

queue<EventPtr> AlphaNode::getEvRes()
{
	return EventResult;
}

void AlphaNode::pushResult(queue<EventPtr> result, Node* inputNode)
{
	//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
	//queue<EventPtr> dummy_res = result;
	if (inputQueue.second == inputNode) {
		while (result.size() > 0) {
			//inputQueue.first.push(result.front());
			validify(result.front());
			result.pop();
		}
	}
	//lg.~lock_guard();
		//inputQueue.first = *result;
		//inputQueue.first.push(result);
}

void AlphaNode::pushResult(EventPtr result, Node* inputNode)
{
	std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
	if (inputQueue.second == inputNode) {
		//inputQueue.first.push(result);
		validify(result);
	}
}

int AlphaNode::setPrevConnection(Node* node)
{
	inputQueue.second = node;
	prevNode.second = node;
	directToWM = false;
	return 0;
}

void AlphaNode::setWindow(int len, int step)
{
	win = new SlidingWindow(len, step);
}

long long AlphaNode::getExecutionEstimated()
{
	if (win == NULL)
		return -1;
	else
		return win->getInitTime();
}

vector<Node*> AlphaNode::getNextPairs()
{
	return this->listOfNextPair;
}

pair<string, Node*> AlphaNode::getPrevNode()
{
	return this->prevNode;
}

int AlphaNode::checkExistPair(Node* pairs)
{
	for (int i = 0; i < listOfNextPair.size(); i++) {
		if (listOfNextPair[i] == pairs)
			return 1;
	}

	return 0;
}

int AlphaNode::addBetaPair(Node* pair)
{
	int isExist = checkExistPair(pair);
	if (isExist)
		return 1;
	else {
		//listOfBetaPairs.push_back(pair);
		listOfNextPair.push_back(pair);
		return 1;
	}
	return -1;
}

int AlphaNode::ResetNode()
{
	//testRes = {};
	//listOfBetaPairs = {};
	listOfNextPair = {};
	//betaPair = NULL;
	return 1;
}

int AlphaNode::ClearResults()
{
	//while (!EventResult.empty()) {
	//	EventResult.pop();
	//}
	EventResult = {};

	validationForIncomingEvent.clear();
	return 1;
}

void AlphaNode::testAlphaAndPush(queue<EventPtr>* DirectInputQueue, int TimeSlice)
{
	inputQueue.first = *DirectInputQueue;
	justTest(TimeSlice);
}

int AlphaNode::justTest(int TimeSlice)
{
	if (inputQueue.first.empty()) {// && win->getOriginalRes().size() > 0) {
		//even it is empty, but the time still updated
		if (win)
			win->setInitTime(win->getInitTime() + win->getTriggerTime());

		return 0;
	}

	float limit = atof(thisVarLimit.c_str());

	if (limit == 0 && m_spatialOp == NULL) {//its not a integer val
		int cases = -1;
		if (thisCondition == "=" || thisCondition == "==")
			cases = 0;
		else if (thisCondition == "!=")
			cases = 1;

		int timeSlice_i = TimeSlice;
		while (!inputQueue.first.empty() && timeSlice_i > 0) {//current input queue is not empty.

			//null filtering
			if (inputQueue.first.front() == nullptr) {
				delete& inputQueue.first.front();
				//inputQueue.first.pop();
			}
			EventPtr originalFrontEvent = inputQueue.first.front();

			if (thisVarLimit == "All") {
				EventResult.push(originalFrontEvent);
			}
			else if (cases == 0 && originalFrontEvent->getString(thisDataType) == thisVarLimit) {
				EventResult.push(originalFrontEvent);
			}
			else if (cases == 1 && originalFrontEvent->getString(thisDataType) != thisVarLimit) {
				EventResult.push(originalFrontEvent);
			}

			inputQueue.first.pop();
			timeSlice_i--;
		}
	}
	else if (limit == 0 && m_spatialOp != NULL) {

		//special case for alter function
		if (m_spatialOp->getQueryName().find("alter") != std::string::npos) {
			EventResult = m_spatialOp->process(win);
		}
		else {
			//copy the event to window
			//also distinct the anchor --> anchors are all same
			//string tempAnchor = inputQueue.first.front()->getString("anchors");

			int i_timeSlice = TimeSlice;

			inputQueue.first = {};
			//while (!inputQueue.first.empty()){// && i_timeSlice > 0) {
			//	EventPtr originalFrontEvent = inputQueue.first.front();
			//
			//	win->addEvent(originalFrontEvent);
			//
			//	inputQueue.first.pop();
			//
			//	//i_timeSlice--;
			//}

			////check time
			//if (win->getInitTime() + win->getTriggerTime() < win->getHigheststOriginalTime()) {
			//	win->setInitTime(win->getInitTime() + win->getTriggerTime()); // update the execution time
			//}
			//else {
			//	return 0;
			//}

			/*vector<string> objAnchor_s = Utilities::splitDelim(tempAnchor, ",");
			vector<int> objAnchor_i;
			for (auto i : objAnchor_s) {
				objAnchor_i.push_back(atoi(i.c_str()));
			}
			objAnchor_i.pop_back();*/

			//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
			if (win)
				EventResult = m_spatialOp->process(win, {});

			if (win) {// if we have the window but no specialOp, the window len won't update
				win->setInitTime(win->getInitTime() + win->getTriggerTime());
			}
		}
	}
	else {
		int cases = -1;
		// 0 -> equal less
		// 1 -> equal more
		// 2 -> less
		// 3 -> more
		if (thisCondition == "<=")
			cases = 0;
		else if (thisCondition == ">=")
			cases = 1;
		else if (thisCondition == "<")
			cases = 2;
		else if (thisCondition == ">")
			cases = 3;
		else if (thisCondition == "==")
			cases = 4;

		int timeSlice_i = TimeSlice;
		string DataType = Utilities::ToLower(thisDataType);
		while (!inputQueue.first.empty() && timeSlice_i > 0) {//current input queue is not empty.
			EventPtr originalFrontEvent = inputQueue.first.front();

			if (cases == 0 && originalFrontEvent->getFloat(DataType) <= limit) {
				EventResult.push(originalFrontEvent);
			}
			else if (cases == 1 && originalFrontEvent->getFloat(DataType) >= limit) {
				EventResult.push(originalFrontEvent);
			}
			else if (cases == 2 && originalFrontEvent->getFloat(DataType) < limit) {
				EventResult.push(originalFrontEvent);
			}
			else if (cases == 3 && originalFrontEvent->getFloat(DataType) > limit) {
				EventResult.push(originalFrontEvent);
			}
			else if (cases == 4 && originalFrontEvent->getFloat(DataType) == limit) {
				EventResult.push(originalFrontEvent);
			}

			inputQueue.first.pop();
			timeSlice_i--;
		}
	}

	if (EventResult.size() > 0) {
		this->isActivated = true;
		ReteNet::triggered_node.insert(make_pair(justCondition, this));
		ReteNet::triggered_node_vect.push_back(this);
	}

	//PUSH TO PROCEEDING NODE .-.
	std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
	for (Node* n : listOfNextPair) {

		//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
		n->pushResult(EventResult, this);
		//ReteNet::triggered_ev.push(justCondition);
		ReteNet::triggered_node.insert(make_pair(n->justCondition, n));
		ReteNet::triggered_node_vect.push_back(n);

	}

	if (EventResult.size() > 0) {
		ClearResults();
		return 1;
	}
	else {
		return 0;
	}
}

void AlphaNode::validify(EventPtr result)
{
	//Event* re = result->clone();

	int id = result->getId();
	int len = validationForIncomingEvent.size();
	validationForIncomingEvent.insert(id);

	if (validationForIncomingEvent.size() != len) {
		if (win)
			win->addEvent(result);

		inputQueue.first.push(result);
	}
}

bool AlphaNode::isEmptyResult()
{
	//if (EventResult == NULL || EventResult.size() == 0)
	if (EventResult.size() == 0)
		return true;
	return false;
}
