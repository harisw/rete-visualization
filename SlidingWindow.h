#pragma once
#include "Event.h"
#include <queue>
#include "Utilities.h"
#include <assert.h>
#include <thread>

class SlidingWindow
{
public:
	SlidingWindow(long long len, long long step = -1);
	void refresh(queue<EventPtr>& eventRes);

	void refreshSet(set<int>& eventSet, queue<EventPtr>& eventPtr);

	void refreshDouble();
	void addEvent(EventPtr e);
	void addResultEvent(EventPtr e);
	void addEvent(EventPtr a, EventPtr b);

	int duplicateCheck(EventPtr a, queue<EventPtr> eventRes);
	int duplicateCheck(EventPtr a, EventPtr b);

	queue<EventPtr> getFinalRes();
	queue<EventPtr> getOriginalRes();
	long long getLowestOriginalTime();
	long long getHigheststOriginalTime();
	long long getInitTime();
	long long setInitTime(long long newTime);
	long long getTriggerTime();
	queue<pair<EventPtr, EventPtr>> getDoubleRes();

private:
	queue<EventPtr> win_buffer;
	queue<EventPtr> win_res_buffer;

	//they said by using set, it will not insert the duplicate automatically
	set<int> set_win_buffer_id;
	set<int> set_win_res_buffer_id;

	queue<pair<EventPtr, EventPtr>> double_win_buffer;
	long long initTime = 0;

	std::recursive_mutex sliding_window_mutex;//mutex lock

	long long winLen = -1, winStep = -1;
};

