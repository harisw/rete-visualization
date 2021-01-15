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
	static void clearWM();
	static void reduceMemory(int numberFromFront);
	static queue<EventPtr> getWMInputQueue();
	static void cloneInputQueues(queue<EventPtr> clon);
	int size();
	EventPtr getFront();
	static void pop();
	static void pushEvent(EventPtr ev);

private:
	string wmName;

	static queue<EventPtr> inputQueues;
};

