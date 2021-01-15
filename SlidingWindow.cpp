#include "SlidingWindow.h"
//#define NO_THREADING

#define USE_SECOND
//#define USE_MILLISEC

SlidingWindow::SlidingWindow(long long len, long long step)
{
	winStep = step;//think later
#ifdef USE_SECOND
	initTime = step + Utilities::getTimeSec();
	winLen = len;
#endif // USE_SECOND

#ifdef USE_MILLISEC
	initTime = (step * 1000) + Utilities::getTime();
	winLen = len * 1000;
#endif // USE_MILLISEC


}

void SlidingWindow::refresh(queue<EventPtr>& eventRes)
{
	if (eventRes.size() <= 0)
		return;

#ifdef NO_THREADING
	int latest_time = eventRes.back()->getInt("time"); // time n until n - len
#else
	//long long latest_time = Utilities::time;

#ifdef USE_SECOND
	long long latest_time = Utilities::getTimeSec();
#endif // USE_SECOND
#ifdef USE_MILLISEC
	long long latest_time = Utilities::getTime();
#endif // USE_MILISEC



#endif // NO_THREADING

	//int latest_time = eventRes.back()->getInt("time"); // time n until n - len
	//long long latest_time = Utilities::time;

	//FOR GREATER GOOD :"
	if (eventRes.front()->map_size() > 999999 || eventRes.front()->map_size() < -1) {
		eventRes.pop();
	}
	//int a = 11;
	/*while (!eventRes.empty() && eventRes.front()->getInt("time") <= latest_time - winLen) {
		eventRes.pop();
	}*/

	while (!eventRes.empty()) {
		if (eventRes.front()->map_size() < -1) {
			eventRes.pop();
			continue;
		}
		if (eventRes.front()->getTime() <= latest_time - winLen)
			eventRes.pop();
		else
			break;
	}

	/*long long curr = Utilities::getTime();
	while (win_buffer.empty() && win_buffer.front()->getTime() + winLen < curr) {
		win_buffer.pop();
	}*/
}

void SlidingWindow::refreshSet(set<int>& eventRes, queue<EventPtr>& eventPtr)
{
	if (eventRes.size() <= 0 || eventPtr.size() <= 0)
		return;

	std::set<int>::iterator it;
	int front_id = eventPtr.front()->getId();
	int delete_count = 0;
	//matchings

	//it = eventRes.begin();
	//for (int i = 0; i < eventPtr.size(); i++) {
	//	if (*it != front_id) {
	//		eventRes.erase(it);
	//		it++;
	//	}
	//	else
	//		break;
	//}

	for (it = eventRes.begin(); it != eventRes.end(); ++it) {
		if (*it != front_id) {
			eventRes.erase(it);
		}
		else
			break;
	}

	//it = eventRes.begin();
	//for (int i = 0; i < delete_count; i++) {
	//	eventRes.erase(it);
	//	it++;
	//}
}

void SlidingWindow::refreshDouble()
{
	long long curr = Utilities::getTime();
	while (double_win_buffer.empty() && double_win_buffer.front().first->getTime() + winLen < curr) {
		double_win_buffer.pop();
	}
}

void SlidingWindow::addEvent(EventPtr e)
{
	//Event* ec = e->clone();
	/*
	if (win_buffer.size() > 0) {
	//	std::sort(win_buffer->front(), win_buffer->back());
		//duplicate check
		int duplicate = duplicateCheck(EventPtr(ec), win_buffer);

		if(duplicate == 0)
			win_buffer.push(EventPtr(ec));
	}
	else
		win_buffer.push(EventPtr(ec));
	*/

	//comparison method ,lol :v
	int event_id = e->getId();
	int set_len = set_win_buffer_id.size();
	set_win_buffer_id.insert(event_id);
	//no duplicate :v
	if (set_len != set_win_buffer_id.size()) {
		win_buffer.push(e);
	}

	//if (win_buffer.size() > 0) {
	////	std::sort(win_buffer->front(), win_buffer->back());
	//	//duplicate check
	//	int duplicate = duplicateCheck(e, win_buffer);
	//	
	//	if(duplicate == 0)
	//		win_buffer.push(e);
	//}
	//else
	//	win_buffer.push(e);
	//
	refresh(win_buffer);

	//set_win_buffer.insert(EventPtr(ec));
	//refreshSet(set_win_buffer_id, win_buffer);
}

void SlidingWindow::addResultEvent(EventPtr e)
{
	std::lock_guard<std::recursive_mutex> guard(sliding_window_mutex);//mutex lock
	//Event *ev = e->clone();
	//EventPtr ep;// = Event(ev);
	//EventPtr(ev);

	//if (win_res_buffer.size() > 0) {
	//	//	std::sort(win_buffer->front(), win_buffer->back());
	//	
	//	//duplicate check
	//	/*Event* backup_event = e->clone();
	//	int duplicate = duplicateCheck(EventPtr(ev), win_res_buffer);

	//	if (duplicate == 0)
	//		win_res_buffer.push(EventPtr(backup_event));*/

	//	//without dup check
	//		win_res_buffer.push(EventPtr(ev));
	//}
	//else
	win_res_buffer.push(EventPtr(e));

	refresh(win_res_buffer);
}

void SlidingWindow::addEvent(EventPtr a, EventPtr b)
{
	double_win_buffer.push({ a, b });
	refreshDouble();
}

int SlidingWindow::duplicateCheck(EventPtr a, queue<EventPtr> eventRes)
{
	queue<EventPtr> tempQueue = eventRes;

	for (; tempQueue.size() > 0; tempQueue.pop()) {

		if (tempQueue.front()->map_size() < 0) {
			tempQueue.pop();
			/*delete& tempQueue.front();
			tempQueue.pop();*/

			if (tempQueue.size() == 0)
				return 0;
			else
				continue;
		}

		if (a->getId() == tempQueue.front()->getId())
			return 1;
	}
	return 0;
}

int SlidingWindow::duplicateCheck(EventPtr a, EventPtr b)
{
	queue<pair<EventPtr, EventPtr>> tempQueue = double_win_buffer;
	//while (tempQueue.size > 0) {
	//	if (a == tempQueue.front())
	//		return 0;
	//	tempQueue.pop();
	//}
	return 1;
}

queue<EventPtr> SlidingWindow::getFinalRes()
{
	refresh(win_res_buffer);

	//for(;win_res_buffer.size() > 0; )
	set_win_buffer_id.clear();

	return win_res_buffer;
}

queue<EventPtr> SlidingWindow::getOriginalRes()
{
	refresh(win_buffer);
	return win_buffer;
}

long long SlidingWindow::getLowestOriginalTime()
{
	if (win_buffer.size() > 0) {
		return win_buffer.front()->getTime();
	}
	return -1;
}

long long SlidingWindow::getHigheststOriginalTime()
{
	if (win_buffer.size() > 0) {
		return win_buffer.back()->getTime();
	}
	return -1;
}

long long SlidingWindow::getInitTime()
{
	return initTime;
}

long long SlidingWindow::setInitTime(long long newTime)
{
	initTime = newTime;
	return 1;
}

long long SlidingWindow::getTriggerTime()
{
	return winStep;
}

queue<pair<EventPtr, EventPtr>> SlidingWindow::getDoubleRes()
{
	refreshDouble();
	return double_win_buffer;
}
