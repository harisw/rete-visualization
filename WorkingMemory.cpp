#include "WorkingMemory.h"

queue<EventPtr> WorkingMemory::inputQueues;

void WorkingMemory::clearWM()
{
	while (inputQueues.size() > 0) {
		inputQueues.pop();
	}
}

void WorkingMemory::reduceMemory(int numberFromFront)
{
	int itt = 0;
	for (int i = 0; i < numberFromFront && inputQueues.size() > 0; i++) {
		inputQueues.pop();
	}
}

queue<EventPtr> WorkingMemory::getWMInputQueue()
{
	return inputQueues;
}

void WorkingMemory::cloneInputQueues(queue<EventPtr> clon)
{
	inputQueues = clon;
}

int WorkingMemory::size()
{
	return inputQueues.size();
}

EventPtr WorkingMemory::getFront()
{
	try
	{
		EventPtr* front = &inputQueues.front();

		if (!inputQueues.empty()) {
			if (front == NULL) {
				delete& inputQueues.front();
				inputQueues.pop();
			}
			else {
				return inputQueues.front();
			}
			//return inputQueues.front();
		}
	}
	catch (exception& e) {
		cout << "the problem is :" << e.what() << endl;
	}
}

void WorkingMemory::pop()
{
	inputQueues.pop();
}

void WorkingMemory::pushEvent(EventPtr ev)
{
	inputQueues.push(ev);
}
