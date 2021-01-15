#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <boost/geometry/index/rtree.hpp>

#include <boost/foreach.hpp>

#include <algorithm>

#include "Node.h"
#include "ReteNet.h"
#include "RTree.h"
#include "Utilities.h"

#define THE_DIM 3
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

struct SpatialRect {

	SpatialRect() {
		for (int i = 0; i < THE_DIM; i++) {
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

	float min[THE_DIM];
	float max[THE_DIM];
};

class SpatialNodeIndexing
{
public:
	static int AddStaticCQ(Node* n);
	static int buildStaticTree();
	static int getStaticCQDictionaryID(string observedObj);
	static vector<int> stabStaticTree(EventPtr ev, string observedObj);

	static vector<int> stabStaticBoostTree(EventPtr ev, string observedObj);
	static void reset();

	static vector<pair<string, Node*>> setAnchorEntity();

	static int AddDynamicCQ(Node* n);

private:
	static RTree<int, float, THE_DIM, float> staticTree; //format -> lat, long, enum

	//map format --> node id, vector of corresponding obj type --> most square can be overlapping
	//map example --> enemyVessel - (0,3,4,5) || enemyaircraft - (1,2,6)
	//dictionary must be organized as follow --> enemyvessel - 1, enemyaircraft 2
	static map<string, vector<int>> staticCQ_anchor_map;
	static vector<pair<int, Node*>> staticCQ_collectedNode;
	static vector<string> static_observed_obj_dict;

	//now using boost approach, damn tle :"
	typedef bg::model::point<float, 2, bg::cs::cartesian> point;
	typedef bg::model::box<point> box;
	typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon
	typedef std::pair<box, unsigned> value;
	static unordered_map<string, bgi::rtree< value, bgi::rstar<16, 4> >> spatialTree; //observed obj -- the rstar tree
	static unordered_map<string, vector<int>> node_id_dictionary;
	static unordered_map<int, polygon> polygon_dict;

	//static vector<pair<int, Node*>> dynamicCQ_collected_Node;
};

