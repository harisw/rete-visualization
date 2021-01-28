#include "GenerateData.h"

vector<POINT_GEN> GenerateData::converted_vector;

void GenerateData::Generate(vector<pair<int,int>> input_vector, int obj_num, int time_len)
{
	converted_vector = {};

	//change first
	for (auto vec : input_vector) {
		converted_vector.push_back({ vec.first, vec.second });
	}
	//generate object and print the output
	CQObjectSet* pRet = generateObjects(converted_vector, obj_num, time_len, CURVETYPE::LINE);

	//insert the produced output :v
	//errr, after some consideration, better to do this in main function

	//no no that doesn't work, do here instead
	//convert again, sorry developer :"
	vector<pair<float, float>> point_list;
	for (int i = 0; i < obj_num; i++) {

		point_list = pRet->getSingleObjectTrajectory(i);

		MFC_FixedMultiThread::setupData(i, point_list);

		point_list = {};
	}

	if (pRet)   delete pRet;
}

int GenerateData::ReadFromFile(string filename)
{
	fstream newfile;

	int obj_id_counter = 0;
	newfile.open(filename, ios::in); //open a file to perform read operation using file object
	if (newfile.is_open()) {   //checking whether the file is open
		string tp;
		vector<pair<float, float>> collectedLatLong = {};
		while (getline(newfile, tp)) { //read data from file object and put it into string.

			std::size_t found = tp.find(",");

			if (found > 0 && found < tp.length()) {
				float lat = atof(tp.substr(0, found).c_str());
				float lon = atof(tp.substr(found + 1, tp.length()).c_str());

				collectedLatLong.push_back({ lat, lon });
			}
			else {
				if (collectedLatLong.size() == 0)
					continue;

				MFC_FixedMultiThread::setupData(obj_id_counter, collectedLatLong);
				//MFC_MultiThread::setupEventLatLong(obj_id_counter, collectedLatLong);

				obj_id_counter++;
				collectedLatLong = {};
			}

			cout << tp << "\n"; //print the data of the string
		}

		//last line has to be manually recognized
		MFC_FixedMultiThread::setupData(obj_id_counter, collectedLatLong);

		newfile.close(); //close the file object.
	}

	return obj_id_counter;
}
