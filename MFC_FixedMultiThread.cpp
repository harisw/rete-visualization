#include "MFC_FixedMultiThread.h"

#define FUTURE_TOLLERANCE_IN_SEC 10

//this is where to fix beteween cycle and command prompt
#define SLEEP_GAP 1000

static int global_curr_itt;

static bool is_stop, is_paused;

unordered_map<int, vector<pair<float, float>>> MFC_FixedMultiThread::objectLocationMap;

void MFC_FixedMultiThread::setupData(int obj_id, vector<pair<float, float>> trajectoryList)
{
	for (auto tr : trajectoryList) {
		MFC_FixedMultiThread::objectLocationMap[obj_id].push_back(tr);
	}
}

void MFC_FixedMultiThread::start(int obj_num)
{
	is_stop = false;
	is_paused = false;

	//make a dummy they said
	ReteNet theRete;
	theRete.buildNetNode();

	//Data Generator
	ThreadFixedDataGeneration* d = new ThreadFixedDataGeneration();
	d->setupTheFixedData(objectLocationMap);
	d->runThread(obj_num, &theRete).detach();

	////Data process
	ThreadFixedDataProcessing* p = new ThreadFixedDataProcessing();
	p->runThread(&theRete, obj_num).detach();

	////Temporal Processing
	ThreadFixedTemporalProcessing* tp = new ThreadFixedTemporalProcessing();
	tp->runThread(&theRete, global_curr_itt).detach();
}

void MFC_FixedMultiThread::startWithVisual(int obj_num)
{
	//make a dummy they said
	ReteNet theRete;
	theRete.buildNetNode();

	//Data Generator
	ThreadFixedDataGeneration* d = new ThreadFixedDataGeneration();
	d->setupTheFixedData(objectLocationMap);
	d->runThread(obj_num, &theRete).detach();
}

void ThreadFixedDataGeneration::setupTheFixedData(unordered_map<int, vector<pair<float, float>>> inp)
{
	local_objMap = inp;
}

void ThreadFixedDataGeneration::run()
{
	//time now is useless
	int time_now = 0;// , curr_obj_id = 0;
	long long startTime = Utilities::getTime();

	//Utilities::start_cycle_time = Utilities::getTime();// +1000;
	//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock

	while (global_curr_itt < local_objMap[0].size()) {
		for (auto map : local_objMap) {
			EventPtr e = generateFixedEvent(map.first, this_obj_num, global_curr_itt);

			if (e != nullptr) {
				std::unique_lock<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
				//ReteNet::pushToWM(e);
				theNet->pushToWM(e);
			}

			if (map.first == this_obj_num - 1) {

#ifdef CALCULATE_BASED_ON_DATA_GENERATION
				long long delta = Utilities::getTime() - startTime;
				cout << "on Cycle: " << global_curr_itt << " time spent: " << delta << endl;// << " current id: " << Utilities::id << endl;
				//cout << "Data Generated in cycle: " << global_curr_itt << " current id: " << Utilities::id << endl;
#endif // CALCULATE_BASED_ON_DATA_GENERATION

				cout << "Data Generated in cycle: " << global_curr_itt << endl;

				//time_now++;
				global_curr_itt++;// = time_now;
				//Utilities::time = global_curr_time;

				//std::this_thread::sleep_for(std::chrono::milliseconds(this_obj_num / 10));
				//std::this_thread::sleep_for(std::chrono::milliseconds(1000));


				std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_GAP));

				//Utilities::start_cycle_time = Utilities::getTime();// +1000;
				startTime = Utilities::getTime();
			}
		}
	}
	ReteNet::is_execution_done = true;
	//	while (global_curr_itt < local_objMap.size()) {
	//		/*if (is_paused) {
	//			continue;
	//		}*/
	//		//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
	//		//long long tempStart = Utilities::getTime();
	//
	//		EventPtr e = generateFixedEvent(curr_obj_id, this_obj_num, global_curr_itt);
	//
	//		if (e != nullptr) {
	//			std::unique_lock<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
	//			//ReteNet::pushToWM(e);
	//			theNet->pushToWM(e);
	//		}
	//
	//		if (curr_obj_id == this_obj_num) {
	//			curr_obj_id = 0;
	//
	//#ifdef CALCULATE_BASED_ON_DATA_GENERATION
	//			long long delta = Utilities::getTime() - startTime;
	//			cout << "on Cycle: " << global_curr_itt << " time spent: " << delta << endl;// << " current id: " << Utilities::id << endl;
	//			//cout << "Data Generated in cycle: " << global_curr_itt << " current id: " << Utilities::id << endl;
	//#endif // CALCULATE_BASED_ON_DATA_GENERATION
	//
	//			cout << "Data Generated in cycle: " << global_curr_itt << endl;
	//
	//			//time_now++;
	//			global_curr_itt++;// = time_now;
	//			//Utilities::time = global_curr_time;
	//
	//			//std::this_thread::sleep_for(std::chrono::milliseconds(this_obj_num / 10));
	//			//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_GAP));
	//
	//			//Utilities::start_cycle_time = Utilities::getTime();// +1000;
	//			startTime = Utilities::getTime();
	//		}
	//		else {
	//			curr_obj_id++;
	//		}

			//cout << "this is wm: " << ReteNet::getWMSize() << endl;
	//	}

}

EventPtr ThreadFixedDataGeneration::generateFixedEvent(int obj_id, int obj_num, int curr_time)
{
	Event* e = new Event(Utilities::id++, Utilities::getTimeSec() + FUTURE_TOLLERANCE_IN_SEC);

	if (obj_id == 0) {
		e->addAttr("iff", "true"); // scalar
	}
	else {
		e->addAttr("iff", "false");
	}

	e->addAttr("type", "recon"); //sclar
	if (obj_id < (1 / obj_num)) { //vessel

		if (curr_time == 0) {
			float dist = 10;
			e->addAttr("speed", dist); //scalar
			float ele = (float)Utilities::randomFloat(0, 10);
			e->addAttr("elevation", ele); //scalar
		}
		else {
			float dist = sqrt(pow(2, (local_objMap[obj_id][curr_time].first - local_objMap[obj_id][curr_time - 1].first)) + pow(2, (local_objMap[obj_id][curr_time].second - local_objMap[obj_id][curr_time - 1].second)));

			e->addAttr("speed", dist); //scalar
			float ele = (float)Utilities::randomFloat(0, 10);
			e->addAttr("elevation", ele); //scalar
		}

		//e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
		//float ele = (float)Utilities::randomFloat(0, 10);
		//e->addAttr("elevation", ele); //scalar
	}
	else if (obj_id < (2 / obj_num)) { //submarine
		if (curr_time == 0) {
			float dist = 10;
			e->addAttr("speed", dist); //scalar
			float ele = (float)Utilities::randomFloat(-10, 0);
			e->addAttr("elevation", ele); //scalar
		}
		else {
			float dist = sqrt(pow(2, (local_objMap[obj_id][curr_time].first - local_objMap[obj_id][curr_time - 1].first)) + pow(2, (local_objMap[obj_id][curr_time].second - local_objMap[obj_id][curr_time - 1].second)));

			e->addAttr("speed", dist); //scalar
			float ele = (float)Utilities::randomFloat(-10, 0);
			e->addAttr("elevation", ele); //scalar
		}
		//e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
		//float ele = (float)Utilities::randomFloat(-10, 0);
		//e->addAttr("elevation", ele); //scalar
	}
	else { //aircraft

		if (curr_time == 0) {
			float dist = 10;
			e->addAttr("speed", dist); //scalar
			float ele = (float)Utilities::randomFloat(5, 30);
			e->addAttr("elevation", ele); //scalar
		}
		else {
			float dist = sqrt(pow(2, (local_objMap[obj_id][curr_time].first - local_objMap[obj_id][curr_time - 1].first)) + pow(2, (local_objMap[obj_id][curr_time].second - local_objMap[obj_id][curr_time - 1].second)));

			e->addAttr("speed", dist); //scalar
			float ele = (float)Utilities::randomFloat(5, 30);
			e->addAttr("elevation", ele); //scalar
		}
		//e->addAttr("speed", (float)Utilities::randomFloat(10, 40)); //scalar
		//float ele = (float)Utilities::randomFloat(5, 30);
		//e->addAttr("elevation", ele); //scalar
	}

	//if (obj_id % obj_num == 0) { // our vessel doesnt move
	//	//e->addAttr("lon", 125); //spatial
	//	//e->addAttr("lat", 35); //spatial
	//	e->addAttr("lon", local_objMap[obj_id][curr_time].first);
	//	e->addAttr("lon", local_objMap[obj_id][curr_time].second);
	//}
	//else {
	//	e->addAttr("lon", local_objMap[obj_id][curr_time].first);
	//	e->addAttr("lon", local_objMap[obj_id][curr_time].second);
	//}

	e->addAttr("lat", local_objMap[obj_id][curr_time].first);
	e->addAttr("lon", local_objMap[obj_id][curr_time].second);

	e->addAttr("electromagnet", "false");

	e->addAttr("objid", obj_id);

	return EventPtr(e);
}

std::thread ThreadFixedDataGeneration::runThread(int obj_num, ReteNet* net)
{
	theNet = net;
	this_obj_num = obj_num;
	return std::thread(&ThreadFixedDataGeneration::run, this);
}

void ThreadFixedDataProcessing::run()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(800)); //sleep for safety
	long long startTime, tempTime;
	bool once = false;
	int counter = 0;
	startTime = Utilities::getTime();
	int local_time = 0;

#ifdef CALCULATE_BASED_ON_DATA_PROCESSING
	Utilities::start_cycle_time = Utilities::getTime() + 800;
#endif // CALCULATE_BASED_ON_DATA_PROCESSING

	//cout << "First time data process: " << Utilities::translateTime(Utilities::getTimeSec()) << endl;
	//cout << "this is data process" << endl;
	while (!is_stop) {
		if (is_paused) {
			continue;
		}

		theNet->ExecuteRete(numOfObject, 0);

		theNet->NewSpatioTemporalExecution(numOfObject, 0);


		if (local_time != global_curr_itt) {

			//int delta = Utilities::getTime() - startTime + 1000; //1000 is 1 sec delay

			//cout << "indexing : " << global_curr_itt << " time spent: " << delta << endl; // " current id: " << Utilities::id << " counter num: " << counter << endl;
			//cout << "indexing : " << global_curr_itt << " processed id: " << Utilities::id << endl;
			local_time = global_curr_itt;
			counter = 0;
#ifdef CALCULATE_BASED_ON_DATA_PROCESSING
			Utilities::start_cycle_time = Utilities::getTime();// +1000;
#endif // CALCULATE_BASED_ON_DATA_PROCESSING
			//startTime = Utilities::getTime();
		}//cout << "this is data process" << endl;

		counter++;
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//}
	}
	//ReteNet::is_execution_done = true;
}

std::thread ThreadFixedDataProcessing::runThread(ReteNet* net, int obj_num)
{
	numOfObject = obj_num;
	theNet = net;
	return std::thread(&ThreadFixedDataProcessing::run, this);
}

void ThreadFixedTemporalProcessing::run()
{
	long long timeStart = Utilities::getTime();
	bool once = false;
	int local_time = 0;
	int counter = 0;

	std::this_thread::sleep_for(std::chrono::milliseconds(80));

	while (!is_stop) {
		if (is_paused) {
			continue;
		}

		//sleep to be safe

		//timeStart = Utilities::getTime();

		thisNet->checkTemporalQueue(100, global_curr_itt);

		if (local_time != global_curr_itt) {
			int delta = Utilities::getTime() - timeStart;

			//int delta = Utilities::getTime() - Utilities::start_cycle_time;
			//cout << "temporal : " << global_curr_itt << " time spent: " << delta <<endl;// << " current id: " << Utilities::id << " counter num: " << counter << endl;
			//cout << "temporal : " << global_curr_itt << " processed id: " << Utilities::id << endl;
			local_time = global_curr_itt;
			counter = 0;
			//timeStart = Utilities::getTime() + 1000;
		}//cout << "this is data process" << endl;
		counter++;
		//cout << "temporal processing spent: " << Utilities::getTime() - timeStart << " when current time: " << global_curr_time << endl;
//		}
	}
}

std::thread ThreadFixedTemporalProcessing::runThread(ReteNet* net, int obj_num)
{
	thisNet = net;
	//this_time_now = time_now;
	return std::thread(&ThreadFixedTemporalProcessing::run, this);
}
