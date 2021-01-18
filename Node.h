#pragma once

#include <iostream>
#include <vector>
#include "Event.h"
#include <queue>

using namespace std;


#ifndef NODE_H
#define NODE_H
class Node
{
public:
	int SuperNodeID;
	Node(int id, string condition);
	void setNewMasterID(int newId);
	//Node();
	//virtual int getID() = 0;
	int getID();
	virtual string getType() = 0;
	virtual bool isEmptyResult() = 0;

	virtual vector<Node*> getAllPairs() = 0;
	virtual Node* getSinglePair(int i) = 0;
	virtual int addBetaPair(Node* pair) = 0;
	virtual bool prevNodeAlreadyConnected() = 0;
	virtual void setWindow(int range, int step) = 0;

	virtual void pushResult(queue<EventPtr> result, Node*) = 0;
	virtual void pushResult(EventPtr result, Node*) = 0;

	virtual int testNode(int TimeSlice, int TimeNow) = 0;

	virtual queue<EventPtr> getEvRes() = 0;

	//Window thing
	virtual long long getExecutionEstimated() = 0;

	//debug purpose
	virtual void printQueueLen() = 0;

	string justCondition;

	//visualization reqs
	pair<int, int> visualPosition;
	virtual vector<Node*> getNextPairs() = 0;

};


#endif // !NODE_H