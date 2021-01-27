#pragma once
#include <vector>
#include <iostream>

#include <unordered_map>

#include "ReteNet.h"
#include "MFC_FixedMultiThread.h"
#include <thread>

using namespace std;

class ThreadFixedDataGeneration
{
public:
	void setupTheFixedData(unordered_map<int, vector<pair<float, float>>> inp);
	void run();
	EventPtr generateFixedEvent(int obj_id, int obj_num, int curr_time);
	std::thread runThread(int obj_num, ReteNet* net);

private:
	unordered_map<int, vector<pair<float, float>>> local_objMap;
	int this_obj_num, this_time_len;
	ReteNet* theNet;
};

class ThreadFixedDataProcessing
{
public:
	void run();
	std::thread runThread(ReteNet* net, int obj_num);
private:
	int numOfObject;
	ReteNet* theNet;
};

class ThreadFixedTemporalProcessing
{
public:
	void run();
	std::thread runThread(ReteNet* net, int obj_num);
private:
	ReteNet* thisNet;

};

class MFC_FixedMultiThread
{
public:
	static void setupData(int obj_id, vector<pair<float, float>> trajectoryList);
	static void start(int obj_num);
	static void startWithVisual(int obj_num);

	static unordered_map<int, vector<pair<float, float>>> objectLocationMap;
};