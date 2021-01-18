#pragma once
#include "Node.h"
//#include "TerminalNode.h"
#include "SlidingWindow.h"
#include "ReteNet.h"
#include "SpatialOp.h"

#include "Utilities.h"

#include <string>
#include <queue>
//#include "../execution/EventProcess.h"

class BetaNode : public Node
{
public:
	//Function from Node
	int testNode(int TimeSlice, int TimeNow);
	//int getID();
	string getType();
	Node* getSinglePair(int i);
	vector<Node*> getAllPairs();
	bool prevNodeAlreadyConnected();

	//bool isEmptyResult();
	void pushResult(queue<EventPtr> result, Node* inputNode);
	void pushResult(EventPtr result, Node* inputNode);

	//BetaNode();
	BetaNode(int id, string condition);

	//connecting beta & terminal
	int addBetaPair(Node* pair);
	int checkBeta(Node* pair);

	//Init function
	int ClearConnection();
	int ResetNode();
	int ClearResult();
	int ClearAllInputQueue();
	int ClearInputQueue(bool isLeft);

	//getSet Function
	string getProduct();
	Node* getLeftConnNode();
	Node* getRightConnNode();
	string getLeftConnName();
	string getRightConnName();
	string getSpecialOpName();
	queue<EventPtr> getLeftInput();
	queue<EventPtr> getRightInput();
	//NEW METHOD--------------------------------------
	void validify(EventPtr);

	float getSpatialLimFloat();

	void setWindow(int len, int step);
	void setMatchingKey(string newKey);
	void setAggregateCondition(string expression);

	int setLeftConnection(Node* node);
	int setRightConnection(Node* node);

	bool isEmptyResult();
	int justTest(int timeSlice);
	void forcePushInQueue(EventPtr* result, bool toLeft = true);
	void forcePushInQueue(EventPtr* result, Node* sourceNode);

	void forcePushInEv(EventPtr result);
	void forcePushToPairNodeQueue(EventPtr result);
	//void forcePushInQueue(EventPtr* result, Node * sourceNode);

	queue<EventPtr> getEvRes();

	//WINDOW PART
	void refreshEvent(queue<EventPtr>& inputEvent);
	long long getExecutionEstimated();
	vector<Node*> getNextPairs();

private:
	//int id;
	string thisCondition; //AND OR
	string thisProduct; //Then expression or RHS
	string specialOperation, specialOperationLeft, specialOperationRight;
	string key = "id";

	pair<string, Node*> leftSourcePair, rightSourcePair, termPair;
	pair<queue<EventPtr>, Node*> leftInputQueue, rightInputQueue;

	queue<EventPtr> EventResult;
	//TerminalNode* terminalPair;

	//Connected Pairs 
	vector<Node*> listOfNextPair;

	//WINDOW PART
	//int winLen = -1, winSlide = -1;
	//long long nextTime = -1;
	SlidingWindow* win;

	//MISCS
	queue<Event> createdEvent;
	void printQueueLen();
	//id, the event
	//id, count
	unordered_map<int, int> validationForIncomingEvent;

	//Spatial Operator
	SpatialOp* thisSpatialOp;

	//Extra condition to handle exist, sum, and so on ._.

};

