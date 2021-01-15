#include "MultiThread.h"

#define USE_SECOND
#define USE_MILLISEC

static int global_curr_time;
static int global_itt_time;
static int fastest = 99999, longest = -1;

void ThreadOfDataGenerator::run()
{
	int time_now = 0, curr_obj_id = 0;
	long long startTime = Utilities::getTime();

	//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock

	while (time_now < this_time_len) {
		//while (!is_stop) {
			//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
		long long tempStart = Utilities::getTime();

		EventPtr e = generateSingleEvent(curr_obj_id, this_obj_num, time_now);

		/*queue<EventPtr> singlePoint = {};
		singlePoint.push(e);
		ReteNet::cloneToWm(singlePoint);*/
		if (e != nullptr) {
			std::unique_lock<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
			//ReteNet::pushToWM(e);
			theNet->pushToWM(e);
			lg.unlock();
		}

		if (curr_obj_id > this_obj_num) {
			curr_obj_id = 0;
			int delta = Utilities::getTime() - startTime;

			cout << "Generated data at: " << global_curr_time << " time spent: " << delta << " current id: " << Utilities::id << endl;

			time_now++;
			global_curr_time = time_now;
			Utilities::time = global_curr_time;

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			startTime = Utilities::getTime();
		}
		else {
			curr_obj_id++;
		}

		//cout << "this is wm: " << ReteNet::getWMSize() << endl;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	is_stop = true;
}

EventPtr ThreadOfDataGenerator::generateSingleEvent(int obj_id, int obj_num, int curr_time)
{
	//Event* e = new Event(Utilities::id++, curr_time);
	Event* e = new Event(Utilities::id++, Utilities::getTimeSec());

	auto search = objectLocations.find(obj_id);
	bool isExist = search != objectLocations.end() ? true : false;

	if (obj_id == 0) {
		e->addAttr("iff", "true"); // scalar
	}
	else {
		e->addAttr("iff", "false");
	}

	e->addAttr("type", "recon"); //sclar
	if (obj_id < (1 / obj_num)) { //vessel
		e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
		float ele = (float)Utilities::randomFloat(0, 10);
		e->addAttr("elevation", ele); //scalar
	}
	else if (obj_id < (2 / obj_num)) { //submarine
		e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
		float ele = (float)Utilities::randomFloat(-10, 0);
		e->addAttr("elevation", ele); //scalar
	}
	else { //aircraft
		e->addAttr("speed", (float)Utilities::randomFloat(10, 40)); //scalar
		float ele = (float)Utilities::randomFloat(5, 30);
		e->addAttr("elevation", ele); //scalar
	}

	if (obj_id % obj_num == 0) { // our vessel doesnt move
		e->addAttr("lon", 125); //spatial
		e->addAttr("lat", 35); //spatial
		objectLocations[obj_id] = make_pair(125, 35);
	}
	else {
		float lon, lat;
		if (isExist) {
			if (obj_id % 2 == 1) {
				lon = objectLocations[obj_id].first - (float)Utilities::randomFloat(1, 5);
				lat = objectLocations[obj_id].second - (float)Utilities::randomFloat(1, 5);
			}
			else {
				lon = objectLocations[obj_id].first + (float)Utilities::randomFloat(1, 5);
				lat = objectLocations[obj_id].second + (float)Utilities::randomFloat(1, 5);
			}
		}
		else {
			lon = objectLocations[obj_id].first - (float)Utilities::randomFloat(120, 131);
			lat = objectLocations[obj_id].second - (float)Utilities::randomFloat(30, 41);
		}
		objectLocations[obj_id] = make_pair(lon, lat);
		e->addAttr("lon", lon); //spatial
		e->addAttr("lat", lat); //spatial
	}

	e->addAttr("electromagnet", "false");

	e->addAttr("objid", obj_id);

	return EventPtr(e);
}

std::thread ThreadOfDataGenerator::runThread(int obj_num, int time_len, ReteNet* net)
{
	theNet = net;
	this_obj_num = obj_num;
	this_time_len = time_len;
	return std::thread(&ThreadOfDataGenerator::run, this);
}

void ThreadOfDataProcess::run()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //sleep for safety
	long long startTime, tempTime;
	bool once = false;
	int counter = 0;
	startTime = Utilities::getTime();
	int local_time = 0;
	cout << "First time data process: " << Utilities::translateTime(Utilities::getTimeSec()) << endl;
	//cout << "this is data process" << endl;
	while (!is_stop) {
		//std::this_thread::sleep_for(std::chrono::milliseconds(5));
		//cout << "this is happening at : " << counter << " wm: " << ReteNet::getWMSize() << " is Stop : "<< is_stop<< endl;
		//cout << "this is happening at : " << counter << " wm: " << theNet->getWMSize() << " is Stop : " << is_stop << endl;
		//if (ReteNet::getWMSize() > 0) {

		//std::lock_guard<std::recursive_mutex> lg(theNet->mutexOfReteProcess);//mutex lock
		//if (theNet->getWMSize() > 0) {

			//std::lock_guard<std::recursive_mutex> lg(theNet->mutexOfReteProcess);//mutex lock

			//tempTime = Utilities::getTime();
			//ordinary nodes
		theNet->ExecuteRete(numOfObject, global_curr_time);
		//cout << "multi dim time : " << Utilities::getTime() - tempTime << endl;

		//tempTime = Utilities::getTime();
		//this is spatiotemporal
		theNet->NewSpatioTemporalExecution(numOfObject, global_curr_time);
		//cout << "spatial time : " << Utilities::getTime() - tempTime << endl;


		if (local_time != global_curr_time) {

			int delta = Utilities::getTime() - startTime - 1000; //1000 is 1 sec delay

			if (once) {
				if (delta > longest) {
					longest = delta;
				}
				if (delta < fastest) {
					fastest = delta;
				}
			}

			if (!once) {
				once = true;
			}

			cout << "this is happening at indexing : " << global_curr_time << " time spent: " << delta << " current id: " << Utilities::id << " counter num: " << counter << endl;
			local_time = global_curr_time;
			counter = 0;
			startTime = Utilities::getTime();
		}//cout << "this is data process" << endl;

		counter++;
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//}
	}
}

std::thread ThreadOfDataProcess::runThread(ReteNet* net, int obj_num)
{
	numOfObject = obj_num;
	theNet = net;
	return std::thread(&ThreadOfDataProcess::run, this);
}

void MultiThread::StartBoth(int obj_num, int time_len)
{
	//make a dummy they said
	ReteNet theRete;

	ThreadOfDataGenerator* d = new ThreadOfDataGenerator();
	d->runThread(obj_num, time_len, &theRete).detach();
	//std::thread tr1 = d->runThread(obj_num, time_len).detach();

	long long startTime = Utilities::getTime();

	ThreadOfDataProcess* p = new ThreadOfDataProcess();
	//p->runThread(&theRete, obj_num).detach();
	std::thread tr2 = p->runThread(&theRete, obj_num);

	ThreadOfTemporalProcessing* tp = new ThreadOfTemporalProcessing();
	tp->runThread(&theRete, global_curr_time).detach();

	tr2.join();

	//cin.get();
	//cin.get();
	//is_stop = true;

	//std::this_thread::sleep_for(std::chrono::seconds(60));
	//is_stop = true;
	//if (is_stop) {
	cout << "time spent: " << Utilities::getTime() - startTime << endl;
	cout << "avg time: " << (Utilities::getTime() - startTime) / time_len << " fastest : " << fastest << " longest : " << longest << endl;

	system("pause");
	//}

	//tr1.join();
}

/*
void MultiThread::StartSingle(int obj_num, int time_len)
{
	ReteNet theRete;

	//thread for processing
	ThreadOfDataProcess* p = new ThreadOfDataProcess();
	//p->runThread().detach();
	std::thread tr2 = p->runThread(&theRete, obj_num);

	//data generation
	int time_now = 0, curr_obj_id = 0;

	while (time_now < time_len) {
		//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
		long long tempStart = Utilities::getTime();

		//EventPtr e = generateSingleEvent(curr_obj_id, obj_num, time_now);
		EventPtr* e;

		if (e != nullptr) {
			//ReteNet::pushToWM(e);
			theRete.pushToWM(e);
		}

		if (curr_obj_id > obj_num) {
			curr_obj_id = 0;

			if (time_now % 10 == 0) {
				//cout << "processing at time: " << time_now << endl;
				//cout << "processing at time: " << time_now << "data size" <<ReteNet::getWMSize() << endl;
				//cout << "processing at time: " << time_now << "data size" << theNet->getWMSize() << endl;
			}
			time_now++;
			global_curr_time = time_now;

			//std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		else {
			curr_obj_id++;
		}
		//cout << "this is wm: " << ReteNet::getWMSize() << endl;
	}

	//end
}
*/

void ThreadOfTemporalProcessing::run()
{
	long long timeStart;
	bool once = false;
	int local_time = 0;
	int counter = 0;

	//std::this_thread::sleep_for(std::chrono::milliseconds(80));

	while (!is_stop) {
		//sleep to be safe


//		if(thisNet)
//		if (thisNet->getWMSize() > 0 || once) {
		timeStart = Utilities::getTime();

		thisNet->checkTemporalQueue(100, global_curr_time);

		if (local_time != global_curr_time) {
			int delta = Utilities::getTime() - timeStart;

			cout << "this is happening at temporal : " << global_curr_time << " time spent: " << delta << " current id: " << Utilities::id << " counter num: " << counter << endl;
			local_time = global_curr_time;
			counter = 0;
			timeStart = Utilities::getTime();
		}//cout << "this is data process" << endl;
		counter++;
		//cout << "temporal processing spent: " << Utilities::getTime() - timeStart << " when current time: " << global_curr_time << endl;
//		}
	}
}

void ThreadOfTemporalProcessing::runSingleNode(int nodeId)
{
	Node* n = thisNet->getNodeFromId(nodeId);
}

/*
void ThreadOfTemporalProcessing::runSeparately()
{
	long long localTime = 0;
	bool execute = false;
	vector<int> resId = {};

	while (!is_stop) {
		resId = {};
		if (global_curr_time != localTime) {
			localTime = global_curr_time;

			resId = thisNet->CheckTemporalExecutionID(100, global_curr_time);
			if (resId.size() > 0)
				execute = true;
			else false;
		}

		if (execute) {
			long long timeStart = Utilities::getTime();
			//init thread
			for (auto a : resId) {
				singleNodeThread.push_back(std::thread(thisNet->ExecuteSingleTemporalNode, 100, Utilities::time, thisNet->getNodeFromId(a)));
			}
			//join thread
			int itt = 0;
			for (auto& th : singleNodeThread) {
				th.join();
				thisNet->reInsertTemporalQueue(resId[itt]);
				itt++;
			}


			execute = false;

			cout << "temporal processing spent: " << Utilities::getTime() - timeStart << " when current time: " << global_curr_time << endl;
		}
	}
}
*/

std::thread ThreadOfTemporalProcessing::runThread(ReteNet* net, int time_now)
{
	thisNet = net;
	//this_time_now = time_now;
	return std::thread(&ThreadOfTemporalProcessing::run, this);
	//return std::thread(&ThreadOfTemporalProcessing::runSeparately, this);
}
