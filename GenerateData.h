#pragma once

#include <iostream>
#include <fstream>

#include "MFC_FixedMultiThread.h"
#include "CQObject.h"

//#include <vector>

using namespace std;

class GenerateData
{
public:
	static void Generate(vector<pair<int,int>> input_vector, int obj_num, int time_len);
	static int ReadFromFile(string filePath);
private:
	static vector<POINT_GEN> converted_vector;
};

