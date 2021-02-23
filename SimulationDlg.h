#pragma once
#include "Node.h"
#include "ReteNet.h"
//#include "BetaNode.h"
//#include "AlphaNodeDlg.h"
//#include "BetaNodeDlg.h"
//#include "RulesVisualDlg.h"
//#include "MFC_FixedMultiThread.h"
#include <algorithm>
//#ifndef SIMUDLG_H
//#define SIMUDLG_H

#define NODE_SIZE 25
#define WIND_WIDTH 1800
#define WIND_HEIGHT 1000
#define RAD_CONST 1.25
#define DIST_CONST 0.8
#define IDT_TIMER_0 WM_USER + 200
// SimulationDlg dialog

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
	vector<Node*> visualizedNode;
	CPen m_oPen;
	CPen* oldPen;
	int xCorrection;
	int yCorrection;
	void showAlphaWindow(AlphaNode* nodeInput);
	void showBetaWindow(BetaNode* nodeInput);
	CPoint getPosition(int x, int y);
	void connectNodes(Node*& currNode, vector<Node*>& unconnectedNodes, CClientDC& dc);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void OnLButtonDown(UINT nFlags, CPoint point);
	vector<pair<int, Node*>> nodePositions;
	Node* findClickedNode(CPoint point);

	//SIZE CONSTANTS
	int rad;
	int distance;

	CDC m_dcMem;
	void appendTextToEditCtrl(string pszText);
	int num_of_obj;
	CListCtrl m_alpha_list_ctrl;
	CListCtrl m_beta_list_ctrl;
	vector<Node*> m_NodeList;

	CEdit m_output_ctrl;
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
};

//#endif // !SIMUDLG_H