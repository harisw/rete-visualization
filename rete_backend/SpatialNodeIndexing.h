#pragma once
#include "Node.h"
#include "ReteNet.h"
#include "RTree.h"
#include "Utilities.h"

#include <algorithm>

#define DIMENSION 3

struct SpatialRect {

	SpatialRect() {
		for (int i = 0; i < DIMENSION; i++) {
			min[i] = 0;
			max[i] = 0;
		}
	}

	SpatialRect(float minx, float miny, float maxx, float maxy, float objEnum) {
		min[0] = minx;
		min[1] = miny;
		min[2] = objEnum;

		max[0] = maxx;
		max[1] = maxy;
		max[2] = objEnum;
	}

	float min[DIMENSION];
	float max[DIMENSION];
};

class SpatialNodeIndexing
{
public:
	static int AddStaticCQ(Node* n);
	static int buildStaticTree();
	static int getStaticCQDictionaryID(string observedObj);
	static vector<int> stabStaticTree(EventPtr ev, string observedObj);
	static void reset();

	static int AddDynamicCQ(Node* n);

private:
	static RTree<int, float, DIMENSION, float> staticTree; //format -> lat, long, enum

	//map format --> node id, vector of corresponding obj type --> most square can be overlapping
	//map example --> enemyVessel - (0,3,4,5) || enemyaircraft - (1,2,6)
	//dictionary must be organized as follow --> enemyvessel - 1, enemyaircraft 2
	static map<string, vector<int>> staticCQ_anchor_map;
	static vector<pair<int, Node*>> staticCQ_collectedNode;
	static vector<string> static_observed_obj_dict;

	//static vector<pair<int, Node*>> dynamicCQ_collected_Node;
};

