#pragma once
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include "SpatialNodeIndexing.h"

#include "MFC_FixedMultiThread.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
// ObjVisualDlg dialog

#define INF 10000 
//#define DEBUGGING_MODE 1
class ObjVisualDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ObjVisualDlg)

public:
	ObjVisualDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ObjVisualDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ObjVisualDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CPen* oldPen;
	CPen greenPen;
	CPen bluePen;
	CPen redPen;
	CBrush hollowBrush;
	CRect objRect;
	typedef boost::geometry::model::d2::point_xy<double> point_type;

	typedef bg::model::point<float, 2, bg::cs::cartesian> point;
	typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon

	float max_first = -999, min_first = 999;
	float max_second = -999, min_second = 999;

	float max_w = 400, min_w = 0;
	float max_h = 300, min_h = 0;
	int rad;
	int distance;
	float x_norm, y_norm;

	bool has_drawn;
	unordered_map<string, int> color_hex_map;
	unordered_map<int, polygon> spatialNodePolygon;
	int global_itt = 0;
	int cycle_step = 10;
	float xCorrection = 0;
	float yCorrection = 0;
public:
	unordered_map<int, vector<pair<float, float>>> m_object_location;

	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	void drawCQVessel(CPaintDC& dc);
	void initObjectVisualization();
	void drawObjects(CPaintDC& dc);
};
