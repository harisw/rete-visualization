#include "SpatialNodeIndexing.h"

RTree<int, float, DIMENSION, float> SpatialNodeIndexing::staticTree;

map<string, vector<int>> SpatialNodeIndexing::staticCQ_anchor_map;
vector<pair<int, Node*>> SpatialNodeIndexing::staticCQ_collectedNode;
vector<string> SpatialNodeIndexing::static_observed_obj_dict;

int SpatialNodeIndexing::AddStaticCQ(Node* n)
{
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

void SpatialNodeIndexing::reset()
{
	staticTree.RemoveAll();

	staticCQ_anchor_map = {};
	staticCQ_collectedNode = {};
	static_observed_obj_dict = {};
}

int SpatialNodeIndexing::AddDynamicCQ(Node* n)
{
	return 0;
}
