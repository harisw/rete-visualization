#include "BetaNode.h"

//all of this is located in line 785

//hmm, i forget :v
#define ONLY_TRIGGER_RESULT

//due to effectivity, this only show the time, without explaining what event that trigger dis
//#define ONLY_TRIGGER_TIME_RESULT 

//will definitely trigger all triggered node, hmm, i think its the same with the SHOW_RESULT
//#define SHOW_RESULT 

//will only cout the trigger of the last node, and the node name (CEP name)
//#define SHOW_LAST_NODE_TRIGGER 

#define INDEX_ON
//#define INDEX_OFF

//BetaNode::BetaNode()
//	: Node(BaseNodeID, tempComingCondition)
//{
//}

BetaNode::BetaNode(int id_given, string condition)
	: Node(id_given, condition)
{
	int itt = 0;

	//id = id_given;

	string limit;

	Utilities::tokenizeBetaExp(condition, leftSourcePair.first, rightSourcePair.first, specialOperation, limit, thisCondition, thisProduct);

	//trimming
	leftSourcePair.first = Utilities::lTrim(leftSourcePair.first);
	leftSourcePair.first = Utilities::rTrim(leftSourcePair.first);

	rightSourcePair.first = Utilities::lTrim(rightSourcePair.first);
	rightSourcePair.first = Utilities::rTrim(rightSourcePair.first);

#pragma region CreateSmallNodeBecauseitContainspatialOp

	if (specialOperation == "") {
		int leftBr = -1, rightBr = -1, coma = -1;
		//check left and right
		leftBr = leftSourcePair.first.find("(");
		rightBr = leftSourcePair.first.find(")");
		coma = leftSourcePair.first.find(",");

		if (coma > -1) { //if spatial function --> no just distance rule :v
			Node* temp = ReteNet::addBetaReturnNode(leftSourcePair.first);
			//*id_given = *(temp->getID() + 1);
			//int masterNodeID = ReteNet::NodeList.size();
			//connect both
			static_cast<BetaNode*>(temp)->addBetaPair(this);
			leftSourcePair.second = temp;
			leftInputQueue.second = temp;

			//ok this is just temporary --> i need to put the window information inside --------------------------------------------------
			//why we need window information?
			//static_cast<BetaNode*>(temp)->setWindow(1, 0);
			//----------------------------------------------------------------------------------------------------------------------------

			key = "time";
		}
		if (leftBr > -1 && rightBr > -1 && coma == -1) { //only aggregate function
			specialOperationLeft = leftSourcePair.first.substr(0, leftBr);

			string leftCompExp = Utilities::getComparisonExp(leftSourcePair.first);
			if (leftCompExp != "") {
				string leftLimit = leftSourcePair.first.substr(rightBr, leftSourcePair.first.size() - rightBr);
			}
		}

		leftBr = rightSourcePair.first.find("(");
		rightBr = rightSourcePair.first.find(")");
		coma = rightSourcePair.first.find(",");

		if (coma > -1) { //if spatial function
			Node* temp = ReteNet::addBetaReturnNode(rightSourcePair.first);

			//connect both
			static_cast<BetaNode*>(temp)->addBetaPair(this);
			rightSourcePair.second = temp;
			rightInputQueue.second = temp;

			key = "time";

			//ok this is just temporary --> i need to put the windor information inside --------------------------------------------------
			static_cast<BetaNode*>(temp)->setWindow(1, 0);
			//----------------------------------------------------------------------------------------------------------------------------
		}
		if (leftBr > -1 && rightBr > -1 && coma == -1) { //only aggregate function
			specialOperationLeft = rightSourcePair.first.substr(0, leftBr);

			string leftCompExp = Utilities::getComparisonExp(rightSourcePair.first);
			if (leftCompExp != "")
				string leftLimit = rightSourcePair.first.substr(rightBr, rightSourcePair.first.size() - rightBr);

		}
	}

#pragma endregion


	if (specialOperation != "") {
		key = "time";
		thisSpatialOp = new SpatialOp(specialOperation);
		if (limit != "")
			thisSpatialOp->setVarLimit(limit);
		if (thisCondition != "")
			thisSpatialOp->setVarCondition(thisCondition);
	}
}

string BetaNode::getType()
{
	return "Beta";
}

Node* BetaNode::getLeftConnNode()
{
	return leftSourcePair.second;
}

Node* BetaNode::getRightConnNode()
{
	return rightSourcePair.second;
}

string BetaNode::getLeftConnName()
{
	return leftSourcePair.first;
}

string BetaNode::getRightConnName()
{
	return rightSourcePair.first;
}

string BetaNode::getSpecialOpName()
{
	return specialOperation;
}

queue<EventPtr> BetaNode::getLeftInput()
{
	return leftInputQueue.first;
}

queue<EventPtr> BetaNode::getRightInput()
{
	return rightInputQueue.first;
}

void BetaNode::validify(EventPtr result)
{
	//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
	//Event* cl = result->clone();
	//if (win && thisSpatialOp == NULL)
	//	win->addResultEvent(EventPtr(e));
	//else
	//	win->addEvent(EventPtr(e));

	//EventResult.push(EventPtr(r));
	int id = result->getId();

	//validationForIncomingEvent[id] += 1;
	auto search = validationForIncomingEvent.find(id);
	bool isExist = false;

	if (search != validationForIncomingEvent.end()) {//it is exist
		search->second += 1;
		isExist = true;
	}
	else {
		validationForIncomingEvent[id] = 1;
		isExist = false;
	}

	if (key != "time") {
		//validationForIncomingEvent[id]++;
		//validationForIncomingEvent.find(id)->second ++;

		if (validationForIncomingEvent[id] == 2 && thisCondition == "and") { // no need to edit
		//if (isExist) {
		//	if (search->second == 2 && thisCondition == "and") {
				//match made
			if (win)
				win->addEvent(result);
			else
				EventResult.push(result);
		}
		//}
	}
	else {
		//validationForIncomingEvent[id] = validationForIncomingEvent[id] + 1;
		//validationForIncomingEvent[id] += 1;

		//single node
		if (leftInputQueue.second == rightInputQueue.second) {
			if (validationForIncomingEvent[id] == 1 && thisCondition == "and") { //just created no need to edit
			//if (search->second == 1 && thisCondition == "and") {
				//match made
				if (win)
					win->addEvent(result);
				else
					EventResult.push(result);
			}
		}
		else if (specialOperation == "plot_dist" ||
			specialOperation == "radar_dist" ||
			specialOperation == "sonar_dist") {
			if (validationForIncomingEvent[id] == 1 && thisCondition == "and") {
				//if (search->second == 1 && thisCondition == "and") {
					//match made
				if (win)
					win->addEvent(result);
				else
					EventResult.push(result);
			}
		}
		//double input node
		else {
			if (validationForIncomingEvent[id] == 2 && thisCondition == "and") {
				//if (search->second == 2 && thisCondition == "and") {

				bool contain_distance = false;
				int distanceOnLeft = leftSourcePair.first.find("dist");
				int distanceOnRight = rightSourcePair.first.find("dist");
				if (distanceOnLeft > -1 || distanceOnRight > -1) {
					contain_distance = true;
				}

				if (contain_distance) {
					//Event* r = result->clone();

					if (win && thisSpatialOp == NULL)
						win->addResultEvent(result);
					else
						win->addEvent(result);

					EventResult.push(result);
					//left->pop();
				}
				else {
					//no distance, just put new event
					Event* e = new Event(Utilities::id++, win->getInitTime());
					e->addAttr(thisProduct, "true");

					//so it should be left and right already get the anchors
					e->addAttr("anchors", result->getString("anchors"));

					//Event* r = e->clone();

					//push to window
					if (win) {
						//no need to consider about original event --> staright to result
						win->addResultEvent(EventPtr(e));
					}

					EventResult.push(EventPtr(e));

					//free(&result);
				}

				////match made
				//if (win && thisSpatialOp == NULL)
				//	win->addResultEvent(result);
				//else if(win && thisSpatialOp != NULL)
				//	win->addEvent(result);
				////else
				//EventResult.push(result);
			}
		}
	}
	//lg.~lock_guard();

	//cl = nullptr;

	//delete cl;
	//free(cl);
}

float BetaNode::getSpatialLimFloat()
{
	return thisSpatialOp->getLimitFloat();
}

void BetaNode::setWindow(int len, int step)
{
	win = new SlidingWindow(len, step);

	//temporary function
	if (leftSourcePair.second != NULL) {
		leftSourcePair.second->setWindow(len, step);
	}
	if (rightSourcePair.second != NULL) {
		rightSourcePair.second->setWindow(len, step);
	}
}

void BetaNode::setMatchingKey(string newKey)
{
	key = newKey;
}

void BetaNode::setAggregateCondition(string expression)
{
	specialOperation = expression;
	thisSpatialOp = new SpatialOp(expression);
}

Node* BetaNode::getSinglePair(int i)
{
	return listOfNextPair[i];
}

vector<Node*> BetaNode::getAllPairs()
{
	return listOfNextPair;
}

bool BetaNode::prevNodeAlreadyConnected()
{
	if (leftSourcePair.second != NULL && rightSourcePair.second != NULL)
		return true;
	return false;
}

int BetaNode::setLeftConnection(Node* node)
{
	if (leftSourcePair.second == NULL) {
		leftSourcePair.second = node;

		leftInputQueue.first = {};
		leftInputQueue.second = node;
		return 1;
	}

	if (leftInputQueue.second == NULL) {
		leftInputQueue.first = {};
		leftInputQueue.second = node;
		return 1;
	}

	return 0;
}

int BetaNode::setRightConnection(Node* node)
{
	if (rightSourcePair.second == NULL) {
		rightSourcePair.second = node;

		rightInputQueue.first = {};
		rightInputQueue.second = node;
		return 1;
	}

	if (rightInputQueue.second == NULL) {
		rightInputQueue.first = {};
		rightInputQueue.second = node;
		return 1;
	}

	return 0;

}

bool BetaNode::isEmptyResult()
{
	if (EventResult.size() == 0)
		return true;
	return false;
}

bool compareQueue(EventPtr a, EventPtr b)
{
	return (a->getInt("time") < b->getInt("time"));
}

int BetaNode::justTest(int TimeSlice)
{
	//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
	//dont forget about acnhor
	bool anchorIsAtLeft = true;
	vector<int> anchorObjId = {};

	stringstream output_stream;
	//in case both of the inputs are same
	//idk why but only left that is working
	if (leftSourcePair.second == rightSourcePair.second) {

		if (leftInputQueue.first.empty()) {
			//even it is empty, but the time still updated
			if (win)
				win->setInitTime(win->getInitTime() + win->getTriggerTime());

			return 0;
		}

		while (1) {
			if (leftInputQueue.first.size() == 0)
				break;

			//push to window
			if (win) {
				//straight to result
				//win->addEvent(leftInputQueue.first.front());
				win->addResultEvent(leftInputQueue.first.front());
			}

			EventResult.push(leftInputQueue.first.front()); //this add 1

			leftInputQueue.first.pop();
		}
	}
	//Ordered join method
	else if (thisCondition == "and" || specialOperation != "") {// if it is and or some specialOperation thing


		if (leftInputQueue.first.size() == 0 || rightInputQueue.first.size() == 0) {
			//even it is empty, but the time still updated
			if (specialOperation != "plot_dist") {
				if (win)
					win->setInitTime(win->getInitTime() + win->getTriggerTime());

				return 0;
			}
		}

		//special case for time --> so far we make this system is executed each time ticks. Meaning all input from left or right 
		//is already at the same time
		if (key == "time") {
			// if it is talking about special operation, so we need to decide who is the anchor
			if (specialOperation == "distance") { // woo, woo, this is for distance node anyway ._.
				//left input also the anchor
				vector<EventPtr> tempVecQueue;

				for (; leftInputQueue.first.size() > 0; leftInputQueue.first.pop()) {
					tempVecQueue.push_back(leftInputQueue.first.front());
					anchorObjId.push_back(leftInputQueue.first.front()->getInt("objid"));
				}
				//right input
				for (; rightInputQueue.first.size() > 0; rightInputQueue.first.pop()) {
					tempVecQueue.push_back(rightInputQueue.first.front());
				}

				sort(tempVecQueue.begin(), tempVecQueue.end(), compareQueue);
				tempVecQueue.erase(unique(tempVecQueue.begin(), tempVecQueue.end()), tempVecQueue.end());

				for (auto ve : tempVecQueue) {
					if (win)
						win->addEvent(ve);
					//EventResult.push(ve);
				}

				int a = 10;
			}
			else if (specialOperation == "plot_dist" || specialOperation == "sonar_dist" || specialOperation == "radar_dist") { //in indexing it is not executed

				//match already done in pushNode function

				vector<EventPtr> tempVecQueue;

				set<int> verifyId;
				int verifyLen;

				//right input
				for (; rightInputQueue.first.size() > 0; rightInputQueue.first.pop()) {
					tempVecQueue.push_back(rightInputQueue.first.front());

					//new filtering method
					verifyLen = verifyId.size();
					verifyId.insert(rightInputQueue.first.front()->getId());
					if (verifyLen != verifyId.size()) {
						if (win) {
							win->addEvent(rightInputQueue.first.front());
						}
					}
				}

				//duplicate filtering
				//sort(tempVecQueue.begin(), tempVecQueue.end(), compareQueue);
				//tempVecQueue.erase(unique(tempVecQueue.begin(), tempVecQueue.end()), tempVecQueue.end());

				//for (auto ve : tempVecQueue) {
				//	if (win)
				//		win->addEvent(ve);
				//	//EventResult.push(ve);
				//}
			}
			else {

				//new mathcing is done in pushNode function

				/*
				//no need to consider about anchor just match the time
				//if one of them is distance, so that distance contain the original data ._.
				int distanceOnLeft = leftSourcePair.first.find("dist");
				int distanceOnRight = rightSourcePair.first.find("dist");
				bool contain_distance = false, distanceIsAtLeft = true;
				if (distanceOnLeft > -1 || distanceOnRight > -1) {
					contain_distance = true;
					if (distanceOnRight > -1)
						distanceIsAtLeft = false;
				}

				//left is less than right - HAVE TO
				queue<EventPtr>* left, * right;
				if (leftInputQueue.first.size() <= rightInputQueue.first.size()) { // normal
					left = &leftInputQueue.first;
					right = &rightInputQueue.first;
					anchorIsAtLeft = true;

					if (contain_distance && distanceIsAtLeft)
						distanceIsAtLeft = true;
					else
						distanceIsAtLeft = false;
				}
				else
				{ //flip
					left = &rightInputQueue.first;
					right = &leftInputQueue.first;
					anchorIsAtLeft = false;

					if (contain_distance && distanceIsAtLeft)
						distanceIsAtLeft = false;
					else
						distanceIsAtLeft = true;
				}

				while (1) {
					if (left->size() == 0 || right->size() == 0)
						break;

					if (left->size() == 4294967294 || right->size() == 4294967294)
						break;

					EventPtr frontLeftEvent = left->front();
					EventPtr frontRightEvent = right->front();

					if (frontLeftEvent->map_size() > 4294967294) {
						left->pop();
						//delete &left->front();
						continue;
					}
					if (frontRightEvent->map_size() > 4294967294 || frontRightEvent->map_size() < 0) {
						right->pop();
						//delete& right->front();
						continue;
					}

					if (frontLeftEvent->getInt(key) == frontRightEvent->getInt(key)) {

						if (contain_distance) {
							Event* e;
							if (distanceIsAtLeft)
								e = frontLeftEvent->clone();
							else
								e = frontRightEvent->clone();

							Event* r = e->clone();

							if(win && thisSpatialOp == NULL)
								win->addResultEvent(EventPtr(e));
							else
								win->addEvent(EventPtr(e));

							EventResult.push(EventPtr(r));
							//left->pop();
						}
						else {
							//no distance, just put new event
							Event* e = new Event(Utilities::id++, win->getInitTime());
							e->addAttr(thisProduct, "true");

							//so it should be left and right already get the anchors
							e->addAttr("anchors", frontLeftEvent->getString("anchors"));

							Event* r = e->clone();

							//push to window
							if (win) {
								//no need to consider about original event --> staright to result
								win->addResultEvent(EventPtr(e));
							}

							EventResult.push(EventPtr(r));

							left->pop();
						}
					}
					if (frontLeftEvent->getInt(key) >= frontRightEvent->getInt(key)) {
						right->pop();
					}
					else if (frontLeftEvent->getInt(key) < frontRightEvent->getInt(key)) {
						left->pop();
					}
				}

				//int c = 10;
				*/
			}
		}
		else {

			//this case if it is still using id to do matching
			//left is less than right - HAVE TO
			queue<EventPtr>* left, * right;
			if (leftInputQueue.first.size() <= rightInputQueue.first.size()) {
				left = &leftInputQueue.first;
				right = &rightInputQueue.first;
				anchorIsAtLeft = true;
			}
			else
			{
				left = &rightInputQueue.first;
				right = &leftInputQueue.first;
				anchorIsAtLeft = false;
			}

			while (1) {
				if (left->size() == 0 || right->size() == 0)
					break;

				if (left->size() == 4294967294 || right->size() == 4294967294)
					break;

				EventPtr frontLeftEvent = left->front();
				EventPtr frontRightEvent = right->front();

				if (frontLeftEvent->getInt(key) == frontRightEvent->getInt(key)) {
					//EventPtr res;

					//if (specialOperation == "distance") { //in case of distance --> need to calculate each time ticks
					//	res = thisSpatialOp->calculate(frontLeftEvent, frontRightEvent);
					//}

					//push to window
					if (win) {
						//ALL SPECIAL OPERATION IS PROCESSED ON SPATIAL CLASS --> so, just push all to window

						if (specialOperation != "") {
							//later i will consider about double buffer
							//win->addEvent(frontLeftEvent, frontRightEvent);

							win->addEvent(frontLeftEvent);
							win->addEvent(frontRightEvent);

							left->pop();
							//right->pop();
						}
						else {
							//ONE OF THEM MUST BE NEWLY CREATED EVENT ._.
							if (frontLeftEvent != frontRightEvent) {
								if (anchorIsAtLeft)
									win->addEvent(frontLeftEvent);
								else
									win->addEvent(frontRightEvent);
							}
							else { //BOTH are original Event
								win->addEvent(frontLeftEvent);
							}
						}
					}

					EventResult.push(frontLeftEvent);

					if (anchorIsAtLeft) {
						//check anchor duplicate
						bool dup = false;
						int tempObjId = frontLeftEvent->getInt("objid");
						for (int i = 0; i < anchorObjId.size(); i++) {
							if (anchorObjId[i] == tempObjId) {
								dup = true;
								break;
							}
						}

						//push
						if (!dup)
							anchorObjId.push_back(frontLeftEvent->getInt("objid"));
					}
					else {
						//check anchor duplicate
						bool dup = false;
						int tempObjId = frontRightEvent->getInt("objid");
						for (int i = 0; i < anchorObjId.size(); i++) {
							if (anchorObjId[i] == tempObjId) {
								dup = true;
								break;
							}
						}

						//push
						if (!dup)
							anchorObjId.push_back(frontRightEvent->getInt("objid"));
					}

				}
				if (frontLeftEvent->getInt(key) >= frontRightEvent->getInt(key)) {
					right->pop();
				}
				else if (frontLeftEvent->getInt(key) < frontRightEvent->getInt(key)) {
					left->pop();
				}
			}
		}
	}
	else if (thisCondition == "or") {
		//OR Statement
	}

	if (win != NULL && specialOperation != "") {
		//Spatial Op
		//ClearResult();
		EventResult = thisSpatialOp->process(win, anchorObjId);
	}

	if (win != NULL && EventResult.size() > 0 && specialOperation == "") {

		//EventResult = *local_win;
		//ClearResult();
		EventResult = win->getFinalRes();

		if (specialOperation == "" && win) {// if we have the window but no specialOp, the window len won't update

			if (win->getOriginalRes().size() < 1) {//if window is empty, so dont update the trigger time
				win->setInitTime(win->getInitTime() + win->getTriggerTime());
			}
		}
	}

	//at the end, re evaluate the spatial operation again -- final re evaluation
	bool shouldKeep = false;
	//PUSH TO PROCEEDING NODE .-.
	if (listOfNextPair.size() > 0) {

		std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
		for (Node* n : listOfNextPair) {
			std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
#ifdef INDEX_ON
			if (dynamic_cast<BetaNode*>(n)) {
				if (dynamic_cast<BetaNode*>(n)->specialOperation == "distance")
					shouldKeep = true;
				else {
					n->pushResult(EventResult, this);
				}
			}

			//size_t detect_distance = n->justCondition.find("distance");
			//if the next node is distance, so stop please. else, pass the result
			else {
				n->pushResult(EventResult, this);
			}

#endif // INDEX_ON

#ifdef INDEX_OFF
			n->pushResult(EventResult, this);
#endif // INDEX_OFF
			//lg.~lock_guard();
		}
	}
	//If it doesn't have, maybe this is the end?
	else {
#ifdef SHOW_RESULT
		cout << "Result from " << thisProduct << " as follows: " << endl;
		queue<EventPtr> local_ptr = EventResult;

		//FOR REPORT ONLY, THE FIRST EVENT ALWAYS DUPLICATE, IDK WHY
		local_ptr.pop();
		//----------------------------------------------------------

		while (local_ptr.size() > 0) {
			cout << *local_ptr.front() << endl;
			local_ptr.pop();
		}
		cout << endl;
#endif // (SHOW_RESULT)

#ifdef ONLY_TRIGGER_RESULT
		output_stream << "-------------------> TRIGGERED event: " << thisProduct
			<< " --- " << Utilities::translateTime(Utilities::getTimeSec()) << endl;
		ReteNet::triggered_ev.push(output_stream.str());

		cout << "-------------------> TRIGGERED event: " << thisProduct
			<< " --- " << Utilities::translateTime(Utilities::getTimeSec()) << endl;
#endif // ONLY_TRIGGER_RESULT

#ifdef ONLY_TRIGGER_TIME_RESULT
		int delta = Utilities::getTime() - Utilities::start_cycle_time;
		cout << "-------------------> TRIGGERED event: " << thisProduct
			<< " processed id: " << Utilities::id << " time spent: " << delta << endl;
#endif // ONLY_TRIGGER_TIME_RESULT

#ifdef SHOW_LAST_NODE_TRIGGER
		if (ReteNet::lastNode() == this->SuperNodeID) {
			int delta = Utilities::getTime() - Utilities::start_cycle_time;
			cout << "-------------------> FINISHED one cycle on: " << thisProduct
				<< " processed id: " << Utilities::id << " time spent: " << delta << endl;
		}
#endif // SHOW_LAST_NODE_TRIGGER


	}

	if (EventResult.size() > 0) {
#ifdef INDEX_ON
		//keep it for the better future :3
		//nothing~
		if (!shouldKeep)
			ClearResult();
#endif // INDEX_ON

#ifdef INDEX_OFF
		//dispose the result
		ClearResult();
#endif // INDEX_OFF
		return 1;
	}
	else {
		return 0;
	}
}

void BetaNode::forcePushInQueue(EventPtr* result, bool toLeft)
//void BetaNode::forcePushInQueue(EventPtr* result, Node * sourceNode)
{
	EventPtr dummy_res = *result;
	if (toLeft) {
		leftInputQueue.first.push(dummy_res);
	}
	else {
		rightInputQueue.first.push(dummy_res);
	}
}

void BetaNode::forcePushInQueue(EventPtr* result, Node* sourceNode)
{
	EventPtr dummy_res = *result;
	if (sourceNode == leftSourcePair.second) {
		leftInputQueue.first.push(dummy_res);
	}
	else {
		rightInputQueue.first.push(dummy_res);
	}
}

void BetaNode::forcePushInEv(EventPtr result)
{
	EventResult.push(result);

	for (auto a : listOfNextPair) {
		if (dynamic_cast<AlphaNode*>(a)) {
			a->pushResult(result, this);
		}
		else if (dynamic_cast<BetaNode*>(a)) {
			if (static_cast<BetaNode*>(a)->getSpecialOpName() != "distance")
				a->pushResult(result, this);
		}
	}
}

void BetaNode::forcePushToPairNodeQueue(EventPtr result)
{
	std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
	//queue<EventPtr> temp;
	//temp.push(result);
	for (auto n : listOfNextPair) {
		n->pushResult(result, this);
	}
}

queue<EventPtr> BetaNode::getEvRes()
{

#ifdef INDEX_ON
	queue<EventPtr> temp_evResult = EventResult;
	ClearResult();
	return temp_evResult;
#endif // INDEX_ON

#ifdef INDEX_OFF
	return EventResult;
#endif // INDEX_OFF



}

void BetaNode::refreshEvent(queue<EventPtr>& inputEvent) {
	long long curr = Utilities::getTime();
	/*while (!inputEvent.empty() && inputEvent.front()->getTime() + winLen < curr) {
		inputEvent.pop();
	}*/
}

long long BetaNode::getExecutionEstimated()
{
	if (win == NULL)
		return -1;
	else
		return win->getInitTime();
}

vector<Node*> BetaNode::getNextPairs()
{
	return this->listOfNextPair;
}

void BetaNode::printQueueLen()
{
	cout << " size left: " << leftInputQueue.first.size() << " right input: " << rightInputQueue.first.size() << endl;
}

void BetaNode::pushResult(queue<EventPtr> result, Node* source)
{
	//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
	//queue<EventPtr> dummy_res = *result;
	if (source == leftInputQueue.second) {
		while (result.size() > 0) {
			leftInputQueue.first.push(result.front());
			validify(result.front());
			result.pop();
		}
	}
	else if (source == rightInputQueue.second) {
		while (result.size() > 0) {
			rightInputQueue.first.push(result.front());
			validify(result.front());
			result.pop();
		}
	}
	/*if (source == leftInputQueue.second)
		leftInputQueue.first = *result;
	else if (source == rightInputQueue.second)
		rightInputQueue.first = *result;*/
}

void BetaNode::pushResult(EventPtr result, Node* source)
{
	//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
	if (source == leftInputQueue.second) {
		leftInputQueue.first.push(result);
	}
	else if (source == rightInputQueue.second) {
		rightInputQueue.first.push(result);
	}

	validify(result);
}

string BetaNode::getProduct()
{
	return thisProduct;
}

int BetaNode::testNode(int TimeSlice, int TimeNow)
{
	//ReteNet::checkTemporalQueue(TimeSlice, TimeNow);

	if (leftSourcePair.second == rightSourcePair.second) {
		if (!win)
			return justTest(TimeSlice);
		else if (win->getOriginalRes().size() > 0 || EventResult.size() > 0) // because the matching part is already done in void::validify
			return justTest(TimeSlice);
	}

	if (leftInputQueue.first.size() > 0 && rightInputQueue.first.size() > 0) {
		if (!win)
			return justTest(TimeSlice);
		else if (win->getOriginalRes().size() > 0 || EventResult.size() > 0)
			return justTest(TimeSlice);

	}
	else if ((specialOperation == "plot_dist"
		|| specialOperation == "radar_dist"
		|| specialOperation == "sonar_dist") && rightInputQueue.first.size() > 0) { //the right one just rect, so it will be empty
		if (!win)
			return justTest(TimeSlice);
		else if (win->getOriginalRes().size() > 0 || EventResult.size() > 0)
			return justTest(TimeSlice);
	}

	if (win)
		win->setInitTime(win->getInitTime() + win->getTriggerTime());
	return 0;
}

//int BetaNode::getID()
//{
//	return id;
//}


int BetaNode::addBetaPair(Node* pair)
{
	int isExist = checkBeta(pair);
	if (isExist == 1)
		return 1;
	else {
		listOfNextPair.push_back(pair);
		return 1;
	}

	return -1;
}

int BetaNode::checkBeta(Node* pair)
{
	for (int i = 0; i < listOfNextPair.size(); i++) {
		if (listOfNextPair[i] == pair)
			return 1;
	}
	return 0;
}

int BetaNode::ClearConnection()
{
	//betaPair = NULL;
	//leftSourceBool = false;
	//rightSourceBool = false;
	termPair.second = NULL;
	termPair.first = "";
	return 1;
}

int BetaNode::ResetNode()
{
	ClearConnection();
	//testRes = {};
	//EventResult;
	listOfNextPair = {};

	ClearResult();
	return 0;
}

int BetaNode::ClearResult()
{
	while (EventResult.size() > 0) {
		EventResult.pop();
	}

	validationForIncomingEvent.clear();

	//queue<EventPtr> empty;
	//swap(EventResult, empty);

	return 1;
}

int BetaNode::ClearAllInputQueue()
{
	leftInputQueue.first = {};
	rightInputQueue.first = {};
	return 1;
}

int BetaNode::ClearInputQueue(bool isLeft = false)
{
	if (isLeft) {
		leftInputQueue.first = {};
	}
	else {
		rightInputQueue.first = {};
	}
	return 1;
}
