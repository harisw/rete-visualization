#pragma once
#include "../RETEwithDoubleNodeIndexing/ReteNet.h"
#include <thread>
#include <iostream>
#include <set>

#include <chrono>

static bool is_stop, is_paused;

class MFCThreadOfDataProcessing
{
public:
	void run();
	std::thread runThread(ReteNet* net, int obj_num);
private:
	int numOfObject;
	ReteNet* theNet;
};

class MFCTHreadOfTemporalProcessing
{
public:
	void run();
	std::thread runThread(ReteNet* net, int obj_num);
private:
	ReteNet* thisNet;

};

class MFCThreadOfDataGenerator
{
public:
	queue<EventPtr> inpQueue;
	void run();
	EventPtr generateSingleEvent(int obj_id, int obj_num, int curr_time);
	std::thread runThread(int obj_num, ReteNet* net, queue<EventPtr>& inp);

private:
	int this_obj_num, this_time_len;
	ReteNet* theNet;
	unordered_map<int, pair<float, float> > objectLocations;

};

class MFC_MultiThread
{
public :
	static void start_sim(int obj_num, queue<EventPtr> inpQueue);
	static void stop_sim();
	static void pause_sim();
	static void continue_sim();
};

