#include "SpatialNodeIndexing.h"

RTree<int, float, THE_DIM, float> SpatialNodeIndexing::staticTree;

map<string, vector<int>> SpatialNodeIndexing::staticCQ_anchor_map;
vector<pair<int, Node*>> SpatialNodeIndexing::staticCQ_collectedNode;
vector<string> SpatialNodeIndexing::static_observed_obj_dict;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon
typedef std::pair<box, unsigned> value;
unordered_map<string, bgi::rtree< value, bgi::rstar<16, 4> >> SpatialNodeIndexing::spatialTree;
unordered_map<int, polygon>  SpatialNodeIndexing::polygon_dict;

int SpatialNodeIndexing::AddStaticCQ(Node* n)
{
	//std::unordered_map<string, bgi::rtree< value, bgi::rstar<16, 4>>>::const_iterator found = spatialTree.find("allyvessel");

	if (static_cast<BetaNode*>(n)->getSpecialOpName() == "plot_dist") {

		vector<string> result = Utilities::splitDelim(static_cast<BetaNode*>(n)->getLeftConnName(), "(");
		if (result[0] == "rect") {
			//result = Utilities::splitDelim(result[1],")");

			////get the exact location
			//result = Utilities::splitDelim(result[0], ",");
			//vector<float> fetchedVar = {};

			//for (auto n : result) {
			//    float temp = atof(n.c_str());
			//    fetchedVar.push_back(temp);
			//}

			//not exist
			if (staticCQ_anchor_map.find(static_cast<BetaNode*>(n)->getRightConnName()) == staticCQ_anchor_map.end()) {
				staticCQ_anchor_map[static_cast<BetaNode*>(n)->getRightConnName()].push_back(n->getID());
			}
			else {
				//duplicate check
				bool duplicate = false;
				for (auto a : staticCQ_anchor_map[static_cast<BetaNode*>(n)->getRightConnName()]) {
					if (n->getID() == a) {
						duplicate = true;
					}
				}
				if (!duplicate) {
					staticCQ_anchor_map[static_cast<BetaNode*>(n)->getRightConnName()].push_back(n->getID());
				}
			}

			staticCQ_collectedNode.push_back({ n->getID(), n });

			//add this later
			//SpatialRect s(fetchedVar[0], fetchedVar[1], n->getID(), fetchedVar[2], fetchedVar[3], );

//#pragma region Using Boost Polygons
//			polygon poly;
//
//#pragma endregion

		}
		return 1;
	}
	else if (static_cast<BetaNode*>(n)->getSpecialOpName() == "sonar_dist") {

		vector<string> result = Utilities::splitDelim(static_cast<BetaNode*>(n)->getLeftConnName(), "(");
		if (result[0] == "config") {

			std::unordered_map<string, bgi::rtree< value, bgi::rstar<16, 4>>>::const_iterator found = spatialTree.find(static_cast<BetaNode*>(n)->getRightConnName());
			//it exist
			if (found != spatialTree.end()) {
				//box b = bg::return_envelope<box>(polygons[i]);
				//spatialTree[static_cast<BetaNode*>(n)->getRightConnName()].insert();
			}
			//it doesn't exist
			else {

			}

			//not exist
			if (staticCQ_anchor_map.find(static_cast<BetaNode*>(n)->getRightConnName()) == staticCQ_anchor_map.end()) {
				staticCQ_anchor_map[static_cast<BetaNode*>(n)->getRightConnName()].push_back(n->getID());
			}
			else {
				//duplicate check
				bool duplicate = false;
				for (auto a : staticCQ_anchor_map[static_cast<BetaNode*>(n)->getRightConnName()]) {
					if (n->getID() == a) {
						duplicate = true;
					}
				}
				if (!duplicate) {
					staticCQ_anchor_map[static_cast<BetaNode*>(n)->getRightConnName()].push_back(n->getID());
				}
			}

			staticCQ_collectedNode.push_back({ n->getID(), n });
		}

		return 1;
	}
	else if (static_cast<BetaNode*>(n)->getSpecialOpName() == "radar_dist") {

		vector<string> result = Utilities::splitDelim(static_cast<BetaNode*>(n)->getLeftConnName(), "(");
		if (result[0] == "point") {

			std::unordered_map<string, bgi::rtree< value, bgi::rstar<16, 4>>>::const_iterator found = spatialTree.find(static_cast<BetaNode*>(n)->getRightConnName());
			//it exist
			if (found != spatialTree.end()) {
				//box b = bg::return_envelope<box>(polygons[i]);
				//spatialTree[static_cast<BetaNode*>(n)->getRightConnName()].insert();
			}
			//it doesn't exist
			else {

			}

			//not exist
			if (staticCQ_anchor_map.find(static_cast<BetaNode*>(n)->getRightConnName()) == staticCQ_anchor_map.end()) {
				staticCQ_anchor_map[static_cast<BetaNode*>(n)->getRightConnName()].push_back(n->getID());
			}
			else {
				//duplicate check
				bool duplicate = false;
				for (auto a : staticCQ_anchor_map[static_cast<BetaNode*>(n)->getRightConnName()]) {
					if (n->getID() == a) {
						duplicate = true;
					}
				}
				if (!duplicate) {
					staticCQ_anchor_map[static_cast<BetaNode*>(n)->getRightConnName()].push_back(n->getID());
				}
			}

			staticCQ_collectedNode.push_back({ n->getID(), n });
		}

		return 1;
	}

	return 0;
}

int SpatialNodeIndexing::buildStaticTree()
{
	//build the dictionary first
	for (auto dic : staticCQ_anchor_map) {
		static_observed_obj_dict.push_back(dic.first);
	}

	//now build the tree from the node
	for (auto node : staticCQ_collectedNode) {

		vector<string> result = Utilities::splitDelim(static_cast<BetaNode*>(node.second)->getLeftConnName(), "(");
		if (result[0] == "rect") {
			string observedObj = static_cast<BetaNode*>(node.second)->getRightConnName();
			result = Utilities::splitDelim(result[1], ")");

			//get the exact location
			result = Utilities::splitDelim(result[0], ",");
			vector<float> fetchedVar = {};

			for (auto n : result) {
				float temp = atof(n.c_str());
				fetchedVar.push_back(temp);
			}

			int rectEnum = getStaticCQDictionaryID(observedObj);
			/*try
			{
				throw rectEnum;
			}
			catch (int e)
			{
				cout << "unrecognized enumeration" << endl;
			}*/
			if (rectEnum == -1)
				continue;

			//SpatialRect s(fetchedVar[0], fetchedVar[1], fetchedVar[2], fetchedVar[3], rectEnum);
			SpatialRect s(fetchedVar[1], fetchedVar[3], fetchedVar[0], fetchedVar[2], rectEnum);


			staticTree.Insert(s.min, s.max, node.second->getID());
		}
	}


	//address the static tree------------------------------------------------------------------------------------------------
	for (auto node : staticCQ_collectedNode) {

		//build polygon

		vector<string> result = Utilities::splitDelim(static_cast<BetaNode*>(node.second)->getLeftConnName(), "(");
		if (result[0] == "rect") {
			result = Utilities::splitDelim(result[1], ")");

			//get the exact location
			result = Utilities::splitDelim(result[0], ",");
			vector<float> fetchedVar = {};

			for (auto n : result) {
				float temp = atof(n.c_str());
				fetchedVar.push_back(temp);
			}

			box thebox(point(fetchedVar[1], fetchedVar[3]), point(fetchedVar[0], fetchedVar[2]));

			//string observedObj = static_cast<BetaNode*>(node.second)->getRightConnName();
			//int rectEnum = getStaticCQDictionaryID(observedObj);

			//if (rectEnum == -1)
			//	continue;

			//insert into tree ---------------------------------------
			spatialTree[static_cast<BetaNode*>(node.second)->getRightConnName()].insert(std::make_pair(thebox,
				node.second->getID()));

			polygon rect_poly;
			bg::convert(thebox, rect_poly);
			polygon_dict[node.second->getID()] = rect_poly;// bg::wkt<polygon>(p);

			//std::unordered_map<string, bgi::rtree< value, bgi::rstar<16, 4>>>::const_iterator found = spatialTree.find(static_cast<BetaNode*>(node.second)->getRightConnName());
			////it exist
			//if (found != spatialTree.end()) {
			//	//box b = bg::return_envelope<box>(polygons[i]);
			//	//node_id_dictionary[ = spatialTree[static_cast<BetaNode*>(node.second)->getRightConnName()].size()
			//	spatialTree.at(static_cast<BetaNode*>(node.second)->getRightConnName()).insert(std::make_pair(thebox, spatialTree.at(static_cast<BetaNode*>(node.second)->getRightConnName()).size()));
			//}
			////it doesn't exist
			//else {
			//	spatialTree[static_cast<BetaNode*>(node.second)->getRightConnName()].insert(std::make_pair(thebox, spatialTree[static_cast<BetaNode*>(node.second)->getRightConnName()].size()));
			//}


		}
		else if (result[0] == "config") { //sonar
			string ori_expression = static_cast<BetaNode*>(node.second)->getLeftConnName();
			ori_expression = ori_expression.substr(ori_expression.find("("), ori_expression.length() - 5);

			vector<string> tempRes = Utilities::splitDelim(ori_expression, ",");

			vector<float> fetchedValue;
			//get the start degree --> temp res 0 and 1
			int LeftBr = ori_expression.find("((");
			float range = atof(tempRes[0].substr(2, tempRes[0].length()).c_str());
			fetchedValue.push_back(range);

			range = atof(tempRes[1].substr(0, tempRes[1].length() - 1).c_str());
			fetchedValue.push_back(range);

			//get the distance --> temp res 2
			fetchedValue.push_back(atof(tempRes[2].c_str()));

			//get the entitiy name --> temp res 3
			int ispoint = -1;
			ispoint = tempRes[3].find("(");
			if (ispoint > -1) {//it is a point
				fetchedValue.push_back(atof(tempRes[3].substr(1, tempRes[3].length()).c_str()));
				fetchedValue.push_back(atof(tempRes[4].substr(0, tempRes[4].length() - 1).c_str()));
			}

			//fetched value order
			//0 - deg_min
			//1 - deg_max
			//2 - distance
			//3 - point_long
			//4 - point_lat

			//build the cone
			float rad;
			float tempX, tempY;

			//minimum ---------------------------------------------------
			rad = fetchedValue[0] * 3.1416 / 180;
			tempX = fetchedValue[4] + (fetchedValue[2] * std::cos(rad));
			tempY = fetchedValue[3] + (fetchedValue[2] * std::sin(rad));
			point min(tempX, tempY);

			//max -------------------------------------------------------
			rad = fetchedValue[1] * 3.1416 / 180;
			tempX = fetchedValue[4] + (fetchedValue[2] * std::cos(rad));
			tempY = fetchedValue[3] + (fetchedValue[2] * std::sin(rad));
			point max(tempX, tempY);

			//mid -------------------------------------------------------
			rad = (fetchedValue[0] + (abs(fetchedValue[1] - fetchedValue[0]) / 2)) * 3.1416 / 180;
			tempX = fetchedValue[4] + (fetchedValue[2] * std::cos(rad));
			tempY = fetchedValue[3] + (fetchedValue[2] * std::sin(rad));
			point mid(tempX, tempY);

			point ori(fetchedValue[4], fetchedValue[3]);


			//build polygon
			polygon p1;
			p1.outer().push_back(ori);
			p1.outer().push_back(min);
			p1.outer().push_back(mid);
			p1.outer().push_back(max);
			//p1.outer().push_back(ori);
			//p1.outer().push_back(mid);
			//p1.outer().push_back(min);
			//p1.outer().push_back(max);

			//insert into tree ---------------------------------------
			box b = bg::return_envelope<box>(p1);
			spatialTree[static_cast<BetaNode*>(node.second)->getRightConnName()].insert(std::make_pair(b, node.second->getID()));

			polygon_dict[node.second->getID()] = p1;
			//int a = 11;
		}
		else if (result[0] == "point") { // radar

		//radar will be visualized as 10 sided polygon / decagon
			string ori_expression = static_cast<BetaNode*>(node.second)->getLeftConnName();
			ori_expression = ori_expression.substr(ori_expression.find("("), ori_expression.length() - 5);

			vector<string> tempRes = Utilities::splitDelim(ori_expression, ",");

			vector<float> fetchedValue;
			//get the point location --> temp res 0 and 1
			float range = atof(tempRes[0].substr(1, tempRes[0].length()).c_str());
			fetchedValue.push_back(range);

			range = atof(tempRes[1].substr(0, tempRes[1].length() - 1).c_str());
			fetchedValue.push_back(range);

			//distance
			range = atof(tempRes[2].c_str());
			fetchedValue.push_back(range);

			//fetched value order
			//0 - point long
			//1 - point lat
			//2 - distance

			//build the decagon
			float rad;
			float tempX, tempY;
			vector<point> sides;
			polygon decagon;
			for (int i = 0; i < 10; i++) {
				rad = (360 - (i * 36)) * 3.1416 / 180;
				tempX = fetchedValue[1] + (fetchedValue[2] * std::cos(rad));
				tempY = fetchedValue[0] + (fetchedValue[2] * std::sin(rad));
				//sides.push_back(point(tempX, tempY));
				decagon.outer().push_back(point(tempX, tempY));
			}

			//insert into tree ---------------------------------------
			box b = bg::return_envelope<box>(decagon);
			spatialTree[static_cast<BetaNode*>(node.second)->getRightConnName()].insert(std::make_pair(b, node.second->getID()));

			polygon_dict[node.second->getID()] = decagon;
			//int a = 11;
		}
	}

	return 1;
}

int SpatialNodeIndexing::getStaticCQDictionaryID(string observedObj)
{
	for (int i = 0; i < static_observed_obj_dict.size(); i++) {
		if (static_observed_obj_dict[i] == observedObj)
			return i;
	}
	return -1;
}

vector<int> SpatialNodeIndexing::stabStaticTree(EventPtr ev, string observedObj)
{
	int objEnum = getStaticCQDictionaryID(observedObj);

	float xpos[3], ypos[3];
	xpos[0] = xpos[1] = ev->getFloat("lat");
	ypos[0] = ypos[1] = ev->getFloat("lon");

	xpos[2] = ypos[2] = objEnum; // objEnum is dictionary id

	vector<int> res = staticTree.Search_vec(xpos, ypos, NULL, NULL);

	//Event *temp = ev.front()->clone();

	return res;
}

vector<int> SpatialNodeIndexing::stabStaticBoostTree(EventPtr ev, string observedObj)
{
	//	box query_box(point(ev., 5), point(0, 6));
	vector<int> result_node_id;
	point poi(ev->getFloat("lat"), ev->getFloat("lon"));
	std::vector<value> result_s;
	spatialTree[observedObj].query(bgi::intersects(poi), std::back_inserter(result_s));

	BOOST_FOREACH(value const& v, result_s) {
		//need validation
		bool b = boost::geometry::intersects(polygon_dict[v.second], poi);
		if (b) {
			result_node_id.push_back(v.second);
		}
		//std::cout << "Intersects: " << (b ? "YES" : "NO") << std::endl;
		//std::deque<polygon> output;
		//bg::intersection(bg::wkt<polygon>(polygons[v.second]), bg::wkt<polygon>(polygons[v.second]), output);
	}

	return result_node_id;
}

void SpatialNodeIndexing::reset()
{
	staticTree.RemoveAll();

	staticCQ_anchor_map = {};
	staticCQ_collectedNode = {};
	static_observed_obj_dict = {};

	spatialTree = {};
}

vector<pair<string, Node*>> SpatialNodeIndexing::setAnchorEntity()
{
	//vector<pair<string, Node*>> result;
	//for (auto n : staticCQ_anchor_map) {
	//	result.push_back()
	//}
	return vector<pair<string, Node*>>();
}

int SpatialNodeIndexing::AddDynamicCQ(Node* n)
{
	return 0;
}

unordered_map<int, polygon> SpatialNodeIndexing::getExistingPolygons()
{
	return polygon_dict;
}
