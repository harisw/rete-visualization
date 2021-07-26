//******************************************************************************************************
//This is implementation of Spatial-temporal tactical moving object Management Index based TPR*-tree
//Coded by Bong-hee Hong, Ji-wan Lee, Jae-gi Hong, Ki-jin Kim from Pusan National University
//May 2016
//******************************************************************************************************
#pragma once
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include "Node.h"
#include "CEntry.h"
#include <fstream>
#include <string.h>
#include "ICMSCalculator.h"
#include <stdlib.h>
#include "time.h"
#include <time.h>
// #include <opencv2/core.hpp>

#define MAX_TRACK_NUM 110000
#define MAX_LOCATION_XY 999999.

struct InsertedTrackInfo
{
public:
	int ID;
	double x;
	double y;
	double z;
	double vx;
	double vy;
	double vz;
	double time;
};

struct EntryKNN
{
public:
	CEntry cEntry;
	double DistanceXYZ;

	EntryKNN()
	{
		cEntry = CEntry();
		DistanceXYZ = 0;
	}
};

class TPRTree
{
public:
	enum STATUS{NORMAL, REINSERT};

public:
	TPRTree(void);
	~TPRTree(void);

	int m_TreeID = 0;
	int getTreeID() { return m_TreeID; }


	STATUS m_status;

	int getLevel(){	return m_level;	}
	void setLevel(int _level){ m_level = _level; }
	
	unsigned long getTime(){	return m_time;	}
	void setTime(unsigned long _time){ m_time = _time; }

	Node* getRoot(){	return m_root;	}
	void setRoot(Node* _root){ m_root = _root; }

	void TreeCheck(Node* node);
public: //for insertion
	bool Insert(CEntry _input);// 최초 insert함수이며, InsertRecursive를 호출함
	bool InsertRecursive(Node* _curNode, CEntry _input, double _insertTime);//Recursive하게 choosepath하여 leaf노드에 도달시 insert

	//bool Delete(CEntry _input);
	bool Delete(int _id);
	bool Update(CEntry _input);

	CEntry m_InputEntry;
	int m_PreNodeid;
	int m_NodeIDCtrl;


public: // for Deletion
	bool DeleteRecursive(CEntry _input, Node* _curNode, bool &_bUnderflow, list<CEntry> &_CEntryList, double _deleteTime);
	bool isUnderflow(Node* _curNode);
	bool isUnderflowEntry(Node* _curNode);// cskim

	double CalcNodeEnlargeMBR(Node* _underflowNode, Node* _borrowNode);

public: // for Search
	void rangeQueryKNN(double centralX, double centralY, double centralZ, double radius, vector<EntryKNN> &KNNEntryList);//KNN(질의 시간으로 업데이트 X)
	void rangeQueryKNN4(double centralX, double centralY, double centralZ, double radius, vector<CEntry> &KNNEntryList, double queryTime); //KNN(질의 시간으로 업데이트)

public: // for Extrapolation
	void setTrackInfo(CEntry _input);
	InsertedTrackInfo getTrackInfo(int id);

public:
	unsigned long m_time;
	int m_level;
public :
	Node *m_root;
	InsertedTrackInfo *InsertedTrackList;// [10000];

public:
	Node **m_ObjectNodePosition = new Node*[MAX_TRACK_NUM]; // cskim

	bool RemoveEntry(Node* _curNode, int _id); // cskim



	int getNodeCount(){
		return getNodeCountRecursive(m_root);
	}
	int getNodeCountRecursive(Node *node);

	int getNLeafCount() {
		return getNLeafCountRecursive(m_root);
	}
	int getNLeafCountRecursive(Node *node);

	int getLeafCount() {
		return getLeafCountRecursive(m_root);
	}
	int getLeafCountRecursive(Node *node);

	int getObjectCount() {
		return getObjectCountRecursive(m_root);
	}
	int getObjectCountRecursive(Node *node);

	Node *getRootNode()
	{
		return m_root;
	}



public: // cskim

	bool Clear(void);
	void ClearNodeRecursive(Node *_curNode);

	int update_count;
	int hit_count;
	int false_count;
	int underflow_count;
	int overflow_count;
	int cant_move_count_by_overflow_accul;
	bool overflowed;




};


