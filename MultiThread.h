#pragma once

#include "ReteNet.h"
#include <thread>
#include <iostream>
#include <set>

#include <chrono>

using namespace std;

static bool is_stop = false;

class ThreadOfDataGenerator
{
public:
	void run();
	EventPtr generateSingleEvent(int obj_id, int obj_num, int curr_time);
	std::thread runThread(int obj_num, int time_len, ReteNet* net);
	unordered_map<int, pair<float, float> > objectLocations;

private:
	int this_obj_num, this_time_len;
	ReteNet* theNet;
};

class ThreadOfDataProcess
{
public:
	void run();
	std::thread runThread(ReteNet* net, int obj_num);
private:
	int time_now;
	int numOfObject;
	ReteNet* theNet;
};

class ThreadOfTemporalProcessing
{
public:
	void run();
	void runSingleNode(int nodeId);
	void runSeparately();
	vector<std::thread> singleNodeThread;
	std::thread runThread(ReteNet* net, int time_now);

private:
	ReteNet* thisNet;
	int this_time_now;

};

class MultiThread
{
public:
	static void StartBoth(int obj_num, int time_len);
	static void StartSingle(int obj_num, int time_len);



};

