#include <iostream>
#include <vector>
#include <string>
#include <list>

//#define oneVessel_threeVessel
#define twoVessel_twoVessel
//#define threeVessel_oneVessel
//#define oneVessel_twovessel_oneAircraft
//#define oneVessel_oneSub_oneVessel_oneAircraft
//#define oneVessel_oneSub_oneVessel_oneAircraft_oneMissile
//#define allenemy_oneSub_oneVessel_oneAircraft
//#define oneVessel_onevessel_twoAircraft
//#define twoVessel_oneVessel_oneAircraft

#define sameTime_differentObjectNumber
//#define allVessel
//#define halfVessel_halfAircraft
#define onethirdVessel_oneThirdAircraft_oneThirdSubmarine


//#define INDEX_ON
#define INDEX_OFF

#include "ReteNet.h"

//using namespace std;

queue<EventPtr> generateSamepleLatLong(int number) {
	
	queue<EventPtr> ev;

	Event* e;
	//use system wise time
	int time_now = Utilities::time;

#ifdef sameTime_differentObjectNumber
	//object id
	Utilities::time++;
	time_now = Utilities::time;
	while (time_now < 600) {
		for (int i = 0; i < number; i++) {
			if (i % number == 0) {
				Utilities::time++;
				time_now = Utilities::time;
			}
			e = new Event(Utilities::id++, time_now);
			e->addAttr("iff", "false");
			e->addAttr("type", "recon"); //sclar
			e->addAttr("objid", i % number);

#ifdef allVessel
			e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
			float ele = (float)Utilities::randomFloat(0, 10);
			e->addAttr("elevation", ele); //scalar
#endif // allVessel

#ifdef halfVessel_halfAircraft
			if (i < number / 2) { // vessel
				e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
				float ele = (float)Utilities::randomFloat(0, 10);
				e->addAttr("elevation", ele); //scalar
			}
			else { //aircraft
				e->addAttr("speed", (float)Utilities::randomFloat(10, 40)); //scalar
				float ele = (float)Utilities::randomFloat(5, 30);
				e->addAttr("elevation", ele); //scalar
			}

#endif // halfVessel_halfAircraft

#ifdef onethirdVessel_oneThirdAircraft_oneThirdSubmarine

			if (i < (number *(1 / 3))) { // vessel
				e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
				float ele = (float)Utilities::randomFloat(0, 10);
				e->addAttr("elevation", ele); //scalar
			}
			else if (i > (number * (1 / 3)) && i < (number * (2 / 3))) { //submarine
				e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
				float ele = (float)Utilities::randomFloat(-10, 0);
				e->addAttr("elevation", ele); //scalar
			}
			else { //aircraft
				e->addAttr("speed", (float)Utilities::randomFloat(10, 40)); //scalar
				float ele = (float)Utilities::randomFloat(5, 30);
				e->addAttr("elevation", ele); //scalar
			}

#endif // onethirdVessel_oneThirdAircraft_oneThirdSubmarine

			e->addAttr("lon", (float)Utilities::randomFloat(120, 131)); //spatial
			e->addAttr("lat", (float)Utilities::randomFloat(30, 41)); //spatial
			ev.push(EventPtr(e));
		}
	}

#endif // sameTime_differentObjectNumber


#ifdef oneVessel_threeVessel
	for (int i = 0; i < number; i++) {
		e = new Event(Utilities::id++, time_now);
		if (i % 4 == 3) {
			Utilities::time++;
			time_now = Utilities::time;
		}

		if (i % 4 == 0) {
			e->addAttr("iff", "true"); // scalar
		}
		else {
			e->addAttr("iff", "false");
		}

		e->addAttr("type", "recon"); //sclar
		e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
		//e->addAttr("lon", (float)Utilities::randomFloat(120, 133)); //spatial_original_range
		//e->addAttr("lat", (float)Utilities::randomFloat(30, 42)); //spatial_original_range
		e->addAttr("lon", (float)Utilities::randomFloat(120, 131)); //spatial
		e->addAttr("lat", (float)Utilities::randomFloat(30, 41)); //spatial
		float ele = (float)Utilities::randomFloat(0, 10);
		e->addAttr("elevation", ele); //scalar

		e->addAttr("objid", i % 4);

		ev.push(EventPtr(e));
	}
#endif // oneVessel_threeVessel

#ifdef twoVessel_twoVessel
	for (int i = 0; i < number; i++) {
		e = new Event(Utilities::id++, time_now);
		if (i % 4 == 3) {
			Utilities::time++;
			time_now = Utilities::time;
		}

		if (i % 2 == 0) {
			e->addAttr("iff", "true"); // scalar
		}
		else {
			e->addAttr("iff", "false");
		}

		e->addAttr("type", "recon"); //sclar
		e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
		//e->addAttr("lon", (float)Utilities::randomFloat(120, 133)); //spatial_original_range
		//e->addAttr("lat", (float)Utilities::randomFloat(30, 42)); //spatial_original_range
		e->addAttr("lon", (float)Utilities::randomFloat(120, 131)); //spatial
		e->addAttr("lat", (float)Utilities::randomFloat(30, 41)); //spatial
		float ele = (float)Utilities::randomFloat(0, 10);
		e->addAttr("elevation", ele); //scalar

		e->addAttr("objid", i % 4);

		ev.push(EventPtr(e));
	}
#endif // twoVessel_twoVessel

#ifdef oneVessel_oneSub_oneVessel_oneAircraft
	//obj 0, 1 --> vessel
	//obj 2 --> submarine
	//obj 3 --> aircraft
	for (int i = 0; i < number; i++) {
		e = new Event(Utilities::id++, time_now);
		if (i % 4 == 3) {
			Utilities::time++;
			time_now = Utilities::time;
		}

		if (i % 4 == 0) {
			e->addAttr("iff", "true"); // scalar
		}
		else {
			e->addAttr("iff", "false");
		}

		e->addAttr("type", "recon"); //sclar
		if (i % 4 < 2) { //vessel
			e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
			float ele = (float)Utilities::randomFloat(0, 10);
			e->addAttr("elevation", ele); //scalar
		}
		else if (i % 4 == 2) { //submarine
			e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
			float ele = (float)Utilities::randomFloat(-10, 0);
			e->addAttr("elevation", ele); //scalar
		}
		else { //aircraft
			e->addAttr("speed", (float)Utilities::randomFloat(10, 40)); //scalar
			float ele = (float)Utilities::randomFloat(5, 30);
			e->addAttr("elevation", ele); //scalar
		}

		if (i % 4 == 0) { // our vessel doesnt move
			e->addAttr("lon", 125); //spatial
			e->addAttr("lat", 35); //spatial
		}
		else {
			e->addAttr("lon", (float)Utilities::randomFloat(120, 131)); //spatial
			e->addAttr("lat", (float)Utilities::randomFloat(30, 41)); //spatial
		}

		e->addAttr("objid", i % 4);

		ev.push(EventPtr(e));
	}
#endif // oneVessel_oneSub_oneVessel_oneAircraft

#ifdef allenemy_oneSub_oneVessel_oneAircraft
	//obj 0,1 --> vessel
	//obj 2 --> submarine
	//obj 3,4 --> aircraft

	for (int i = 0; i < number; i++) {
		e = new Event(Utilities::id++, time_now);
		if (i % 5 == 3) {
			Utilities::time++;
			time_now = Utilities::time;
		}

		e->addAttr("iff", "false");
		e->addAttr("type", "recon"); //sclar

		//if (i % 5 == 0) {
		//	e->addAttr("iff", "true"); // scalar
		//}
		//else {
		//	e->addAttr("iff", "false");
		//}

		if (i % 5 < 2) { //vessel
			e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
			float ele = (float)Utilities::randomFloat(0, 10);
			e->addAttr("elevation", ele); //scalar
		}
		else if (i % 5 == 2) {//submarine
			e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
			float ele = (float)Utilities::randomFloat(-10, 0);
			e->addAttr("elevation", ele); //scalar
		}
		else if (i % 5 >= 3) {//aircraft 
			e->addAttr("speed", (float)Utilities::randomFloat(10, 40)); //scalar
			float ele = (float)Utilities::randomFloat(5, 30);
			e->addAttr("elevation", ele); //scalar
		}

		//if (i % 5 == 0) { // our vessel doesnt move
		//	e->addAttr("lon", 125); //spatial
		//	e->addAttr("lat", 35); //spatial
		//}
		//else {
			e->addAttr("lon", (float)Utilities::randomFloat(120, 131)); //spatial
			e->addAttr("lat", (float)Utilities::randomFloat(30, 41)); //spatial
		//}

		e->addAttr("objid", i % 5);

		ev.push(EventPtr(e));
	}

#endif // allenemy_oneSub_oneVessel_oneAircraft

#ifdef oneVessel_oneSub_oneVessel_oneAircraft_oneMissile
	//obj 0, 1 --> vessel
	//obj 2 --> submarine
	//obj 3 --> aircraft
	//obj 4 --> aircraft
	for (int i = 0; i < number; i++) {
		e = new Event(Utilities::id++, time_now);
		if (i % 5 == 3) {
			Utilities::time++;
			time_now = Utilities::time;
		}

		if (i % 5 == 0) {
			e->addAttr("iff", "true"); // scalar
		}
		else {
			e->addAttr("iff", "false");
		}

		e->addAttr("type", "recon"); //sclar
		if (i % 5 < 2) { //vessel
			e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
			float ele = (float)Utilities::randomFloat(0, 10);
			e->addAttr("elevation", ele); //scalar
		}
		else if (i % 5 == 2) {//submarine
			e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
			float ele = (float)Utilities::randomFloat(-10, 0);
			e->addAttr("elevation", ele); //scalar
		}
		else if (i % 5 == 3) {//aircraft 
			e->addAttr("speed", (float)Utilities::randomFloat(10, 40)); //scalar
			float ele = (float)Utilities::randomFloat(5, 30);
			e->addAttr("elevation", ele); //scalar
		}
		else { //missile
			e->addAttr("speed", (float)Utilities::randomFloat(10, 40)); //scalar
			float ele = (float)Utilities::randomFloat(5, 30);
			e->addAttr("elevation", ele); //scalar
		}

		if (i % 5 == 0) { // our vessel doesnt move
			e->addAttr("lon", 125); //spatial
			e->addAttr("lat", 35); //spatial
		}
		else {
			e->addAttr("lon", (float)Utilities::randomFloat(120, 131)); //spatial
			e->addAttr("lat", (float)Utilities::randomFloat(30, 41)); //spatial
		}

		e->addAttr("objid", i % 5);

		ev.push(EventPtr(e));
	}
#endif // oneVessel_oneSub_oneVessel_oneAircraft_oneMissile

#ifdef oneVessel_twovessel_oneAircraft
	//obj 0, 1, 2 --> vessel
	//obj 3 --> aircraft
	for (int i = 0; i < number; i++) {
		e = new Event(Utilities::id++, time_now);
		if (i % 4 == 3) {
			Utilities::time++;
			time_now = Utilities::time;
		}

		if (i % 4 == 0) {
			e->addAttr("iff", "true"); // scalar
		}
		else {
			e->addAttr("iff", "false");
		}

		e->addAttr("type", "recon"); //sclar
		if (i % 4 <= 2) { //vessel
			e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
			float ele = (float)Utilities::randomFloat(0, 10);
			e->addAttr("elevation", ele); //scalar
		}
		else { //aircraft
			e->addAttr("speed", (float)Utilities::randomFloat(10, 40)); //scalar
			float ele = (float)Utilities::randomFloat(5, 30);
			e->addAttr("elevation", ele); //scalar
		}

		if (i % 4 == 0) { // our vessel doesnt move
			e->addAttr("lon", 125); //spatial
			e->addAttr("lat", 35); //spatial
		}
		else {
			e->addAttr("lon", (float)Utilities::randomFloat(120, 131)); //spatial
			e->addAttr("lat", (float)Utilities::randomFloat(30, 41)); //spatial
		}		

		e->addAttr("objid", i % 4);

		ev.push(EventPtr(e));
	}
#endif // oneVessel_twovessel_oneAircraft	

#ifdef oneVessel_onevessel_twoAircraft
	//obj 0, 1 --> vessel
	//obj 2, 3 --> aircraft
	for (int i = 0; i < number; i++) {
		e = new Event(Utilities::id++, time_now);
		if (i % 4 == 3) {
			Utilities::time++;
			time_now = Utilities::time;
		}

		if (i % 4 == 0) {
			e->addAttr("iff", "true"); // scalar
		}
		else {
			e->addAttr("iff", "false");
		}

		e->addAttr("type", "recon"); //sclar
		if (i % 4 <= 1) { //vessel
			e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
			float ele = (float)Utilities::randomFloat(0, 10);
			e->addAttr("elevation", ele); //scalar
		}
		else { //aircraft
			e->addAttr("speed", (float)Utilities::randomFloat(10, 40)); //scalar
			float ele = (float)Utilities::randomFloat(5, 30);
			e->addAttr("elevation", ele); //scalar
		}
		e->addAttr("lon", (float)Utilities::randomFloat(120, 131)); //spatial
		e->addAttr("lat", (float)Utilities::randomFloat(30, 41)); //spatial


		e->addAttr("objid", i % 4);

		ev.push(EventPtr(e));
	}
#endif // oneVessel_onevessel_twoAircraft


#ifdef twoVessel_oneVessel_oneAircraft
	//obj 0, 1, 2 --> vessel
	//obj 3 --> aircraft
	for (int i = 0; i < number; i++) {
		e = new Event(Utilities::id++, time_now);
		if (i % 4 == 3) {
			Utilities::time++;
			time_now = Utilities::time;
		}

		if (i % 2 == 0) { // 2 ally, 2 enemy
			e->addAttr("iff", "true"); // scalar
		}
		else {
			e->addAttr("iff", "false");
		}

		e->addAttr("type", "recon"); //sclar
		if (i % 4 == 3) { //vessel
			e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
			float ele = (float)Utilities::randomFloat(0, 10);
			e->addAttr("elevation", ele); //scalar
		}
		else { //aircraft
			e->addAttr("speed", (float)Utilities::randomFloat(10, 40)); //scalar
			float ele = (float)Utilities::randomFloat(5, 30);
			e->addAttr("elevation", ele); //scalar
		}
		e->addAttr("lon", (float)Utilities::randomFloat(120, 131)); //spatial
		e->addAttr("lat", (float)Utilities::randomFloat(30, 41)); //spatial


		e->addAttr("objid", i % 4);

		ev.push(EventPtr(e));
	}
#endif // twoVessel_oneVessel_oneAircraft

#ifdef threeVessel_oneVessel
	//obj 0, 1, 2, 3 --> vessel
	for (int i = 0; i < number; i++) {
		e = new Event(Utilities::id++, time_now);
		if (i % 4 == 3) {
			Utilities::time++;
			time_now = Utilities::time;
		}

		if (i % 4 <= 2) { // 2 ally, 2 enemy
			e->addAttr("iff", "true"); // scalar
		}
		else {
			e->addAttr("iff", "false");
		}

		e->addAttr("type", "recon"); //sclar
		e->addAttr("speed", (float)Utilities::randomFloat(3, 10)); //scalar
		float ele = (float)Utilities::randomFloat(0, 10);
		e->addAttr("elevation", ele); //scalar
		
		e->addAttr("lon", (float)Utilities::randomFloat(120, 131)); //spatial
		e->addAttr("lat", (float)Utilities::randomFloat(30, 41)); //spatial


		e->addAttr("objid", i % 4);

		ev.push(EventPtr(e));
	}
#endif // threeVessel_oneVessel


	int a;
	return ev;
}

queue<EventPtr> generateSample(int number) {
	queue<EventPtr> ev;

	Event* e;
	int time_now = 0;
	for (int i = 0; i < number; i++) {
		e = new Event(Utilities::id++, time_now);
		if (i % 2 == 1)
			time_now++;

		if (i % 2  == 0) {
			e->addAttr("iff", "ally");
		}
		else {
			e->addAttr("iff", "enemy");
		}

		e->addAttr("type", "recon");
		e->addAttr("speed", (float)Utilities::randomFloat(3, 20));
		e->addAttr("lon", (float)Utilities::randomFloat(125, 135)); // selisih 10
		e->addAttr("lat", (float)Utilities::randomFloat(35, 45)); // selisih 10
		//e->addAttr("dir", (float)Utilities::randomFloat(0, 360));
		float ele = (float)Utilities::randomFloat(0, 20);
		e->addAttr("elevation", ele);

		e->addAttr("objid", i % 4);

		ev.push(EventPtr(e));
	}

	//ReteNet::cloneToWm(ev);
	return ev;
}

int main() {

	int select;
	queue<EventPtr> tempWM;

	cout << "select what you want " << endl;
	cout << "0. exit " << endl;
	cout << "1. manually input the rule " << endl;
	cout << "2. generate pre defined rule" << endl;
	cout << "3. generate random event" << endl;
	cout << "4. run rete" << endl;
	cout << "5. run rete and show time consumption" << endl;
	cin >> select;
	while (select <= 20) {
		switch (select) {
			cin.ignore();
		case 0: {
			return 0;
		}
		case 1: {
			system("cls");
			string temp, then_detect;
			vector<string> master_str;
			cout << "there are 9 attributes in each event, those are:" << endl;
			cout << "1. event id -> already fixed from system" << endl;
			cout << "2. time --> generated new time (seconds) each 4 events" << endl;
			cout << "3. iff --> contain ('ally', 'enemy')" << endl;
			cout << "4. type --> represent object type, only contain ('recon')" << endl;
			cout << "5. lon --> represent longitude or x axis, random range (127, 135)" << endl;
			cout << "6. lat --> represent latitude or y axis, random range (35, 44)" << endl;
			cout << "7. elevation --> repesent object elevation, random range (0,20)" << endl;
			cout << "8. objid --> represent object id, distributed evenly from 0 until 3" << endl<<endl;

			cout << "distance is calculated by: object_latitude - (latitude/2), object_latitude + (latitude/2), object_longitude - (longitude/2), object_longitude + (longitude/2) ---" << endl << endl;

			cout << "start with IF and finish with THEN" << endl << endl;
			cout << "example:" << endl;
			cout << "IF speed > 10 & elevation > 10 & iff=ally" << endl;
			cout << "THEN allyaircraft" << endl << endl << "or" << endl << endl;
			cout << "IF distance(allyaircraft, enemyvessel) < 10 & allyaircraft.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN airthreatdetected" << endl << endl;
			cout << "your input:" << endl;
			 
			cin.ignore();
			getline(cin, temp);
			if (temp.substr(0, 2) != "IF") {
				break;
			}
			else {
				master_str.push_back(temp);

				while (then_detect != "THEN") {
					//cin.ignore();
					getline(cin, temp);
					then_detect = temp.substr(0, 4);
					master_str.push_back(temp);
				}
			}

			vector<vector<pair<string, string>>> colMade;
			colMade = ReteNet::parseConditionOriginal(master_str);

			ReteNet::growTheNodes(colMade);

			system("pause");
			break;
		}
		case 2: {
			system("cls");
			cout << "Rules for checking distance" << endl;

			//BASIC IF ELSE
			vector<string> made;
			made.push_back("IF speed>3 & elevation<10 & iff=true");
			made.push_back("THEN allyvessel");
			vector<vector<pair<string, string>>> colMade;
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<10 & iff=ally" << endl;
			cout << "THEN allyvessel" << endl;

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>3 & elevation<10 & iff=false");
			made.push_back("THEN enemyvessel");
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<10 & iff=enemy" << endl;
			cout << "THEN enemyvessel" << endl;

			ReteNet::growTheNodes(colMade);

			//SPATIAL IF ELSE
			made = {};
			made.push_back("IF distance(allyvessel,enemyvessel)<10 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat, count.enemyvessel.objid");
			//made.push_back("THEN navalthreat");

			cout << endl << "IF distance(allyvessel,enemyvessel)<10 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalthreat, count.enemyvessel.objid" << endl;
			//cout << "THEN enemyvessel" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			//Now Exist
			made = {};
			made.push_back("IF exist(navalthreat)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalresponse");

			cout << endl << "IF exist(navalthreat)" << endl;
			cout << "WINDOW range=10, trigger=10" << endl;
			cout << "THEN navalresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			system("pause");
			break;
		}case 3: {
			system("cls");
			cout << endl;
			cout << "Generate WM EVENTS" << endl << endl;
			cout << "Enter the number of events : " << endl;
			int n;
			cin >> n;
			tempWM = generateSamepleLatLong(n);

			break;
		}case 4: {
			system("cls");
			ReteNet::buildNetNode();

			cout << endl << "Execute RETE" << endl << endl;

			while (tempWM.size() > 0) {

				/*
				//this is real time mode --------------------------------------------------
				EventPtr e = tempWM.front();
				queue<EventPtr> singlePoint = {};
				singlePoint.push(e);

				ReteNet::cloneToWm(singlePoint);
				//-------------------------------------------------------------------------
				*/

				//this is same-time mode --------------------------------------------------
				int currTime = tempWM.front()->getInt("time");

				queue<EventPtr> oneTimeEvent = {};
				while (tempWM.front()->getInt("time") == currTime) {
					oneTimeEvent.push(tempWM.front());
					tempWM.pop();

					if (tempWM.size() <= 0)
						break;
				}

				ReteNet::cloneToWm(oneTimeEvent);
				//-------------------------------------------------------------------------

				//do pre-processing
				ReteNet::ExecuteRete(100);

				//now the spatio temporal thing
				//ReteNet::SpatioTemporalExecution(100, oneTimeEvent.front()->getInt("time"));
				ReteNet::NewSpatioTemporalExecution(100, oneTimeEvent.front()->getInt("time"));

				//tempWM.pop();

			}

			system("pause");
			break;
		}case 5: {
			system("cls");
			ReteNet::buildNetNode();

			int sizewm = tempWM.size();
			cout << endl << "calculate time spent" << endl << endl;
			cout << "Number of nodes: " << ReteNet::GetNumberOfNodes() << endl;
			cout << "Number of data: " << sizewm << endl;

			long long startTime = Utilities::getTime();

			while (tempWM.size() > 0) {

				/*
				//this is real time mode --------------------------------------------------
				EventPtr e = tempWM.front();
				queue<EventPtr> singlePoint = {};
				singlePoint.push(e);

				ReteNet::cloneToWm(singlePoint);
				//-------------------------------------------------------------------------
				*/

				//this is same-time mode --------------------------------------------------
				int currTime = tempWM.front()->getInt("time");

				queue<EventPtr> oneTimeEvent = {};
				while (tempWM.front()->getInt("time") == currTime) {
					oneTimeEvent.push(tempWM.front());
					tempWM.pop();

					if (tempWM.size() <= 0)
						break;
				}

				ReteNet::cloneToWm(oneTimeEvent);
				//-------------------------------------------------------------------------

				//small time viewer
				long long temp_startTime = Utilities::getTime();

				//do pre-processing
				ReteNet::ExecuteRete(100);

				long long filteringTime = temp_startTime - Utilities::getTime();
				long long filterTimeStart = Utilities::getTime();

				//now the spatio temporal thing
				//ReteNet::SpatioTemporalExecution(100, oneTimeEvent.front()->getInt("time"));
				ReteNet::NewSpatioTemporalExecution(100, oneTimeEvent.front()->getInt("time"));
				
				long long spatiotemporalTime = filterTimeStart - Utilities::getTime();

				if (currTime % 100 == 0) {
					cout << "filtering time : " << filteringTime << " spatiotemporal time : " << spatiotemporalTime << endl;
				}

			}

			long long timeSpent = startTime - Utilities::getTime();
			cout << "Number of nodes: " << ReteNet::GetNumberOfNodes() << endl;
			cout << "Number of data: " << sizewm << endl;
#ifdef INDEX_OFF
			cout << "No-indexing time spent: " << timeSpent << endl << endl;
#endif // INDEX_OFF
#ifdef INDEX_ON 
			cout << "with indexing time spent: " << timeSpent << endl << endl;
#endif // INDEX_ON 

			system("pause");
			break;
		}case 6: {
			system("cls");
			cout << "Rules for checking multiple vessel distances" << endl;

			//BASIC IF ELSE
			vector<string> made;
			made.push_back("IF speed>3 & elevation<10 & iff=ally");
			made.push_back("THEN allyvessel");
			vector<vector<pair<string, string>>> colMade;
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<10 & iff=ally" << endl;
			cout << "THEN allyvessel" << endl;

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>3 & elevation<10 & iff=enemy");
			made.push_back("THEN enemyvessel");
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<10 & iff=enemy" << endl;
			cout << "THEN enemyvessel" << endl;

			ReteNet::growTheNodes(colMade);

			//SPATIAL IF ELSE
			made = {};
			made.push_back("IF distance(allyvessel,enemyvessel)<12 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat_far, count.enemyvessel.objid");

			cout << endl << "IF distance(allyvessel,enemyvessel)<12 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalthreat_far, count.enemyvessel.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF distance(allyvessel,enemyvessel)<10 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat_med, count.enemyvessel.objid");

			cout << endl << "IF distance(allyvessel,enemyvessel)<10 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalthreat_med, count.enemyvessel.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF distance(allyvessel,enemyvessel)<7 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat_close, count.enemyvessel.objid");

			cout << endl << "IF distance(allyvessel,enemyvessel)<8 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalthreat_close, count.enemyvessel.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			//Now Exist
			made = {};
			made.push_back("IF exit(navalthreat_far)");
			made.push_back("WINDOW range=12, trigger=12");
			made.push_back("THEN navalresponse_far");

			cout << endl << "IF exist(navalthreat_far)" << endl;
			cout << "WINDOW range=12, trigger=12" << endl;
			cout << "THEN navalresponse_far" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF enter(navalthreat_med)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalresponse_med");

			cout << endl << "IF exist(navalthreat_med)" << endl;
			cout << "WINDOW range=10, trigger=10" << endl;
			cout << "THEN navalresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF stay(navalthreat_close)");
			made.push_back("WINDOW range=8, trigger=8");
			made.push_back("THEN navalresponse_close");

			cout << endl << "IF exist(navalthreat_close)" << endl;
			cout << "WINDOW range=8, trigger=18" << endl;
			cout << "THEN navalresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			system("pause");
			break;
		} case 7:{ // multiple distance node but all exist function
			system("cls");
			cout << "Rules for checking multiple vessel distances" << endl;

			//BASIC IF ELSE
			vector<string> made;
			made.push_back("IF speed>3 & elevation<10 & iff=ally");
			made.push_back("THEN allyvessel");
			vector<vector<pair<string, string>>> colMade;
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<10 & iff=ally" << endl;
			cout << "THEN allyvessel" << endl;

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>3 & elevation<10 & iff=enemy");
			made.push_back("THEN enemyvessel");
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<10 & iff=enemy" << endl;
			cout << "THEN enemyvessel" << endl;

			ReteNet::growTheNodes(colMade);

			//SPATIAL IF ELSE
			made = {};
			made.push_back("IF distance(allyvessel,enemyvessel)<12 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat_far, count.enemyvessel.objid");

			cout << endl << "IF distance(allyvessel,enemyvessel)<12 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalthreat_far, count.enemyvessel.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF distance(allyvessel,enemyvessel)<10 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat_med, count.enemyvessel.objid");

			cout << endl << "IF distance(allyvessel,enemyvessel)<10 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalthreat_med, count.enemyvessel.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF distance(allyvessel,enemyvessel)<7 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat_close, count.enemyvessel.objid");

			cout << endl << "IF distance(allyvessel,enemyvessel)<8 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalthreat_close, count.enemyvessel.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			//Now Exist
			made = {};
			made.push_back("IF exist(navalthreat_far)");
			made.push_back("WINDOW range=12, trigger=12");
			made.push_back("THEN navalresponse_far");

			cout << endl << "IF exist(navalthreat_far)" << endl;
			cout << "WINDOW range=12, trigger=12" << endl;
			cout << "THEN navalresponse_far" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(navalthreat_med)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalresponse_med");

			cout << endl << "IF exist(navalthreat_med)" << endl;
			cout << "WINDOW range=10, trigger=10" << endl;
			cout << "THEN navalresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(navalthreat_close)");
			made.push_back("WINDOW range=8, trigger=8");
			made.push_back("THEN navalresponse_close");

			cout << endl << "IF exist(navalthreat_close)" << endl;
			cout << "WINDOW range=8, trigger=18" << endl;
			cout << "THEN navalresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			system("pause");
			break;
		}case 8: { // Aircraft entering the battlefield
			system("cls");
			cout << "Rules for checking Aircraft And Vessels" << endl;

			//BASIC IF ELSE
			vector<string> made;
			made.push_back("IF speed>3 & elevation<10 & iff=true");
			made.push_back("THEN allyvessel");
			vector<vector<pair<string, string>>> colMade;
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<10 & iff=true" << endl;
			cout << "THEN allyvessel" << endl;

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>3 & elevation<10 & iff=false");
			made.push_back("THEN enemyvessel");
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<10 & iff=false" << endl;
			cout << "THEN enemyvessel" << endl;

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>10 & elevation>5 & iff=false");
			made.push_back("THEN enemyaircraft");
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>10 & elevation>5 & iff=false" << endl;
			cout << "HEN enemyaircraft" << endl;

			ReteNet::growTheNodes(colMade);

			//SPATIAL IF ELSE
			made = {};
			made.push_back("IF distance(allyvessel,enemyvessel)<10 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat, count.enemyvessel.objid");

			cout << endl << "IF distance(allyvessel,enemyvessel)<10 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalthreat, count.enemyvessel.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF distance(allyvessel,enemyaircraft)<10 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalairthreat, count.enemyaircraft.objid");

			cout << endl << "IF distance(allyvessel,enemyaircraft)<10 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalairthreat, count.enemyaircraft.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			//Now Exist
			made = {};
			made.push_back("IF exist(navalthreat)");
			made.push_back("WINDOW range=15, trigger=15");
			made.push_back("THEN navalresponse");

			cout << endl << "IF exist(navalthreat)" << endl;
			cout << "WINDOW range=15, trigger=15" << endl;
			cout << "THEN navalresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(navalairthreat)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalairresponse");

			cout << endl << "IF exist(navalairthreat)" << endl;
			cout << "WINDOW range=10, trigger=10" << endl;
			cout << "THEN navalairresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			system("pause");
			break;
		} case 9: { //average time spent they said
			system("cls");
			ReteNet::buildNetNode();

			cout << endl << "calculate average time spent" << endl << endl;
			cout << "Number of nodes: " << ReteNet::GetNumberOfNodes() << endl;
			cout << "Number of data: " << tempWM.size() << endl;

			long long startTime;
			int time_itt = 0;
			long long avg_time = 0;

			while (tempWM.size() > 0) {

				//this is same-time mode --------------------------------------------------
				int currTime = tempWM.front()->getInt("time");

				queue<EventPtr> oneTimeEvent = {};
				while (tempWM.front()->getInt("time") == currTime) {
					oneTimeEvent.push(tempWM.front());
					tempWM.pop();

					if (tempWM.size() <= 0)
						break;
				}

				ReteNet::cloneToWm(oneTimeEvent);
				//-------------------------------------------------------------------------

				time_itt++;
				startTime = Utilities::getTime();

				//do pre-processing
				ReteNet::ExecuteRete(100);

				//now the spatio temporal thing
				ReteNet::SpatioTemporalExecution(100, oneTimeEvent.front()->getInt("time"));

				long long timeSpent = Utilities::getTime() - startTime;

				avg_time += timeSpent;
				//avg_time = avg_time + ((timeSpent - avg_time) / time_itt);
				//cout << "average time spent: " << avg_time << endl << endl;
			}

			cout << "average time spent: " << avg_time / (long long)time_itt << endl << endl;
			system("pause");
			break; 
		}case 10: { //submarine, aircraft, and vessel
			system("cls");
			cout << "Rules for checking Aircraft, Vessels, submarines" << endl;

			//BASIC IF ELSE
			vector<string> made;
			made.push_back("IF speed>3 & elevation<10 & iff=true");
			made.push_back("THEN allyvessel");
			vector<vector<pair<string, string>>> colMade;
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<10 & iff=true" << endl;
			cout << "THEN allyvessel" << endl;

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>3 & elevation<10 & iff=false");
			made.push_back("THEN enemyvessel");
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<10 & iff=false" << endl;
			cout << "THEN enemyvessel" << endl;

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>3 & elevation<0 & iff=false");
			made.push_back("THEN enemysubmarine");
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<0 & iff=false" << endl;
			cout << "THEN enemysubmarine" << endl;

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>10 & elevation>5 & iff=false");
			made.push_back("THEN enemyaircraft");
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>10 & elevation>5 & iff=false" << endl;
			cout << "HEN enemyaircraft" << endl;

			ReteNet::growTheNodes(colMade);

			//SPATIAL IF ELSE
			made = {};
			made.push_back("IF distance(allyvessel,enemyvessel)<10 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat, count.enemyvessel.objid");

			cout << endl << "IF distance(allyvessel,enemyvessel)<10 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalthreat, count.enemyvessel.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF distance(allyvessel,enemyaircraft)<10 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalairthreat, count.enemyaircraft.objid");

			cout << endl << "IF distance(allyvessel,enemyaircraft)<10 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalairthreat, count.enemyaircraft.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF distance(allyvessel,enemysubmarine)<10 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalsubmarinethreat, count.enemysubmarine.objid");

			cout << endl << "IF distance(allyvessel,enemysubmarine)<10 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalsubmarinethreat, count.enemysubmarine.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			//Now Exist
			made = {};
			made.push_back("IF exist(navalthreat)");
			made.push_back("WINDOW range=15, trigger=15");
			made.push_back("THEN navalresponse");

			cout << endl << "IF exist(navalthreat)" << endl;
			cout << "WINDOW range=15, trigger=15" << endl;
			cout << "THEN navalresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(navalairthreat)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalairresponse");

			cout << endl << "IF exist(navalairthreat)" << endl;
			cout << "WINDOW range=10, trigger=10" << endl;
			cout << "THEN navalairresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(navalsubmarinethreat)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalsubresponse");

			cout << endl << "IF exist(navalairthreat)" << endl;
			cout << "WINDOW range=10, trigger=10" << endl;
			cout << "THEN navalsubresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			system("pause");
			break;
		}
		case 11: { //submarine, aircraft, vessel, and missiles
			system("cls");
			cout << "Rules for checking Aircraft, Vessels, submarine, missiles" << endl;

			//BASIC IF ELSE
			vector<string> made;
			made.push_back("IF speed>3 & elevation<10 & iff=true");
			made.push_back("THEN allyvessel");
			vector<vector<pair<string, string>>> colMade;
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<10 & iff=true" << endl;
			cout << "THEN allyvessel" << endl;

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>3 & elevation<10 & iff=false");
			made.push_back("THEN enemyvessel");
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<10 & iff=false" << endl;
			cout << "THEN enemyvessel" << endl;

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>3 & elevation<0 & iff=false");
			made.push_back("THEN enemysubmarine");
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>3 & elevation<0 & iff=false" << endl;
			cout << "THEN enemysubmarine" << endl;

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>10 & elevation>5 & iff=false");
			made.push_back("THEN enemyaircraft");
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>10 & elevation>5 & iff=false" << endl;
			cout << "HEN enemyaircraft" << endl;

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>15 & elevation>5 & iff=false");
			made.push_back("THEN enemymissile");
			colMade = ReteNet::parseConditionOriginal(made);

			cout << endl << "IF speed>10 & elevation>5 & iff=false" << endl;
			cout << "HEN enemymissile" << endl;

			ReteNet::growTheNodes(colMade);

			//SPATIAL IF ELSE
			made = {};
			made.push_back("IF distance(allyvessel,enemyvessel)<10 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat, count.enemyvessel.objid");

			cout << endl << "IF distance(allyvessel,enemyvessel)<10 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalthreat, count.enemyvessel.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF distance(allyvessel,enemyaircraft)<10 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalairthreat, count.enemyaircraft.objid");

			cout << endl << "IF distance(allyvessel,enemyaircraft)<10 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalairthreat, count.enemyaircraft.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF distance(allyvessel,enemysubmarine)<10 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalsubmarinethreat, count.enemysubmarine.objid");

			cout << endl << "IF distance(allyvessel,enemysubmarine)<10 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalsubmarinethreat, count.enemysubmarine.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF distance(allyvessel,enemymissile)<10 & allyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalmissilethreat, count.enemymissile.objid");

			cout << endl << "IF distance(allyvessel,enemymissile)<10 & allyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalmissilethreat, count.enemymissile.objid" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			//Now Exist
			made = {};
			made.push_back("IF exist(navalthreat)");
			made.push_back("WINDOW range=15, trigger=15");
			made.push_back("THEN navalresponse");

			cout << endl << "IF exist(navalthreat)" << endl;
			cout << "WINDOW range=15, trigger=15" << endl;
			cout << "THEN navalresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(navalairthreat)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalairresponse");

			cout << endl << "IF exist(navalairthreat)" << endl;
			cout << "WINDOW range=10, trigger=10" << endl;
			cout << "THEN navalairresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(navalmissilethreat)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalmissileresponse");

			cout << endl << "IF exist(navalairthreat)" << endl;
			cout << "WINDOW range=10, trigger=10" << endl;
			cout << "THEN navalairresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(navalsubmarinethreat)");
			made.push_back("WINDOW range=15, trigger=15");
			made.push_back("THEN navalsubresponse");

			cout << endl << "IF exist(navalairthreat)" << endl;
			cout << "WINDOW range=10, trigger=10" << endl;
			cout << "THEN navalairresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			system("pause");
			break;
		}
		case 12: { //simple static cq testing
			system("cls");
			cout << "Rules for checking distance" << endl;

			//BASIC IF ELSE
			vector<string> made;
			made.push_back("IF speed>3 & elevation<10 & iff=true");
			made.push_back("THEN allyvessel");
			vector<vector<pair<string, string>>> colMade;
			colMade = ReteNet::parseConditionOriginal(made);

			/*cout << endl << "IF speed>3 & elevation<10 & iff=true" << endl;
			cout << "THEN allyvessel" << endl;*/

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>3 & elevation<10 & iff=false");
			made.push_back("THEN enemyvessel");
			colMade = ReteNet::parseConditionOriginal(made);

			/*cout << endl << "IF speed>3 & elevation<10 & iff=false" << endl;
			cout << "THEN enemyvessel" << endl;*/

			ReteNet::growTheNodes(colMade);

			//SPATIAL IF ELSE --------------------------------------------------------------------------------
			made = {};
			made.push_back("IF plot_dist(rect(121,31,130,41), enemyvessel) & enemyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat");
			//made.push_back("THEN navalthreat");

			/*cout << endl << "IF plot_dist(rect(121,31,130,41), enemyvessel) & enemyvessel.type=recon" << endl;
			cout << "WINDOW range=5, trigger=5" << endl;
			cout << "THEN navalthreat, count.enemyvessel.objid" << endl;*/
			//cout << "THEN enemyvessel" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			//Now Exist
			made = {};
			made.push_back("IF exist(navalthreat)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalresponse");

			//cout << endl << "IF exist(navalthreat)" << endl;
			//cout << "WINDOW range=10, trigger=10" << endl;
			//cout << "THEN navalresponse" << endl;

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			system("pause");
			break;
		}
		case 13: { //just enemyvessel but multiple cq
			system("cls");
			cout << "Rules for checking multiple distance" << endl;

			//BASIC IF ELSE
			vector<string> made;
			vector<vector<pair<string, string>>> colMade;
			
			made.push_back("IF speed>3 & elevation<10 & iff=false");
			made.push_back("THEN enemyvessel");
			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			//SPATIAL IF ELSE --------------------------------------------------------------------------------
			made = {};
			made.push_back("IF plot_dist(rect(121,31,130,41), enemyvessel) & enemyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN biggestcq");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF plot_dist(rect(122,32,130,41), enemyvessel) & enemyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN mediumcq");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF plot_dist(rect(125,35,130,41), enemyvessel) & enemyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN mediumcq1");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF plot_dist(rect(122,32,125,35), enemyvessel) & enemyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN mediumcq2");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF plot_dist(rect(126,34,128,41), enemyvessel) & enemyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN mediumcq3");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			//Now Exist
			made = {};
			made.push_back("IF exist(biggestcq)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalresponse");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(mediumcq)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalresponse1");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(mediumcq1)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalresponse2");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(mediumcq2)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalresponse3");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(mediumcq3)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalresponse4");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			system("pause");
			break;
		}
		case 14: { //checking onesub, onevessel, oneaircraft, with multiple cq plot
			system("cls");
			cout << "Rules for checking Aircraft, Vessels, submarines" << endl;

			//BASIC IF ELSE
			vector<string> made;
			vector<vector<pair<string, string>>> colMade;

			made = {};
			made.push_back("IF speed>3 & elevation<10 & iff=false");
			made.push_back("THEN enemyvessel");
			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>3 & elevation<0 & iff=false");
			made.push_back("THEN enemysubmarine");
			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>10 & elevation>5 & iff=false");
			made.push_back("THEN enemyaircraft");
			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			//SPATIAL IF ELSE --------------------------------------------------------------------------------
			made = {};
			made.push_back("IF plot_dist(rect(121,31,130,41), enemyvessel) & enemyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF plot_dist(rect(123,33,127,35), enemyvessel) & enemyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreatsevere");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF plot_dist(rect(121,31,130,41), enemyaircraft) & enemyaircraft.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN airthreat");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF plot_dist(rect(123,33,127,35), enemyaircraft) & enemyaircraft.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN airthreatsevere");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF plot_dist(rect(121,31,130,41), enemysubmarine) & enemysubmarine.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN underwaterthreat");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF plot_dist(rect(123,33,127,35), enemysubmarine) & enemysubmarine.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN underwaterthreatsevere");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			//Now Exist
			made = {};
			made.push_back("IF exist(navalthreat)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalresponse");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(navalthreatsevere)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN urgentnavalresponse");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(airthreat)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN airresponse");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(airthreatsevere)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN urgentairresponse");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(underwaterthreat)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN underwaterresponse");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(underwaterthreatsevere)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN urgentunderwaterresponse");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			system("pause");
			break;
		}
		case 15: { // just like case 12 and 13 but different number of rules
			system("cls");
			cout << "Rules for checking distance" << endl;

			//BASIC IF ELSE
			vector<string> made;
			made.push_back("IF speed>3 & elevation<10 & iff=true");
			made.push_back("THEN allyvessel");
			vector<vector<pair<string, string>>> colMade;
			colMade = ReteNet::parseConditionOriginal(made);

			/*cout << endl << "IF speed>3 & elevation<10 & iff=true" << endl;
			cout << "THEN allyvessel" << endl;*/

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF speed>3 & elevation<10 & iff=false");
			made.push_back("THEN enemyvessel");
			colMade = ReteNet::parseConditionOriginal(made);

			/*cout << endl << "IF speed>3 & elevation<10 & iff=false" << endl;
			cout << "THEN enemyvessel" << endl;*/

			ReteNet::growTheNodes(colMade);

			//SPATIAL IF ELSE --------------------------------------------------------------------------------
			made = {};
			made.push_back("IF plot_dist(rect(121,31,130,41), enemyvessel) & enemyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreat");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF plot_dist(rect(123,33,127,35), enemyvessel) & enemyvessel.type=recon");
			made.push_back("WINDOW range=5, trigger=5");
			made.push_back("THEN navalthreatsevere");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			//Now Exist
			made = {};
			made.push_back("IF exist(navalthreat)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalresponse");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			made = {};
			made.push_back("IF exist(navalthreatsevere)");
			made.push_back("WINDOW range=10, trigger=10");
			made.push_back("THEN navalresponsesevere");

			colMade = ReteNet::parseConditionOriginal(made);

			ReteNet::growTheNodes(colMade);

			system("pause");
			break;
		}
		}

		system("cls");
		cout << "select what you want " << endl;
		cout << "0. exit " << endl;
		cout << "1. manually input the rule " << endl;
		cout << "2. generate pre defined rule" << endl;
		cout << "3. generate random event" << endl;
		cout << "4. run rete" << endl;
		cout << "5. run rete and show time consumption" << endl;
		cin >> select;

	}

	return 0;
}