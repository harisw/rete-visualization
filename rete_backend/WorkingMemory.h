#pragma once
#include <vector>
#include <queue>
#include <string>
#include "Event.h"

using namespace std;

class WorkingMemory
{
public:
	WorkingMemory() {};
	void clearWM();
	void reduceMemory(int numberFromFront);
	queue<EventPtr> getWMInputQueue();
	void cloneInputQueues(queue<EventPtr> clon);
	int size();
	EventPtr getFront();
	void pop();

private:
	string wmName;

	queue<EventPtr> inputQueues;
};

