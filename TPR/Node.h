//******************************************************************************************************
//This is implementation of Spatial-temporal tactical moving object Management Index based TPR*-tree
//Coded by Bong-hee Hong, Ji-wan Lee, Jae-gi Hong, Ki-jin Kim from Pusan National University
//May 2016
//******************************************************************************************************
#pragma once

#define TREE_TYPE_MEMORY_OR_DISK 1

#define CAPACITY 21
#define NUMNODE 5
#define UNDERFLOW_RATIO 0.4
#define PI 3.14159265f



#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


#include "CEntry.h"

#include <iostream>
#include <list>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include "time.h"
#include <stdio.h>
#include <algorithm>
#include <fstream>

using namespace std;


struct indexXY{ // ��Ʈ���� ���������� index������ ��������, ���� �� split�� Ȱ��
	int id;
	int time;
	double x;
	double y;
	double z;
	double vx;
	double vy;
	double vz;
	int index;
	unsigned long uiTOVCreated;			//�����ð� ��(1970�� ���� ��)
	unsigned long uiTOVCreated_us;		//�����ð� ����ũ�� ��
	unsigned long m_uiTOVUpdated;		//���Žð� �� (1970�� ���� ��)
	unsigned long m_uiTOVUpdated_us;	//���Žð� ����ũ�� ��

	unsigned char byTrackStatus;        //_ENUM_TRACK_STATUS
	int m_byEnvironment;
};

struct SplitAxis{ // Split�� ��(x,y)�� index ���� ���� ����ü
	int splitIndex;
	bool isX;
	double pre[4];
	double next[4];
	double preV[6];
	double nextV[6];
	double minPerimeter;
};

class Node
{
public: // cskim
	Node(double _time, Node **nodes, int _treeID);
	Node(Node **nodes);

	int  treeID;
	
public:
	Node(double _time);
	Node(bool isNull);
	~Node(void);

	int getLevel(){	return m_level;	}

	void setLevel(int _level){ m_level = _level; }
	double* getMBR(){
		return m_MBR;
	}
	void setMBR(double* _MBR){
		memcpy(m_MBR, _MBR, sizeof(double)*4);
	}

	double* getExtMBR(){return m_extMBR;}
	void setextMBR(double* _extMBR){
		memcpy(m_extMBR, _extMBR, sizeof(double)*4);
	}

	double* getVBR(){
		return m_VBR;
	}
	void setVBR(double* _VBR){
		memcpy(m_VBR, _VBR, sizeof(double)*6);
	}
	Node* getChildNode(int _index){
		return m_childNode[_index];
	}
	void setChildNode(Node* _childNode)
	{
		m_childNode[m_NumCntChild++] = _childNode;

		// Sunghoon
		/*
		m_MBR[0] = min(m_MBR[0], _childNode->getMBR()[0]);
		m_MBR[1] = min(m_MBR[1], _childNode->getMBR()[1]);
		m_MBR[2] = max(m_MBR[2], _childNode->getMBR()[2]);
		m_MBR[3] = max(m_MBR[3], _childNode->getMBR()[3]);

		m_VBR[0] = min(m_VBR[0], _childNode->getVBR()[0]);
		m_VBR[1] = min(m_VBR[1], _childNode->getVBR()[1]);
		m_VBR[2] = min(m_VBR[2], _childNode->getVBR()[2]);
		m_VBR[3] = max(m_VBR[3], _childNode->getVBR()[3]);
		m_VBR[4] = max(m_VBR[4], _childNode->getVBR()[4]);
		m_VBR[5] = max(m_VBR[5], _childNode->getVBR()[5]);
		*/
	}
	void delChildNode(Node* _childNode);


	Node* getParent(){
		return m_parent;
	}
	void setParent(Node* _parent){
		m_parent = _parent;
	}
	CEntry* getEntry(){
		return m_entry;
	}
	void setEntry(CEntry _data, int _index){
		memcpy(&m_entry[_index],&_data,sizeof(_data));
	}
	int getNumEntrys();
	void setNumEntrys(int _numEntrys){
		m_NumCntEntries = _numEntrys; 
	}

	int getID(){ return m_ID;}
	void setID(int _id){ 
		m_ID = _id;
	}

	void updateChildNode(Node* _childNode, int _index)
	{
		m_childNode[_index] = _childNode;
	}
	int getNumCntChild(){return m_NumCntChild;}

	void setNumCntChild(int _numCntChild){m_NumCntChild = _numCntChild;}
	void setTime(double _time){m_time = _time;}
	double getTime(){return m_time;}
	
public: //for insert
	bool Insert(CEntry _InsertEntry);
	void UpdateMBRbyEntry(double _time);
	void UpdateMBRbyNode(double _time);
	void UpdateMBR(double _time);

	Node* ChooseChildPath(CEntry _InsertEntry, double _time);
	double CalcEnlargeMBR(CEntry _InsertEntry, double _time);

public: //for overflow
	CEntry Pickworst();
	bool RemoveEntry(int _ID);
	bool RemoveAllEntry();
	double getsubArea(double _NodeArea, int _delEntryIndex);

public: //for split
	void NodeSplit(Node* _curNode, double _time, Node* _newNode);
	SplitAxis SetSplitAxis(indexXY* _mbrX, indexXY* _mbrY, Node* _OverflowNode);
	SplitAxis minPerimeter(indexXY* _pre,indexXY* _next); // �ѷ��� ���� ���� ��Ʈ���� ������ �����Ͽ� Split�� index�� Split�� ���� x������ y������ ������
	void MBRCopy(indexXY* _dest, indexXY* _start); // indexXY ����ü���� MBR ����
	void StructtoEntryCopy(CEntry* _dest, indexXY* _start); // indexXY ����ü ���� Entry�� ����
	void EntrytoStructCopy(indexXY* _dest, CEntry* _start); // Entry ���� indexXY ����ü�� ����

	void NonleafNodeSplit(Node* _curNode, double _time, Node* _newNode);
	SplitAxis minNodePerimeter(indexXY* _XGroup,indexXY* _YGroup, Node* _OverflowNode); // isX: X������ SPLIT�ϸ� true, Y������ SPLIT�ϸ� false
	SplitAxis SetNodeSplitAxis(indexXY* _mbrX, indexXY* _mbrY, Node* _OverflowNode);
	void NodetoStructCopy(indexXY* _dest, Node* _start);

	void SetExistingNode(Node* _curNode, SplitAxis _splitAxis, indexXY* _mbrX, indexXY* _mbrY);
	void SetNewNode(SplitAxis _splitAxis, indexXY* _mbrX, indexXY* _mbrY, double _time, Node* _newNode);

	void UpdateParentNode();
	void UpdateMBRbyExt(double _time);

public: // for delete
	bool Included(CEntry _input);
	void SortChildNode();

	int m_ID;

public: //for circle search
	bool RectangleInsect(double minX, double minY,double maxX, double maxY, double centralX, double centralY, double radius, int id);

public:// for Extrapolation
	void extfuture_mbr(double *_extMbr,double *_mbr, double *_vbr, double _time, int _dim);
	void extfuture_mbr_of_node(double *_extMbr, Node *_node, double _time, int _dim); // csk

public: // for KNN
	void rangeQueryKNN(double centralX, double centralY, double radius, unsigned long  _st, unsigned long  _ed, int& _rsltcnt, vector<CEntry> &_CEntryList, double queryTime);


	///file
	void writeEntryFile();
	void readEntryFile();
	void allocEntryMemory();
	void freeEntryMemory();


protected:
	
	int m_level;
	int m_NumCntEntries;
	int m_NumCntChild;

	double m_MBR[4];
	double m_extMBR[4];
	double m_VBR[6];
	
	Node* m_parent;
	//CEntry m_entry[CAPACITY+1];
	CEntry *m_entry;

	unsigned long m_time;

public: // CSKIM
	Node **m_ObjectNodePositionRef;
	Node* m_childNode[NUMNODE + 1];	// Sunghoon CAPACITY -> NUMNODE

	bool checkEntryIncluded(int id) 
	{ // CSKIM
		bool ret = false;
		int i;

		for (i = 0; i < getNumEntrys(); i++)
		{
			if (m_entry[i].getID() == id) break;
		}
		if (i < getNumEntrys()) return true; // included
		else return false; 
	}

	int node_visit_count;
	int founded_obj_count;


	double GetVelocity(CEntry _entry)
	{
		return sqrt(pow(_entry.getVX(), 2) + pow(_entry.getVY(), 2));
	}

};


