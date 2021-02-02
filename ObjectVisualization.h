#pragma once
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include "SpatialNodeIndexing.h"

#include "MFC_FixedMultiThread.h"
#include "RulesVisualDlg.h"
#include <vector>
#include <unordered_map>
#define INF 10000 

// ObjectVisualization dialog

class ObjectVisualization : public CDialog
{
	DECLARE_DYNAMIC(ObjectVisualization)

public:
	ObjectVisualization(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ObjectVisualization();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ObjectVisualization };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int mode;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
private:
	typedef boost::geometry::model::d2::point_xy<double> point_type;

	typedef bg::model::point<float, 2, bg::cs::cartesian> point;
	typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon

	unordered_map<int, vector<pair<float, float>>> m_object_location;
	float max_first = -999, min_first = 999;
	float max_second = -999, min_second = 999;

	float max_w = 400, min_w = 0;
	float max_h = 300, min_h = 0;

	float x_norm, y_norm;

	bool has_drawn;
	unordered_map<string, int> color_hex_map;
	unordered_map<int, polygon> spatialNodePolygon;
	int global_itt = 0;
	int cycle_step = 10;
	float xCorrection = 30;
	float yCorrection = 30;
	void drawCQVessel(CPaintDC& dc);
	void visualizeRule(Node* inpNode);
	vector<Node*> m_NodeList;
	void fillNodes(Node* currentNode);
	void iteratePrevNode(Node* currentNode);
	void iterateNextNode(Node* currentNode);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

};
