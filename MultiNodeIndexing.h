#pragma once
#include "Node.h"
//#include "ReteNet.h"
#include "RTree.h"
#include "Utilities.h"
#include "AlphaNode.h"
#include "BetaNode.h"
#include <algorithm>

#define MAX_LIM 9999
#define MIN_LIM -999
#define THE_DIM 10

struct RectHelper
{
	RectHelper() {
		for (int i = 0; i < THE_DIM; i++) {
			min[i] = 0;
			max[i] = 0;
		}
	}

	RectHelper(int affectedIndex, float x_min, float x_max) {
		for (int i = 0; i < THE_DIM; i++) {
			if (affectedIndex == i) {
				min[i] = x_min;
				max[i] = x_max;
			}
			else {
				min[i] = MIN_LIM;
				max[i] = MAX_LIM;
			}
		}
	}

	void searchAndSet(int affectedIndex, string value) {
		if (atof(value.c_str()) != 0) { //it is a number!!!
			min[affectedIndex] = atof(value.c_str());
			max[affectedIndex] = atof(value.c_str());
		}
		else {
			if (value == "true") { //
				min[affectedIndex] = 1;
				max[affectedIndex] = 1;
			}
			else {
				min[affectedIndex] = -1;
				max[affectedIndex] = -1;
			}
		}
	}

	RectHelper(int affectedIndex, string value) {
		if (atof(value.c_str()) != 0) { //it is a number!!!
			min[affectedIndex] = atof(value.c_str());
			max[affectedIndex] = atof(value.c_str());
		}
		else {
			if (value == "true") {
				min[affectedIndex] = 1;
				max[affectedIndex] = 1;
			}
			else {
				min[affectedIndex] = 0;
				max[affectedIndex] = 0;
			}
		}
	}

	float min[THE_DIM];
	float max[THE_DIM];
};

class MultiNodeIndexing
{
public:
	static int checkAttrMap(string attrName);
	static int addAlphaNode(Node* node);

	static int returnFinalNode(EventPtr ep);

	static void createCombination();
	static void BuildTree();
	static vector<pair<int, pair<int, int>>> adjustRange(vector<Node*> theData);
	static void freshInsert(int attrIndex, Node* node);
	static void updateInsert(int attrIndex, Node* node);

	static bool sortByMapIndex(pair<int, Node*> a, pair<int, Node*> b);
	static bool sortByUpperBound(pair<int, pair<int, int>> a, pair<int, pair<int, int>> b);
	static bool sortByLowerBound(pair<int, pair<int, int>> a, pair<int, pair<int, int>> b);

	static vector<int> stabEvent(EventPtr ep);

	static unordered_map<int, int> stabEventMap(EventPtr ptr);

	static void reset();
	static int clearTheTree();

private:
	static RTree<int, float, 10, float> multiDimensionalTree;
	static unordered_map<string, int> attributes_map;
	static unordered_map<int, vector<int>> combinationMap;

	static unordered_map<string, int> hashCombination;

	static vector<pair<int, Node*>> collectedAlphaNodes;
};

