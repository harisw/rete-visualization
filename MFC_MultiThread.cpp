#include "MFC_MultiThread.h"

#define FUTURE_TOLLERANCE_IN_SEC 10
//#define CALCULATE_BASED_ON_DATA_GENERATION
//#define CALCULATE_BASED_ON_DATA_PROCESSING

#define USE_SECOND
//#define USE_MILLISEC

static int global_curr_itt;

void MFCThreadOfDataGenerator::run()
{
	//time now is useless
	int time_now = 0, curr_obj_id = 0;
	long long startTime = Utilities::getTime();

	//Utilities::start_cycle_time = Utilities::getTime();// +1000;
	//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
	while (!is_stop) {
		if (is_paused) {
			continue;
		}
		//std::lock_guard<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
		//long long tempStart = Utilities::getTime();

		EventPtr e = generateSingleEvent(curr_obj_id, this_obj_num, time_now);

		if (e != nullptr) {
			std::unique_lock<std::recursive_mutex> lg(ReteNet::mutexOfReteProcess);//mutex lock
			//ReteNet::pushToWM(e);
			theNet->pushToWM(e);
			inpQueue.push(e);
		}

		if (curr_obj_id > this_obj_num) {
			curr_obj_id = 0;

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
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			//Utilities::start_cycle_time = Utilities::getTime();// +1000;
			startTime = Utilities::getTime();
		}
		else {
			curr_obj_id++;
		}

		//cout << "this is wm: " << ReteNet::getWMSize() << endl;
	}
}

EventPtr MFCThreadOfDataGenerator::generateSingleEvent(int obj_id, int obj_num, int curr_time)
{
	//Event* e = new Event(Utilities::id++, curr_time);
#ifdef USE_SECOND
	Event* e = new Event(Utilities::id++, Utilities::getTimeSec() + FUTURE_TOLLERANCE_IN_SEC);
#endif // USE_SECOND
#ifdef USE_MILLISEC
	Event* e = new Event(Utilities::id++, Utilities::getTime());
#endif // USE_MILLISEC

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
			int direction = rand() % 4 + 1;
			if (direction == 1) {	//LEFT
				lon = objectLocations[obj_id].first - (float)Utilities::randomFloat(1, 2);
				lat = objectLocations[obj_id].second;
			} else if (direction == 2) { //UP
				lon = objectLocations[obj_id].first;
				lat = objectLocations[obj_id].second + (float)Utilities::randomFloat(1, 2);
			} else if (direction == 3) {	//RIGHT
				lon = objectLocations[obj_id].first + (float)Utilities::randomFloat(1, 2);
				lat = objectLocations[obj_id].second;
			} else {	//DOWN
				lon = objectLocations[obj_id].first;
				lat = objectLocations[obj_id].second - (float)Utilities::randomFloat(1, 2);
			}
		}
		else {
			lon = (float)Utilities::randomFloat(110, 141);
			lat = (float)Utilities::randomFloat(20, 51);
		}
		objectLocations[obj_id] = make_pair(lon, lat);
		e->addAttr("lon", lon); //spatial
		e->addAttr("lat", lat); //spatial
		//e->addAttr("lon", (float)Utilities::randomFloat(110, 141)); //spatial
		//e->addAttr("lat", (float)Utilities::randomFloat(20, 51)); //spatial
	}

	e->addAttr("electromagnet", "false");

	e->addAttr("objid", obj_id);

	return EventPtr(e);
}

std::thread MFCThreadOfDataGenerator::runThread(int obj_num, ReteNet* net, queue<EventPtr> &inpQ)
{
	theNet = net;
	this_obj_num = obj_num;
	inpQueue = inpQ;
	return std::thread(&MFCThreadOfDataGenerator::run, this);
}

void MFC_MultiThread::start_sim(int obj_num, queue<EventPtr> inpQueue)
{
	is_stop = false;
	is_paused = false;
	//make a dummy they said
	ReteNet theRete;
	theRete.buildNetNode();

	//Data Generator
	MFCThreadOfDataGenerator* d = new MFCThreadOfDataGenerator();
	d->runThread(obj_num, &theRete, inpQueue).detach();

	//Data process
	MFCThreadOfDataProcessing* p = new MFCThreadOfDataProcessing();
	p->runThread(&theRete, obj_num).detach();

	//Temporal Processing
	MFCTHreadOfTemporalProcessing* tp = new MFCTHreadOfTemporalProcessing();
	tp->runThread(&theRete, global_curr_itt).detach();
}

void MFC_MultiThread::stop_sim()
{
	is_stop = true;
}

void MFC_MultiThread::pause_sim()
{
	is_paused = true;
}

void MFC_MultiThread::continue_sim()
{
	is_paused = false;
}

void MFCThreadOfDataProcessing::run()
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
}

std::thread MFCThreadOfDataProcessing::runThread(ReteNet* net, int obj_num)
{
	numOfObject = obj_num;
	theNet = net;
	return std::thread(&MFCThreadOfDataProcessing::run, this);
}

void MFCTHreadOfTemporalProcessing::run()
{
	long long timeStart = Utilities::getTime();
	bool once = false;
	int local_time = 0;
	int counter = 0;

	//std::this_thread::sleep_for(std::chrono::milliseconds(80));

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

std::thread MFCTHreadOfTemporalProcessing::runThread(ReteNet* net, int obj_num)
{
	thisNet = net;
	//this_time_now = time_now;
	return std::thread(&MFCTHreadOfTemporalProcessing::run, this);
}
