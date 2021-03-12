#pragma once
#include "Node.h"
#include "ReteNet.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include "SpatialNodeIndexing.h"

#include "MFC_FixedMultiThread.h"
//#include "ObjVisualDlg.h"
#include "SimulationThreadDlg.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "AlphaNodeDlg.h"
#include "BetaNodeDlg.h"
//#ifndef SIMUDLG_H
//#define SIMUDLG_H

#define INF 10000 
#define NODE_SIZE 25
#define WIND_WIDTH 1800
#define WIND_HEIGHT 1000
#define RAD_CONST 1.25
#define DIST_CONST 0.8
#define IDT_TIMER_0 WM_USER + 200
#define IDT_TIMER_OBJ_SIMU WM_USER + 201
// SimulationDlg dialog

typedef map<string, Node*> nodeMap;
class SimulationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SimulationDlg)

public:
	SimulationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SimulationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SimulationDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CPoint wmPos;
	int wmRad, xWM;

	vector<Node*> visualizedNode;
	CPen m_oPen;
	CPen* oldPen;
	CPen greenPen;
	CPen bluePen;
	CPen redPen;
	CPen thinPen;
	CPen thickPen;
	CBrush hollowBrush;
	CRect objRect;
	CRect nodesRect;

	//vector<Node*> triggeredNodes;
	int paintMode;
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
	int rad;
	int distance;
	int xStart = 50;
	bool initialRete = true;
	bool nodeUpdate = false;
	int lastUpdateIndex = 0;
	

	vector<AlphaNodeDlg*> m_alphaDlgs;
	vector<BetaNodeDlg*> m_betaDlgs;


	void showAlphaWindow(AlphaNode* nodeInput);
	void showBetaWindow(BetaNode* nodeInput);
	//void findSizeScaling();
	CPoint getPosition(int x, int y);
	void connectNodes(Node*& currNode, vector<Node*>& unconnectedNodes, CClientDC& dc);

	void findSizeScaling(CClientDC& dc);
	void paintWMNode(CClientDC& dc);
	void drawConnections(CClientDC& dc);
	void drawNodes(CClientDC& dc);

	int eventCounter = 0;
	void drawCQVessel(CClientDC& dc);
	void initObjectVisualization();
	void drawObjects();

	//void findSizeScaling(CPaintDC& dc);
	//void paintWMNode(CPaintDC& dc);
	//void drawConnections(CPaintDC& dc);
	//void drawNodes(CPaintDC& dc);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void paintNodeVisual(CPaintDC& dc);
	vector<pair<int, Node*>> nodePositions;
	Node* findClickedNode(CPoint point);

	CDC m_dcMem;
	void appendTextToEditCtrl(string pszText);
	int num_of_obj;
	CListCtrl m_alpha_list_ctrl;
	CListCtrl m_beta_list_ctrl;
	vector<Node*> m_NodeList;
	vector<Node*> m_newNodes;

	CEdit m_output_ctrl;
	SimulationThreadDlg* mp_threadDlg;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	void paintNodeVisual(CClientDC& dc);
	void updateListCtrl();
	void updateNodes();
	void getNodesPosition();

	afx_msg void OnDestroy();
};

//#endif // !SIMUDLG_H